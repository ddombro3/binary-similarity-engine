#include <bsim/cli/cli.hpp>

#include <bsim/core/batch_analyzer.hpp>
#include <bsim/core/binary_image.hpp>
#include <bsim/similarity/batch_similarity.hpp>
#include <bsim/similarity/similarity_engine.hpp>

#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
#include <vector>

namespace bsim::cli {

namespace {

#ifndef BSIM_VERSION
#define BSIM_VERSION "development"
#endif

constexpr std::size_t corpus_warning_threshold = 100;

const char* format_name(BinaryFormat format) {
    switch (format) {
        case BinaryFormat::pe:
            return "PE";
        case BinaryFormat::elf:
            return "ELF";
        default:
            return "Unknown";
    }
}

std::size_t default_worker_count() {
    const auto count = std::thread::hardware_concurrency();
    return count == 0 ? 1 : static_cast<std::size_t>(count);
}

void print_banner() {
    constexpr const char* reset = "\033[0m";
    constexpr const char* cyan = "\033[38;5;45m";
    constexpr const char* blue = "\033[38;5;39m";
    constexpr const char* magenta = "\033[38;5;201m";
    constexpr const char* yellow = "\033[38;5;220m";
    constexpr const char* orange = "\033[38;5;208m";
    constexpr const char* gray = "\033[38;5;245m";
    constexpr const char* white = "\033[38;5;255m";

    std::cout << '\n';

    std::cout << cyan    << "                         .-================================-.\n";
    std::cout << blue    << "                    .-==/====================================\\==-.\n";
    std::cout << magenta << "                 .-'   /__|__|__|__|__|__|__|__|__|__|__|__\\   '-.\n";
    std::cout << yellow  << "              .-'    /_|__|__|__|__|__|__|__|__|__|__|__|__|_\\    '-.\n";
    std::cout << orange  << "            .'      /__|__|__|__|__|__|__|__|__|__|__|__|__|__\\      '.\n";
    std::cout << cyan    << "           /       /_|__|__|__|__|__|__|__|__|__|__|__|__|__|_\\       \\\n";
    std::cout << blue    << "          /       /__|__|__|__|__|__|__|__|__|__|__|__|__|__|__\\       \\\n";
    std::cout << magenta << "         |       /_|__|__|__|__|__|__|__|__|__|__|__|__|__|__|_\\       |\n";

    std::cout << yellow  << "         |      |                                                        |\n";
    std::cout << cyan    << "         |      |   BBBB    SSSS   IIII   M   M                         |\n";
    std::cout << blue    << "         |      |   B   B  S        II    MM MM                         |\n";
    std::cout << magenta << "         |      |   BBBB    SSS     II    M M M                         |\n";
    std::cout << yellow  << "         |      |   B   B      S    II    M   M                         |\n";
    std::cout << orange  << "         |      |   BBBB   SSSS    IIII   M   M                         |\n";
    std::cout << cyan    << "         |      |                                                        |\n";

    std::cout << blue    << "         |       \\__|__|__|__|__|__|__|__|__|__|__|__|__|__|__/       |\n";
    std::cout << magenta << "          \\       \\_|__|__|__|__|__|__|__|__|__|__|__|__|__|_/       /\n";
    std::cout << yellow  << "           \\       \\__|__|__|__|__|__|__|__|__|__|__|__|__|__/       /\n";
    std::cout << orange  << "            '.      \\_|__|__|__|__|__|__|__|__|__|__|__|__|_/      .'\n";
    std::cout << cyan    << "              '-.    \\__|__|__|__|__|__|__|__|__|__|__|__/    .-'\n";
    std::cout << blue    << "                 '-.   \\_|__|__|__|__|__|__|__|__|__|_/   .-'\n";
    std::cout << magenta << "                    '-==\\================================/==-'\n";
    std::cout << yellow  << "                         '================================'\n";

    std::cout << '\n';
    std::cout << white << "                   BINARY SIMILARITY ENGINE\n" << reset;
    std::cout << gray << "                         C++23  |  ELF / PE\n" << reset;
    std::cout << '\n';

    std::cout << cyan << "Version: " << reset << BSIM_VERSION
              << gray << "    |    " << reset
              << cyan << "Available workers: " << reset << default_worker_count()
              << "\n\n";
}

void print_help() {
    std::cout << "Usage:\n";
    std::cout << "  bsim                         Interactive mode\n";
    std::cout << "  bsim -h                      Show help\n";
    std::cout << "  bsim --help                  Show help\n";
    std::cout << "  bsim --version               Show version\n";
    std::cout << "  bsim analyze <file>          Analyze one binary\n";
    std::cout << "  bsim compare <file1> <file2> Compare two binaries\n";
    std::cout << "  bsim corpus <directory>      Analyze and compare a corpus\n";
}

void print_analysis(const BinaryImage& image) {
    std::cout << '\n';
    std::cout << "Path: " << image.path << '\n';
    std::cout << "Format: " << format_name(image.format) << '\n';
    std::cout << "File size: " << image.features.file_size << " bytes\n";
    std::cout << "Entropy: " << image.features.entropy << '\n';
    std::cout << "Strings: " << image.features.strings.size() << '\n';
    std::cout << "Sections: " << image.features.section_count << '\n';
    std::cout << "Executable sections: " << image.features.executable_section_count << '\n';
    std::cout << "N-grams: " << image.features.ngrams.size() << '\n';
}

int analyze_command(const std::filesystem::path& path) {
    const auto result = analyze_binary(path);

    if (!result) {
        std::cerr << "Analysis failed: " << result.error().message() << '\n';
        return 1;
    }

    print_analysis(*result);
    return 0;
}

int compare_command(const std::filesystem::path& lhs_path, const std::filesystem::path& rhs_path) {
    const auto lhs = analyze_binary(lhs_path);
    const auto rhs = analyze_binary(rhs_path);

    if (!lhs) {
        std::cerr << "Failed to analyze " << lhs_path << ": " << lhs.error().message() << '\n';
        return 1;
    }

    if (!rhs) {
        std::cerr << "Failed to analyze " << rhs_path << ": " << rhs.error().message() << '\n';
        return 1;
    }

    const auto result = compare_binary_images(*lhs, *rhs);

    std::cout << '\n';
    std::cout << "Cosine similarity: " << result.cosine_score << '\n';
    std::cout << "Jaccard similarity: " << result.jaccard_score << '\n';
    std::cout << "Combined similarity: " << result.combined_score << '\n';

    return 0;
}

int corpus_command(const std::filesystem::path& directory) {
    if (!std::filesystem::is_directory(directory)) {
        std::cerr << "Not a directory: " << directory << '\n';
        return 1;
    }

    std::vector<std::filesystem::path> paths;

    for (const auto& entry : std::filesystem::directory_iterator{directory}) {
        if (entry.is_regular_file()) {
            paths.push_back(entry.path());
        }
    }

    std::sort(paths.begin(), paths.end());

    if (paths.empty()) {
        std::cerr << "No files found in directory\n";
        return 1;
    }

    if (paths.size() > corpus_warning_threshold) {
        const std::size_t pair_count = (paths.size() * (paths.size() - 1)) / 2;

        std::cout << '\n';
        std::cout << "Warning: corpus contains " << paths.size() << " files.\n";
        std::cout << "This requires " << pair_count << " pairwise comparisons.\n";
        std::cout << "Continue? [y/N]: ";

        std::string response;
        std::getline(std::cin, response);

        if (response != "y" && response != "Y") {
            std::cout << "Corpus analysis canceled.\n";
            return 0;
        }
    }

    const std::size_t workers = default_worker_count();

    auto analysis_results = analyze_batch(paths, workers);

    std::vector<BinaryImage> images;
    images.reserve(analysis_results.size());

    for (auto& result : analysis_results) {
        if (result) {
            images.push_back(std::move(*result));
        }
    }

    std::cout << '\n';
    std::cout << "Analyzed " << images.size() << " of " << paths.size() << " files\n";

    if (images.size() < 2) {
        return 0;
    }

    const auto comparisons = compare_batch(images, workers);

    std::cout << '\n';

    for (const auto& comparison : comparisons) {
        std::cout << images[comparison.lhs_index].path.filename()
                  << " <-> "
                  << images[comparison.rhs_index].path.filename()
                  << ": "
                  << comparison.similarity.combined_score
                  << '\n';
    }

    return 0;
}

std::string prompt_path(const std::string& prompt) {
    std::cout << prompt;

    std::string path;
    std::getline(std::cin, path);

    return path;
}

void interactive_menu() {
    while (true) {
        print_banner();

        std::cout << "[1] Analyze Binary\n";
        std::cout << "[2] Compare Binaries\n";
        std::cout << "[3] Analyze Corpus\n";
        std::cout << "[4] Help\n";
        std::cout << "[0] Exit\n";
        std::cout << '\n';
        std::cout << "Select an option: ";

        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            const auto path = prompt_path("Binary path: ");
            analyze_command(path);
        } else if (choice == "2") {
            const auto lhs = prompt_path("First binary: ");
            const auto rhs = prompt_path("Second binary: ");
            compare_command(lhs, rhs);
        } else if (choice == "3") {
            const auto directory = prompt_path("Corpus directory: ");
            corpus_command(directory);
        } else if (choice == "4") {
            std::cout << '\n';
            print_help();
        } else if (choice == "0") {
            std::cout << "Exiting BSIM.\n";
            return;
        } else {
            std::cout << "Invalid option.\n";
        }

        std::cout << '\n';
        std::cout << "Press Enter to continue...";
        std::cin.get();
    }
}

} // namespace

int run(int argc, char* argv[]) {
    if (argc == 1) {
        interactive_menu();
        return 0;
    }

    const std::string_view command{argv[1]};

    if (command == "-h" || command == "--help") {
        print_help();
        return 0;
    }

    if (command == "--version") {
        std::cout << "bsim " << BSIM_VERSION << '\n';
        return 0;
    }

    if (command == "analyze" && argc == 3) {
        return analyze_command(argv[2]);
    }

    if (command == "compare" && argc == 4) {
        return compare_command(argv[2], argv[3]);
    }

    if (command == "corpus" && argc == 3) {
        return corpus_command(argv[2]);
    }

    std::cerr << "Invalid command or arguments.\n\n";
    print_help();

    return 1;
}

} // namespace bsim::cli