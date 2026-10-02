#pragma once

#include <array>
#include <cstddef>
#include <span>

namespace bsim {

[[nodiscard]] std::array<double, 256> calculate_byte_histogram(
    std::span<const std::byte> bytes
) noexcept;

} // namespace bsim