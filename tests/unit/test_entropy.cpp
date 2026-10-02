#include <bsim/features/entropy.hpp>

#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iostream>

void test_empty_input() {
    const std::span<const std::byte> bytes{};

    const double entropy = bsim::calculate_entropy(bytes);

    assert(entropy == 0.0);
}

void test_single_byte_value() {
    constexpr std::array bytes{
        std::byte{0xAA},
        std::byte{0xAA},
        std::byte{0xAA},
        std::byte{0xAA}
    };

    const double entropy = bsim::calculate_entropy(bytes);

    assert(entropy == 0.0);
}

void test_uniform_distribution() {
    constexpr std::array bytes{
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03}
    };

    const double entropy = bsim::calculate_entropy(bytes);

    // Four equally likely values = 2 bits of entropy
    assert(std::abs(entropy - 2.0) < 0.001);
}

void test_random_like_data() {
    constexpr std::array bytes{
        std::byte{0x00},
        std::byte{0x11},
        std::byte{0x22},
        std::byte{0x33},
        std::byte{0x44},
        std::byte{0x55},
        std::byte{0x66},
        std::byte{0x77}
    };

    const double entropy = bsim::calculate_entropy(bytes);

    assert(entropy > 2.0);
}

int main() {
    test_empty_input();
    test_single_byte_value();
    test_uniform_distribution();
    test_random_like_data();

    std::cout << "Entropy tests passed\n";

    return 0;
}