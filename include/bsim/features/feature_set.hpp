#pragma once

#include <bsim/features/ngram.hpp>

#include <array>
#include <cstddef>
#include <string>
#include <vector>

namespace bsim {

struct FeatureSet {             //data container
    std::size_t file_size{0};

    double entropy{0.0}; //shannon entropy of file

    std::array<double, 256> byte_histogram{};

    std::vector<std::string> strings{};
    std::vector<NGram> ngrams{};

    std::size_t section_count{0};
    std::size_t executable_section_count{0};


};

} // namespace bsim