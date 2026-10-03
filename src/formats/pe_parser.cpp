#include <bsim/formats/pe_parser.hpp>

#include <cstdint>

namespace bsim {

namespace {

constexpr std::size_t dos_lfanew_offset = 0x3C;
constexpr std::size_t pe_header_size = 24;
constexpr std::size_t section_header_size = 40;
constexpr std::size_t section_characteristics_offset = 36;
constexpr std::uint32_t image_scn_mem_execute = 0x20000000;

std::uint16_t read_u16_le(std::span<const std::byte> bytes, std::size_t offset) noexcept {
    return static_cast<std::uint16_t>(
        std::to_integer<std::uint8_t>(bytes[offset]) |
        (std::to_integer<std::uint8_t>(bytes[offset + 1]) << 8)
    );
}

std::uint32_t read_u32_le(std::span<const std::byte> bytes, std::size_t offset) noexcept {
    return static_cast<std::uint32_t>(
        std::to_integer<std::uint8_t>(bytes[offset]) |
        (std::to_integer<std::uint8_t>(bytes[offset + 1]) << 8) |
        (std::to_integer<std::uint8_t>(bytes[offset + 2]) << 16) |
        (std::to_integer<std::uint8_t>(bytes[offset + 3]) << 24)
    );
}

} // namespace

std::optional<PeStructure> parse_pe(std::span<const std::byte> bytes) noexcept {
    if (bytes.size() < dos_lfanew_offset + sizeof(std::uint32_t)) {
        return std::nullopt;
    }

    if (bytes[0] != std::byte{'M'} || bytes[1] != std::byte{'Z'}) {
        return std::nullopt;
    }

    const std::size_t pe_offset = read_u32_le(bytes, dos_lfanew_offset);

    if (pe_offset > bytes.size() || bytes.size() - pe_offset < pe_header_size) {
        return std::nullopt;
    }

    if (bytes[pe_offset] != std::byte{'P'} ||
        bytes[pe_offset + 1] != std::byte{'E'} ||
        bytes[pe_offset + 2] != std::byte{0x00} ||
        bytes[pe_offset + 3] != std::byte{0x00}) {
        return std::nullopt;
    }

    const std::size_t section_count = read_u16_le(bytes, pe_offset + 6);
    const std::size_t optional_header_size = read_u16_le(bytes, pe_offset + 20);

    std::size_t section_table_offset = pe_offset + pe_header_size;

    if (optional_header_size > bytes.size() - section_table_offset) {
        return std::nullopt;
    }

    section_table_offset += optional_header_size;

    if (section_count > (bytes.size() - section_table_offset) / section_header_size) {
        return std::nullopt;
    }

    PeStructure structure{};
    structure.section_count = section_count;

    for (std::size_t i = 0; i < section_count; ++i) {
        const std::size_t section_offset = section_table_offset + (i * section_header_size);
        const std::uint32_t characteristics = read_u32_le(bytes, section_offset + section_characteristics_offset);

        if ((characteristics & image_scn_mem_execute) != 0) {
            ++structure.executable_section_count;
        }
    }

    return structure;
}

} // namespace bsim