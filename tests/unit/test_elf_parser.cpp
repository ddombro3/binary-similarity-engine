#include <bsim/formats/elf_parser.hpp>

#include <cassert>
#include <cstddef>
#include <cstring>
#include <elf.h>
#include <iostream>
#include <vector>

void test_valid_elf64() {
    constexpr std::size_t section_count = 3;

    std::vector<std::byte> bytes(
        sizeof(Elf64_Ehdr) + (section_count * sizeof(Elf64_Shdr))
    );

    Elf64_Ehdr header{};
    header.e_ident[EI_MAG0] = ELFMAG0;
    header.e_ident[EI_MAG1] = ELFMAG1;
    header.e_ident[EI_MAG2] = ELFMAG2;
    header.e_ident[EI_MAG3] = ELFMAG3;
    header.e_ident[EI_CLASS] = ELFCLASS64;
    header.e_ident[EI_DATA] = ELFDATA2LSB;

    header.e_shoff = sizeof(Elf64_Ehdr);
    header.e_shentsize = sizeof(Elf64_Shdr);
    header.e_shnum = section_count;

    std::memcpy(bytes.data(), &header, sizeof(header));

    Elf64_Shdr executable_section{};
    executable_section.sh_flags = SHF_EXECINSTR;

    Elf64_Shdr normal_section{};
    normal_section.sh_flags = SHF_ALLOC;

    std::memcpy(
        bytes.data() + sizeof(Elf64_Ehdr) + sizeof(Elf64_Shdr),
        &executable_section,
        sizeof(executable_section)
    );

    std::memcpy(
        bytes.data() + sizeof(Elf64_Ehdr) + (2 * sizeof(Elf64_Shdr)),
        &normal_section,
        sizeof(normal_section)
    );

    const auto result = bsim::parse_elf(bytes);

    assert(result.has_value());
    assert(result->section_count == 3);
    assert(result->executable_section_count == 1);
}

void test_invalid_magic() {
    std::vector<std::byte> bytes(sizeof(Elf64_Ehdr));

    const auto result = bsim::parse_elf(bytes);

    assert(!result.has_value());
}

void test_truncated_file() {
    const std::vector<std::byte> bytes{
        std::byte{0x7F},
        std::byte{'E'},
        std::byte{'L'},
        std::byte{'F'}
    };

    const auto result = bsim::parse_elf(bytes);

    assert(!result.has_value());
}

int main() {
    test_valid_elf64();
    test_invalid_magic();
    test_truncated_file();

    std::cout << "ELF parser tests passed\n";

    return 0;
}