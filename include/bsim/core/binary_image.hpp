#pragma once

#include <bsim/features/feature_set.hpp>
#include <bsim/formats/binary_format.hpp>

#include <expected>
#include <filesystem>
#include <system_error>

namespace bsim {

struct BinaryImage {
    std::filesystem::path path{};
    BinaryFormat format{BinaryFormat::unknown};
    FeatureSet features{};
};

[[nodiscard]] std::expected<BinaryImage, std::error_code> analyze_binary(const std::filesystem::path& path);

} // namespace bsim