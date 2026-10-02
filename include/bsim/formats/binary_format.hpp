#pragma once

#include <cstddef>
#include <span>

namespace bsim {

enum class BinaryFormat {
    unknown,
    pe,
    elf
};

[[nodiscard]] BinaryFormat detect_binary_format(std::span<const std::byte> bytes) noexcept;

} // namespace bsim