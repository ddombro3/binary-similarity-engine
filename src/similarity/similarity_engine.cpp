#include <bsim/similarity/similarity_engine.hpp>

#include <cmath>
#include <unordered_set>


namespace bsim {

double cosine_similarity(const std::array<double, 256>& lhs, const std::array<double, 256>& rhs) noexcept {
    double dot_product = 0.0;
    double lhs_magnitude = 0.0;
    double rhs_magnitude = 0.0;

    for (auto i{0uz}; i < lhs.size(); ++i) {
        dot_product += lhs[i] * rhs[i];
        lhs_magnitude += lhs[i] * lhs[i];
        rhs_magnitude += rhs[i] * rhs[i];
    }

    if (lhs_magnitude == 0.0 || rhs_magnitude == 0.0) {
        return 0.0;
    }

    return dot_product / (std::sqrt(lhs_magnitude) * std::sqrt(rhs_magnitude));
}

double jaccard_similarity(std::span<const NGram> lhs, std::span<const NGram> rhs) {
    if (lhs.empty() || rhs.empty()) {
        return 0.0;
    }

    const std::unordered_set<NGram> lhs_set{lhs.begin(), lhs.end()};
    const std::unordered_set<NGram> rhs_set{rhs.begin(), rhs.end()};

    std::size_t intersection_count = 0;

    for (const auto ngram : lhs_set) {
        if (rhs_set.contains(ngram)) {
            ++intersection_count;
        }
    }

    const std::size_t union_count = lhs_set.size() + rhs_set.size() - intersection_count;

    return static_cast<double>(intersection_count) / static_cast<double>(union_count);
}

SimilarityResult compare_binary_images(const BinaryImage& lhs, const BinaryImage& rhs) {
    SimilarityResult result{};

    result.cosine_score = cosine_similarity(
        lhs.features.byte_histogram,
        rhs.features.byte_histogram
    );

    result.jaccard_score = jaccard_similarity(
        lhs.features.ngrams,
        rhs.features.ngrams
    );

    result.combined_score = (result.cosine_score + result.jaccard_score) / 2.0;

    return result;
}

} // namespace bsim