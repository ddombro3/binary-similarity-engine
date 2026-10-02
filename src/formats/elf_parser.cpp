#include <bsim/formats/elf_parser.hpp>

#include <cstring>
#include <elf.h>

namespace bsim {

template <typename ElfHeader, typename SectionHeader>
std::optional<ElfStructure> parse_elf_impl(std::span<const std::byte> bytes) noexcept {
    if (bytes.size() < sizeof(ElfHeader)) {
        return std::nullopt;
    }

    ElfHeader header{};
    std::memcpy(&header, bytes.data(), sizeof(header));

    const std::size_t section_offset = static_cast<std::size_t>(header.e_shoff);
    const std::size_t section_count = static_cast<std::size_t>(header.e_shnum);
    const std::size_t section_entry_size = static_cast<std::size_t>(header.e_shentsize);

    if (section_entry_size < sizeof(SectionHeader)) {
        return std::nullopt;
    }

    if (section_offset > bytes.size()) {
        return std::nullopt;
    }

    if (section_count > (bytes.size() - section_offset) / section_entry_size) {
        return std::nullopt;
    }

    ElfStructure structure{};
    structure.section_count = section_count;

    for (std::size_t i = 0; i < section_count; ++i) {
        const std::size_t offset = section_offset + (i * section_entry_size);

        SectionHeader section{};
        std::memcpy(&section, bytes.data() + offset, sizeof(section));

        if ((section.sh_flags & SHF_EXECINSTR) != 0) {
            ++structure.executable_section_count;
        }
    }

    return structure;
}

std::optional<ElfStructure> parse_elf(std::span<const std::byte> bytes) noexcept {
    if (bytes.size() < EI_NIDENT) {
        return std::nullopt;
    }

    if (bytes[EI_MAG0] != std::byte{ELFMAG0} ||
        bytes[EI_MAG1] != std::byte{'E'} ||
        bytes[EI_MAG2] != std::byte{'L'} ||
        bytes[EI_MAG3] != std::byte{'F'}) {
        return std::nullopt;
    }

    if (std::to_integer<unsigned char>(bytes[EI_DATA]) != ELFDATA2LSB) {
        return std::nullopt;
    }

    const auto elf_class = std::to_integer<unsigned char>(bytes[EI_CLASS]);

    if (elf_class == ELFCLASS64) {
        return parse_elf_impl<Elf64_Ehdr, Elf64_Shdr>(bytes);
    }

    if (elf_class == ELFCLASS32) {
        return parse_elf_impl<Elf32_Ehdr, Elf32_Shdr>(bytes);
    }

    return std::nullopt;
}

} // namespace bsim