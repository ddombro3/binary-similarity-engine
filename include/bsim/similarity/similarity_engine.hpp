#pragma once

#include <array>
#include <span>

namespace bsim {

[[nodiscard]] double cosine_similarity( const std::array<double, 256>& lhs, const std::array<double, 256>& rhs ) noexcept;

[[nodiscard]] double jaccard_similarit( std::span<const NGram> lhs, std::span<const NGram> rhs);

} // namespace bsim