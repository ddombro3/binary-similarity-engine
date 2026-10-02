#pragma once

#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace bsim {

[[nodiscard]] std::vector<std::string> extract_strings(std::span<const std::byte> bytes, std::size_t min_length = 4);

} // namespace bsim