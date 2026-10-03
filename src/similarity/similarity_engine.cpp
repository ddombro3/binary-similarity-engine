#include <bsim/similarity/similarity_engine.hpp>

#include <cmath>

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

} // namespace bsim