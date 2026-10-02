#include <bsim/features/byte_histogram.hpp>

#include <array>
#include <cstddef>

namespace bsim {

std::array<double, 256> calculate_byte_histogram(std::span<const std::byte> bytes) noexcept {
    
    std::array<double, 256> histogram{};

    if (bytes.empty()) {
        return histogram;
    }

    for (const auto byte : bytes) {
        const auto index = static_cast<std::size_t>(byte);
        histogram[index] += 1.0;
    }

    const double size = static_cast<double>(bytes.size());

    for (auto& frequency : histogram) {
        frequency /= size;
    }

    return histogram;
}

} // namespace bsim