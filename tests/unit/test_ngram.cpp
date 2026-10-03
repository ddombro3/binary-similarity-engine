#include <bsim/features/ngram.hpp>

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>

void test_default_four_byte_ngrams() {
    constexpr std::array bytes{
        std::byte{0xAA},
        std::byte{0xBB},
        std::byte{0xCC},
        std::byte{0xDD},
        std::byte{0xEE}
    };

    const auto ngrams = bsim::extract_ngrams(bytes);

    assert(ngrams.size() == 2);
    assert(ngrams[0] == 0xAABBCCDD);
    assert(ngrams[1] == 0xBBCCDDEE);
}

void test_custom_ngram_size() {
    constexpr std::array bytes{
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03}
    };

    const auto ngrams = bsim::extract_ngrams(bytes, 2);

    assert(ngrams.size() == 2);
    assert(ngrams[0] == 0x0102);
    assert(ngrams[1] == 0x0203);
}

void test_input_smaller_than_ngram() {
    constexpr std::array bytes{
        std::byte{0x01},
        std::byte{0x02}
    };

    const auto ngrams = bsim::extract_ngrams(bytes, 4);

    assert(ngrams.empty());
}

void test_zero_ngram_size() {
    constexpr std::array bytes{
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03}
    };

    const auto ngrams = bsim::extract_ngrams(bytes, 0);

    assert(ngrams.empty());
}

void test_ngram_size_too_large() {
    constexpr std::array bytes{
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03},
        std::byte{0x04},
        std::byte{0x05},
        std::byte{0x06},
        std::byte{0x07},
        std::byte{0x08},
        std::byte{0x09}
    };

    const auto ngrams = bsim::extract_ngrams(bytes, 9);

    assert(ngrams.empty());
}

void test_empty_input() {
    const std::span<const std::byte> bytes{};

    const auto ngrams = bsim::extract_ngrams(bytes);

    assert(ngrams.empty());
}

int main() {
    test_default_four_byte_ngrams();
    test_custom_ngram_size();
    test_input_smaller_than_ngram();
    test_zero_ngram_size();
    test_ngram_size_too_large();
    test_empty_input();

    std::cout << "N-gram tests passed\n";

    return 0;
}