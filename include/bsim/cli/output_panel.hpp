#pragma once

#include <bsim/core/binary_image.hpp>
#include <bsim/similarity/batch_similarity.hpp>
#include <bsim/similarity/similarity_result.hpp>

#include <vector>

namespace bsim::cli {

void print_analysis_panel(const BinaryImage& image);
void print_similarity_panel(const SimilarityResult& result);
void print_corpus_panel(const std::vector<PairwiseSimilarity>& comparisons, const std::vector<BinaryImage>& images);
void print_metrics_help();

} // namespace bsim::cli