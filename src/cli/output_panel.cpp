#include <bsim/cli/output_panel.hpp>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>
#include <vector>

namespace bsim::cli {

namespace {

constexpr std::string_view reset = "\033[0m";
constexpr std::string_view cyan = "\033[38;5;45m";
constexpr std::string_view blue = "\033[38;5;39m";
constexpr std::string_view magenta = "\033[38;5;201m";
constexpr std::string_view yellow = "\033[38;5;220m";
constexpr std::string_view orange = "\033[38;5;208m";
constexpr std::string_view gray = "\033[38;5;245m";
constexpr std::string_view white = "\033[38;5;255m";

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

std::string_view similarity_label(double score) {
    if (score >= 0.90) {
        return "Very High";
    }

    if (score >= 0.70) {
        return "High";
    }

    if (score >= 0.50) {
        return "Moderate";
    }

    if (score >= 0.25) {
        return "Low";
    }

    return "Very Low";
}

std::string_view similarity_color(double score) {
    if (score >= 0.90) {
        return cyan;
    }

    if (score >= 0.70) {
        return blue;
    }

    if (score >= 0.50) {
        return yellow;
    }

    if (score >= 0.25) {
        return orange;
    }

    return magenta;
}

std::string_view entropy_label(double entropy) {
    if (entropy >= 7.5) {
        return "Very High";
    }

    if (entropy >= 6.0) {
        return "High";
    }

    if (entropy >= 3.0) {
        return "Moderate";
    }

    return "Low";
}

std::string_view entropy_color(double entropy) {
    if (entropy >= 7.5) {
        return magenta;
    }

    if (entropy >= 6.0) {
        return orange;
    }

    if (entropy >= 3.0) {
        return yellow;
    }

    return cyan;
}

void print_similarity_line(std::string_view name, double score) {
    std::cout << blue << std::left << std::setw(22) << name << reset;
    std::cout << white << std::fixed << std::setprecision(3) << score << reset;
    std::cout << "   ";
    std::cout << similarity_color(score) << similarity_label(score) << reset << '\n';
}

void print_similarity_interpretation(const SimilarityResult& result) {
    std::cout << '\n' << gray << "Interpretation: " << reset;

    if (result.cosine_score >= 0.75 && result.jaccard_score >= 0.75) {
        std::cout << "Strong agreement in both global byte distribution and local byte sequences.\n";
        return;
    }

    if (result.cosine_score >= 0.75 && result.jaccard_score < 0.40) {
        std::cout << "Similar global byte composition, but limited local byte sequence overlap.\n";
        return;
    }

    if (result.cosine_score < 0.40 && result.jaccard_score >= 0.75) {
        std::cout << "Strong local sequence overlap despite different global byte distributions.\n";
        return;
    }

    if (result.cosine_score < 0.40 && result.jaccard_score < 0.40) {
        std::cout << "Limited similarity according to both current comparison methods.\n";
        return;
    }

    std::cout << "Mixed or moderate similarity across the current comparison methods.\n";
}

} // namespace

void print_analysis_panel(const BinaryImage& image) {
    std::cout << '\n';
    std::cout << cyan << "==================== BINARY ANALYSIS ====================\n" << reset;

    std::cout << blue << "Path:                " << reset << image.path << '\n';
    std::cout << blue << "Format:              " << reset << format_name(image.format) << '\n';
    std::cout << blue << "File size:           " << reset << image.features.file_size << " bytes\n";
    std::cout << blue << "Strings extracted:   " << reset << image.features.strings.size() << '\n';
    std::cout << blue << "Sections:            " << reset << image.features.section_count << '\n';
    std::cout << blue << "Executable sections: " << reset << image.features.executable_section_count << '\n';
    std::cout << blue << "N-grams:             " << reset << image.features.ngrams.size() << '\n';

    std::cout << '\n';
    std::cout << yellow << "Shannon Entropy\n" << reset;
    std::cout << blue << "Score: " << reset;
    std::cout << white << std::fixed << std::setprecision(3) << image.features.entropy << " / 8.000" << reset;
    std::cout << "   ";
    std::cout << entropy_color(image.features.entropy) << entropy_label(image.features.entropy) << reset << '\n';

    std::cout << gray;
    std::cout << "Entropy measures how evenly distributed the file's byte values are.\n";
    std::cout << "Higher values indicate more random looking data and may occur with\n";
    std::cout << "compression, encryption, or packing. Entropy alone does not indicate malware.\n";
    std::cout << reset;

    std::cout << '\n';
    std::cout << gray;
    std::cout << "Sections describe the executable's internal structure. Executable sections\n";
    std::cout << "contain code that may be executed by the CPU. These values are most useful\n";
    std::cout << "when comparing binaries or investigating unusual structural differences.\n";
    std::cout << reset;

    std::cout << cyan << "=========================================================\n" << reset;
}

void print_similarity_panel(const SimilarityResult& result) {
    std::cout << '\n';
    std::cout << cyan << "================== SIMILARITY ANALYSIS ==================\n" << reset;

    print_similarity_line("Cosine similarity:", result.cosine_score);
    print_similarity_line("Jaccard similarity:", result.jaccard_score);
    print_similarity_line("Combined similarity:", result.combined_score);

    std::cout << '\n';

    std::cout << gray;
    std::cout << "Cosine compares overall byte frequency distributions.\n";
    std::cout << "Jaccard compares local byte sequences using byte n-grams.\n";
    std::cout << "Combined similarity is the current average of both measurements.\n";
    std::cout << reset;

    print_similarity_interpretation(result);

    std::cout << '\n';
    std::cout << gray;
    std::cout << "Similarity scores describe feature overlap. They do not prove that two\n";
    std::cout << "files belong to the same malware family or share a common origin.\n";
    std::cout << reset;

    std::cout << cyan << "=========================================================\n" << reset;
}

void print_corpus_panel(const std::vector<PairwiseSimilarity>& comparisons, const std::vector<BinaryImage>& images) {
    std::vector<PairwiseSimilarity> ranked{comparisons};

    std::sort(ranked.begin(), ranked.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.similarity.combined_score > rhs.similarity.combined_score;
    });

    std::cout << '\n';
    std::cout << cyan << "=================== CORPUS RESULTS ======================\n" << reset;

    std::cout << gray;
    std::cout << "A corpus is a collection of binaries analyzed together. BSIM compares every\n";
    std::cout << "unique pair and ranks them by combined similarity. Higher scores indicate\n";
    std::cout << "greater similarity across byte distribution and local byte sequences.\n";
    std::cout << "These scores indicate feature similarity, not proof of common origin or malware.\n";
    std::cout << reset << '\n';

    if (ranked.empty()) {
        std::cout << gray << "No pairwise comparisons available.\n" << reset;
        std::cout << cyan << "=========================================================\n" << reset;
        return;
    }

    std::size_t rank = 1;

    for (const auto& comparison : ranked) {
        const auto score = comparison.similarity.combined_score;

        std::cout << white << std::setw(3) << rank << ". " << reset;
        std::cout << images[comparison.lhs_index].path.filename();
        std::cout << gray << " <-> " << reset;
        std::cout << images[comparison.rhs_index].path.filename();
        std::cout << "   ";
        std::cout << similarity_color(score) << std::fixed << std::setprecision(3) << score;
        std::cout << "  " << similarity_label(score) << reset << '\n';

        ++rank;
    }

    std::cout << cyan << "=========================================================\n" << reset;
}

void print_metrics_help() {
    std::cout << '\n';
    std::cout << cyan << "==================== METRIC GUIDE =======================\n" << reset;

    std::cout << yellow << "\nShannon Entropy\n" << reset;
    std::cout << gray;
    std::cout << "Measures byte value randomness on a scale of approximately 0 to 8.\n";
    std::cout << "Higher entropy can indicate compression, encryption, packing, or other\n";
    std::cout << "highly distributed byte data. It is not a malware verdict.\n";
    std::cout << reset;

    std::cout << yellow << "\nByte Histogram\n" << reset;
    std::cout << gray;
    std::cout << "A normalized distribution of all 256 possible byte values. It describes\n";
    std::cout << "the global byte composition of a binary regardless of absolute file size.\n";
    std::cout << reset;

    std::cout << yellow << "\nCosine Similarity\n" << reset;
    std::cout << gray;
    std::cout << "Compares two byte frequency vectors. Scores near 1 indicate very similar\n";
    std::cout << "global byte distributions. Scores near 0 indicate little alignment.\n";
    std::cout << reset;

    std::cout << yellow << "\nByte N-grams\n" << reset;
    std::cout << gray;
    std::cout << "Short sliding byte sequences that preserve some local ordering information.\n";
    std::cout << "BSIM currently uses four byte n-grams by default.\n";
    std::cout << reset;

    std::cout << yellow << "\nJaccard Similarity\n" << reset;
    std::cout << gray;
    std::cout << "Measures overlap between the unique n-gram sets of two binaries.\n";
    std::cout << "1 means identical sets. 0 means no shared n-grams.\n";
    std::cout << reset;

    std::cout << yellow << "\nSimilarity Bands\n" << reset;
    std::cout << magenta << "0.00 - 0.24  Very Low\n" << reset;
    std::cout << orange << "0.25 - 0.49  Low\n" << reset;
    std::cout << yellow << "0.50 - 0.69  Moderate\n" << reset;
    std::cout << blue << "0.70 - 0.89  High\n" << reset;
    std::cout << cyan << "0.90 - 1.00  Very High\n" << reset;

    std::cout << '\n';
    std::cout << gray;
    std::cout << "These bands are presentation aids, not validated malware family thresholds.\n";
    std::cout << reset;

    std::cout << cyan << "=========================================================\n" << reset;
}

} // namespace bsim::cli