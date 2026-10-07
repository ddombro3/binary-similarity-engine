#pragma once

namespace bsim {

struct SimilarityResult {
    double cosine_score{0.0};
    double jaccard_score{0.0};
    double combined_score{0.0};
};

} // namespace bsim