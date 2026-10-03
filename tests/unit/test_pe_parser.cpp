#include <bsim/formats/pe_parser.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

void write_u16_le(std::vector<std::byte>& bytes, std::size_t offset, std::uint16_t value) {
    bytes[offset] = static_cast<std::byte>(value & 0xFF);
    bytes[offset + 1] = static_cast<std::byte>((value >> 8) & 0xFF);
}

void write_u32_le(std::vector<std::byte>& bytes, std::size_t offset, std::uint32_t value) {
    bytes[offset] = static_cast<std::byte>(value & 0xFF);
    bytes[offset + 1] = static_cast<std::byte>((value >> 8) & 0xFF);
    bytes[offset + 2] = static_cast<std::byte>((value >> 16) & 0xFF);
    bytes[offset + 3] = static_cast<std::byte>((value >> 24) & 0xFF);
}

void test_valid_pe() {
    constexpr std::size_t pe_offset = 0x80;
    constexpr std::size_t pe_header_size = 24;
    constexpr std::size_t section_header_size = 40;
    constexpr std::size_t section_count = 2;

    std::vector<std::byte> bytes(
        pe_offset + pe_header_size + (section_count * section_header_size)
    );

    bytes[0] = std::byte{'M'};
    bytes[1] = std::byte{'Z'};

    write_u32_le(bytes, 0x3C, pe_offset);

    bytes[pe_offset] = std::byte{'P'};
    bytes[pe_offset + 1] = std::byte{'E'};
    bytes[pe_offset + 2] = std::byte{0x00};
    bytes[pe_offset + 3] = std::byte{0x00};

    write_u16_le(bytes, pe_offset + 6, section_count);
    write_u16_le(bytes, pe_offset + 20, 0);

    const std::size_t section_table_offset = pe_offset + pe_header_size;

    write_u32_le(
        bytes,
        section_table_offset + 36,
        0x20000000
    );

    write_u32_le(
        bytes,
        section_table_offset + section_header_size + 36,
        0x40000000
    );

    const auto result = bsim::parse_pe(bytes);

    assert(result.has_value());
    assert(result->section_count == 2);
    assert(result->executable_section_count == 1);
}

void test_invalid_dos_signature() {
    std::vector<std::byte> bytes(128);

    const auto result = bsim::parse_pe(bytes);

    assert(!result.has_value());
}

void test_invalid_pe_signature() {
    constexpr std::size_t pe_offset = 0x40;

    std::vector<std::byte> bytes(128);

    bytes[0] = std::byte{'M'};
    bytes[1] = std::byte{'Z'};

    write_u32_le(bytes, 0x3C, pe_offset);

    const auto result = bsim::parse_pe(bytes);

    assert(!result.has_value());
}

void test_truncated_file() {
    const std::vector<std::byte> bytes{
        std::byte{'M'},
        std::byte{'Z'}
    };

    const auto result = bsim::parse_pe(bytes);

    assert(!result.has_value());
}

int main() {
    test_valid_pe();
    test_invalid_dos_signature();
    test_invalid_pe_signature();
    test_truncated_file();

    std::cout << "PE parser tests passed\n";

    return 0;
}