#include <bsim/formats/binary_format.hpp>

namespace bsim {

BinaryFormat detect_binary_format(std::span<const std::byte> bytes) noexcept {
    
    if (bytes.size() >= 4 &&
        bytes[0] == std::byte{0x7F} &&
        bytes[1] == std::byte{'E'} &&
        bytes[2] == std::byte{'L'} &&
        bytes[3] == std::byte{'F'}) {
        return BinaryFormat::elf;
    }

    if (bytes.size() >= 2 && bytes[0] == std::byte{'M'} && bytes[1] == std::byte{'Z'}) {

        return BinaryFormat::pe;
    }

    return BinaryFormat::unknown;
}

} // namespace bsim