#pragma once

#include <bsim/core/binary_image.hpp>
#include <bsim/similarity/similarity_result.hpp>

#include <cstddef>
#include <vector>

namespace bsim {

struct PairwiseSimilarity {
    std::size_t lhs_index{0};
    std::size_t rhs_index{0};
    SimilarityResult similarity{};
};

[[nodiscard]] std::vector<PairwiseSimilarity> compare_batch(const std::vector<BinaryImage>& images, std::size_t worker_count);

} // namespace bsim