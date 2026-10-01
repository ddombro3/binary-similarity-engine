#pragma once

#include <cstddef>
#include <span>

namespace bsim {

[[nodiscard]] double calculate_entropy(   //shannon entropy algo declaration
    std::span<const std::byte> bytes
) noexcept;

} // namespace bsim