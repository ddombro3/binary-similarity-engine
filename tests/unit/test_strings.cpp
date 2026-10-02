#include <bsim/features/strings.hpp>

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>

void test_empty_input() {
    const std::span<const std::byte> bytes{};

    const auto strings = bsim::extract_strings(bytes);

    assert(strings.empty());
}

void test_extract_single_string() {
    constexpr std::array bytes{
        std::byte{'t'},
        std::byte{'e'},
        std::byte{'s'},
        std::byte{'t'}
    };

    const auto strings = bsim::extract_strings(bytes);

    assert(strings.size() == 1);
    assert(strings[0] == "test");
}

void test_extract_multiple_strings() {
    constexpr std::array bytes{
        std::byte{'h'},
        std::byte{'e'},
        std::byte{'l'},
        std::byte{'l'},
        std::byte{'o'},
        std::byte{0x00},
        std::byte{'w'},
        std::byte{'o'},
        std::byte{'r'},
        std::byte{'l'},
        std::byte{'d'}
    };

    const auto strings = bsim::extract_strings(bytes);

    assert(strings.size() == 2);
    assert(strings[0] == "hello");
    assert(strings[1] == "world");
}

void test_minimum_length() {
    constexpr std::array bytes{
        std::byte{'a'},
        std::byte{'b'},
        std::byte{'c'},
        std::byte{0x00},
        std::byte{'t'},
        std::byte{'e'},
        std::byte{'s'},
        std::byte{'t'}
    };

    const auto strings = bsim::extract_strings(bytes);

    assert(strings.size() == 1);
    assert(strings[0] == "test");
}

void test_custom_minimum_length() {
    constexpr std::array bytes{
        std::byte{'a'},
        std::byte{'b'},
        std::byte{'c'},
        std::byte{0x00},
        std::byte{'d'},
        std::byte{'e'}
    };

    const auto strings = bsim::extract_strings(bytes, 2);

    assert(strings.size() == 2);
    assert(strings[0] == "abc");
    assert(strings[1] == "de");
}

void test_non_printable_bytes_split_strings() {
    constexpr std::array bytes{
        std::byte{'A'},
        std::byte{'B'},
        std::byte{'C'},
        std::byte{'D'},
        std::byte{0xFF},
        std::byte{'E'},
        std::byte{'F'},
        std::byte{'G'},
        std::byte{'H'}
    };

    const auto strings = bsim::extract_strings(bytes);

    assert(strings.size() == 2);
    assert(strings[0] == "ABCD");
    assert(strings[1] == "EFGH");
}

int main() {
    test_empty_input();
    test_extract_single_string();
    test_extract_multiple_strings();
    test_minimum_length();
    test_custom_minimum_length();
    test_non_printable_bytes_split_strings();

    std::cout << "String extraction tests passed\n";

    return 0;
}