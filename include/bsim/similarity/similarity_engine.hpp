#pragma once

#include <array>

namespace bsim {

[[nodiscard]] double cosine_similarity( const std::array<double, 256>& lhs, const std::array<double, 256>& rhs ) noexcept;

} // namespace bsim