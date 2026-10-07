#include <bsim/similarity/batch_similarity.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

void test_batch_similarity() {
    bsim::BinaryImage first{};
    bsim::BinaryImage second{};
    bsim::BinaryImage third{};

    first.features.byte_histogram[0] = 1.0;
    first.features.ngrams = {1, 2, 3};

    second.features.byte_histogram[0] = 1.0;
    second.features.ngrams = {1, 2, 3};

    third.features.byte_histogram[1] = 1.0;
    third.features.ngrams = {4, 5, 6};

    const std::vector<bsim::BinaryImage> images{
        first,
        second,
        third
    };

    const auto results = bsim::compare_batch(images, 2);

    assert(results.size() == 3);

    assert(results[0].lhs_index == 0);
    assert(results[0].rhs_index == 1);
    assert(std::abs(results[0].similarity.combined_score - 1.0) < 0.001);

    assert(results[1].lhs_index == 0);
    assert(results[1].rhs_index == 2);
    assert(results[1].similarity.combined_score == 0.0);

    assert(results[2].lhs_index == 1);
    assert(results[2].rhs_index == 2);
    assert(results[2].similarity.combined_score == 0.0);
}

void test_single_image() {
    const std::vector<bsim::BinaryImage> images(1);

    const auto results = bsim::compare_batch(images, 2);

    assert(results.empty());
}

int main() {
    test_batch_similarity();
    test_single_image();

    std::cout << "Batch similarity integration tests passed\n";

    return 0;
}