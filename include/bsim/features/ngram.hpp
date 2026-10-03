#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace bsim {

using NGram = std::uint64_t; //for readability

[[nodiscard]] std::vector<NGram> extract_ngrams(std::span<const std::byte> bytes, std::size_t n = 4);

} // namespace bsim