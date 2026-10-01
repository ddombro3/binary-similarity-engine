#include <bsim/features/entropy.hpp>

#include <array>
#include <cmath>
#include <cstddef>

namespace bsim {

double calculate_entropy(
    std::span<const std::byte> bytes
) noexcept {
    if (bytes.empty()) {
        return 0.0;
    }

    std::array<std::size_t, 256> frequencies{};

    for (const auto byte : bytes) {
        const auto index = static_cast<std::size_t>(byte);
        ++frequencies[index];
    }

    const double size = static_cast<double>(bytes.size());

    double entropy = 0.0;

    for (const auto frequency : frequencies) {
        if (frequency == 0) {
            continue;
        }

        const double probability =
            static_cast<double>(frequency) / size;

        entropy -= probability * std::log2(probability);
    }

    return entropy;
}

} // namespace bsim