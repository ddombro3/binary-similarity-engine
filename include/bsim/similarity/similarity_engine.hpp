#pragma once

#include <bsim/features/ngram.hpp>
#include <bsim/core/binary_image.hpp>
#include <bsim/similarity/similarity_result.hpp>

#include <array>
#include <span>

namespace bsim {

[[nodiscard]] double cosine_similarity( const std::array<double, 256>& lhs, const std::array<double, 256>& rhs ) noexcept;

[[nodiscard]] double jaccard_similarity(std::span<const NGram> lhs, std::span<const NGram> rhs);

[[nodiscard]] SimilarityResult compare_binary_images(const BinaryImage& lhs, const BinaryImage& rhs);

} // namespace bsim