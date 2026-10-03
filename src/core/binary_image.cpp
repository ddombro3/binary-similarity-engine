#include <bsim/core/binary_image.hpp>

#include <bsim/features/byte_histogram.hpp>
#include <bsim/features/entropy.hpp>
#include <bsim/features/strings.hpp>
#include <bsim/formats/elf_parser.hpp>
#include <bsim/formats/pe_parser.hpp>
#include <bsim/io/mapped_file.hpp>

namespace bsim {

std::expected<BinaryImage, std::error_code> analyze_binary(const std::filesystem::path& path) {
    auto mapped_result = MappedFile::open(path);

    if (!mapped_result) {
        return std::unexpected(mapped_result.error());
    }

    const auto& mapped_file = *mapped_result;
    const auto bytes = mapped_file.bytes();

    BinaryImage image{};
    image.path = path;
    image.format = detect_binary_format(bytes);

    image.features.file_size = mapped_file.size();
    image.features.entropy = calculate_entropy(bytes);
    image.features.byte_histogram = calculate_byte_histogram(bytes);
    image.features.strings = extract_strings(bytes);

    if (image.format == BinaryFormat::elf) {
        const auto structure = parse_elf(bytes);

        if (structure) {
            image.features.section_count = structure->section_count;
            image.features.executable_section_count = structure->executable_section_count;
        }
    }

    if (image.format == BinaryFormat::pe) {
        const auto structure = parse_pe(bytes);

        if (structure) {
            image.features.section_count = structure->section_count;
            image.features.executable_section_count = structure->executable_section_count;
        }
    }

    return image;
}

} // namespace bsim