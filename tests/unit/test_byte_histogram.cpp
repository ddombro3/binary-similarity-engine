#include <bsim/features/byte_histogram.hpp>

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>

void test_empty_input() {
    const std::span<const std::byte> bytes{};

    const auto histogram = bsim::calculate_byte_histogram(bytes);

    for (const auto frequency : histogram) {
        assert(frequency == 0.0);
    }
}

void test_single_byte_value() {
    constexpr std::array bytes{
        std::byte{0xAA},
        std::byte{0xAA},
        std::byte{0xAA},
        std::byte{0xAA}
    };

    const auto histogram = bsim::calculate_byte_histogram(bytes);

    assert(std::abs(histogram[0xAA] - 1.0) < 0.001);

    for (std::size_t i = 0; i < histogram.size(); ++i) {
        if (i != 0xAA) {
            assert(histogram[i] == 0.0);
        }
    }
}

void test_uniform_distribution() {
    constexpr std::array bytes{
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03}
    };

    const auto histogram = bsim::calculate_byte_histogram(bytes);

    assert(std::abs(histogram[0x00] - 0.25) < 0.001);
    assert(std::abs(histogram[0x01] - 0.25) < 0.001);
    assert(std::abs(histogram[0x02] - 0.25) < 0.001);
    assert(std::abs(histogram[0x03] - 0.25) < 0.001);
}

void test_histogram_sums_to_one() {
    constexpr std::array bytes{
        std::byte{0x10},
        std::byte{0x20},
        std::byte{0x30},
        std::byte{0x40},
        std::byte{0x50}
    };

    const auto histogram = bsim::calculate_byte_histogram(bytes);

    double sum = 0.0;

    for (const auto frequency : histogram) {
        sum += frequency;
    }

    assert(std::abs(sum - 1.0) < 0.001);
}

int main() {
    test_empty_input();
    test_single_byte_value();
    test_uniform_distribution();
    test_histogram_sums_to_one();

    std::cout << "Byte histogram tests passed\n";

    return 0;
}