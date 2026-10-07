#pragma once

#include <bsim/core/binary_image.hpp>

#include <cstddef>
#include <expected>
#include <filesystem>
#include <system_error>
#include <vector>

namespace bsim {

[[nodiscard]] std::vector<std::expected<BinaryImage, std::error_code>> analyze_batch(const std::vector<std::filesystem::path>& paths, std::size_t worker_count);

} // namespace bsim