#include <bsim/similarity/similarity_engine.hpp>

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>

void test_identical_histograms() {
    std::array<double, 256> lhs{};
    std::array<double, 256> rhs{};

    lhs[0] = 0.5;
    lhs[1] = 0.5;

    rhs[0] = 0.5;
    rhs[1] = 0.5;

    const double similarity = bsim::cosine_similarity(lhs, rhs);

    assert(std::abs(similarity - 1.0) < 0.001);
}

void test_completely_different_histograms() {
    std::array<double, 256> lhs{};
    std::array<double, 256> rhs{};

    lhs[0] = 1.0;
    rhs[1] = 1.0;

    const double similarity = bsim::cosine_similarity(lhs, rhs);

    assert(std::abs(similarity) < 0.001);
}

void test_partial_similarity() {
    std::array<double, 256> lhs{};
    std::array<double, 256> rhs{};

    lhs[0] = 0.5;
    lhs[1] = 0.5;

    rhs[0] = 0.5;
    rhs[2] = 0.5;

    const double similarity = bsim::cosine_similarity(lhs, rhs);

    assert(similarity > 0.0);
    assert(similarity < 1.0);
}

void test_zero_vector() {
    std::array<double, 256> lhs{};
    std::array<double, 256> rhs{};

    rhs[0] = 1.0;

    const double similarity = bsim::cosine_similarity(lhs, rhs);

    assert(similarity == 0.0);
}

int main() {
    test_identical_histograms();
    test_completely_different_histograms();
    test_partial_similarity();
    test_zero_vector();

    std::cout << "Similarity tests passed\n";

    return 0;
}