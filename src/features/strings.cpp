#include <bsim/features/strings.hpp>

#include <cctype>

namespace bsim {

std::vector<std::string> extract_strings(std::span<const std::byte> bytes, std::size_t min_length) {
    
    std::vector<std::string> strings;
    std::string current;

    for (const auto byte : bytes) {
        const auto value = static_cast<unsigned char>(byte);

        if (std::isprint(value)) {
            current.push_back(static_cast<char>(value));
        } else {
            if (current.size() >= min_length) {
                strings.push_back(current);
            }

            current.clear();
        }
    }

    if (current.size() >= min_length) {
        strings.push_back(current);
    }

    return strings;
}

} // namespace bsim