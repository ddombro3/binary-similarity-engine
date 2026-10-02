#include <bsim/formats/binary_format.hpp>

#include <array>
#include <cassert>
#include <cstddef>
#include <iostream>

void test_detect_elf() {
    constexpr std::array bytes{
        std::byte{0x7F},
        std::byte{'E'},
        std::byte{'L'},
        std::byte{'F'}
    };

    assert(bsim::detect_binary_format(bytes) == bsim::BinaryFormat::elf);
}

void test_detect_pe() {
    constexpr std::array bytes{
        std::byte{'M'},
        std::byte{'Z'}
    };

    assert(bsim::detect_binary_format(bytes) == bsim::BinaryFormat::pe);
}

void test_detect_unknown() {
    constexpr std::array bytes{
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03}
    };

    assert(bsim::detect_binary_format(bytes) == bsim::BinaryFormat::unknown);
}

void test_empty_input() {
    const std::span<const std::byte> bytes{};

    assert(bsim::detect_binary_format(bytes) == bsim::BinaryFormat::unknown);
}

void test_incomplete_elf_header() {
    constexpr std::array bytes{
        std::byte{0x7F},
        std::byte{'E'},
        std::byte{'L'}
    };

    assert(bsim::detect_binary_format(bytes) == bsim::BinaryFormat::unknown);
}

int main() {
    test_detect_elf();
    test_detect_pe();
    test_detect_unknown();
    test_empty_input();
    test_incomplete_elf_header();

    std::cout << "Binary format tests passed\n";

    return 0;
}