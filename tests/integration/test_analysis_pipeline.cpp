#include <bsim/core/binary_image.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstring>
#include <elf.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

int main() {
    constexpr std::size_t section_count = 2;

    std::vector<std::byte> bytes(
        sizeof(Elf64_Ehdr) + (section_count * sizeof(Elf64_Shdr))
    );

    Elf64_Ehdr header{};
    header.e_ident[EI_MAG0] = ELFMAG0;
    header.e_ident[EI_MAG1] = ELFMAG1;
    header.e_ident[EI_MAG2] = ELFMAG2;
    header.e_ident[EI_MAG3] = ELFMAG3;
    header.e_ident[EI_CLASS] = ELFCLASS64;
    header.e_ident[EI_DATA] = ELFDATA2LSB;
    header.e_shoff = sizeof(Elf64_Ehdr);
    header.e_shentsize = sizeof(Elf64_Shdr);
    header.e_shnum = section_count;

    std::memcpy(bytes.data(), &header, sizeof(header));

    Elf64_Shdr executable_section{};
    executable_section.sh_flags = SHF_EXECINSTR;

    std::memcpy(
        bytes.data() + sizeof(Elf64_Ehdr) + sizeof(Elf64_Shdr),
        &executable_section,
        sizeof(executable_section)
    );

    constexpr std::array string_bytes{
        std::byte{'H'},
        std::byte{'E'},
        std::byte{'L'},
        std::byte{'L'},
        std::byte{'O'},
        std::byte{0x00}
    };

    bytes.insert(bytes.end(), string_bytes.begin(), string_bytes.end());

    const auto id = std::chrono::steady_clock::now().time_since_epoch().count();
    const auto path = std::filesystem::temp_directory_path() / ("bsim_pipeline_" + std::to_string(id) + ".elf");

    {
        std::ofstream file{path, std::ios::binary};

        assert(file.is_open());

        file.write(
            reinterpret_cast<const char*>(bytes.data()),
            static_cast<std::streamsize>(bytes.size())
        );

        assert(file.good());
    }

    const auto result = bsim::analyze_binary(path);

    assert(result.has_value());

    const auto& image = *result;

    assert(image.path == path);
    assert(image.format == bsim::BinaryFormat::elf);
    assert(image.features.file_size == bytes.size());
    assert(image.features.entropy > 0.0);
    assert(image.features.section_count == 2);
    assert(image.features.executable_section_count == 1);

    const auto string_it = std::find(
        image.features.strings.begin(),
        image.features.strings.end(),
        "HELLO"
    );

    assert(string_it != image.features.strings.end());

    double histogram_sum = 0.0;

    for (const auto frequency : image.features.byte_histogram) {
        histogram_sum += frequency;
    }

    assert(histogram_sum > 0.999 && histogram_sum < 1.001);

    std::filesystem::remove(path);

    std::cout << "Analysis pipeline integration test passed\n";

    return 0;
}