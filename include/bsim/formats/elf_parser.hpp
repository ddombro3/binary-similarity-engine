#pragma once

#include <cstddef>
#include <optional>
#include <span>

namespace bsim {

struct ElfStructure {
    std::size_t section_count{0};
    std::size_t executable_section_count{0};
};

[[nodiscard]] std::optional<ElfStructure> parse_elf(std::span<const std::byte> bytes) noexcept;

} // namespace bsim