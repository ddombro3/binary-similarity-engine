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

void test_jaccard_identical() {
    const std::array<bsim::NGram, 3> lhs{1, 2, 3};
    const std::array<bsim::NGram, 3> rhs{1, 2, 3};

    const double similarity = bsim::jaccard_similarity(lhs, rhs);

    assert(std::abs(similarity - 1.0) < 0.001);
}

void test_jaccard_no_overlap() {
    const std::array<bsim::NGram, 2> lhs{1, 2};
    const std::array<bsim::NGram, 2> rhs{3, 4};

    const double similarity = bsim::jaccard_similarity(lhs, rhs);

    assert(similarity == 0.0);
}

void test_jaccard_partial_overlap() {
    const std::array<bsim::NGram, 3> lhs{1, 2, 3};
    const std::array<bsim::NGram, 3> rhs{2, 3, 4};

    const double similarity = bsim::jaccard_similarity(lhs, rhs);

    assert(similarity > 0.0);
    assert(similarity < 1.0);
}

void test_jaccard_empty_input() {
    const std::span<const bsim::NGram> lhs{};
    const std::array<bsim::NGram, 2> rhs{1, 2};

    const double similarity = bsim::jaccard_similarity(lhs, rhs);

    assert(similarity == 0.0);
}

int main() {
    test_identical_histograms();
    test_completely_different_histograms();
    test_partial_similarity();
    test_zero_vector();
    test_jaccard_identical();
    test_jaccard_no_overlap();
    test_jaccard_partial_overlap();
    test_jaccard_empty_input();

    std::cout << "Similarity tests passed\n";

    return 0;
}