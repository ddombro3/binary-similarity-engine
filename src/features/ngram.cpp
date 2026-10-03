#include <bsim/features/ngram.hpp>

namespace bsim {

std::vector<NGram> extract_ngrams(std::span<const std::byte> bytes, std::size_t n) {
    std::vector<NGram> ngrams;

    if (n == 0 || n > sizeof(NGram) || bytes.size() < n) {
        return ngrams;
    }

    ngrams.reserve(bytes.size() - n + 1);

    for (auto i{0uz}; i <= bytes.size() - n; ++i) {
        NGram value = 0;

        for (auto j{0uz}; j < n; ++j) {
            value <<= 8;
            value |= std::to_integer<std::uint8_t>(bytes[i + j]);
        }

        ngrams.push_back(value);
    }

    return ngrams;
}

} // namespace bsim