#include <bsim/core/batch_analyzer.hpp>

#include <cassert>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

class TempDirectory {
public:
    TempDirectory() {
        const auto id = std::chrono::steady_clock::now().time_since_epoch().count();
        path_ = std::filesystem::temp_directory_path() / ("bsim_batch_test_" + std::to_string(id));
        std::filesystem::create_directories(path_);
    }

    ~TempDirectory() {
        std::error_code error;
        std::filesystem::remove_all(path_, error);
    }

    [[nodiscard]] const std::filesystem::path& path() const noexcept {
        return path_;
    }

private:
    std::filesystem::path path_;
};

void write_file(const std::filesystem::path& path, const std::string& data) {
    std::ofstream file{path, std::ios::binary};

    assert(file.is_open());

    file.write(data.data(), static_cast<std::streamsize>(data.size()));

    assert(file.good());
}

void test_batch_analysis() {
    const TempDirectory directory;

    const auto first_path = directory.path() / "first.bin";
    const auto second_path = directory.path() / "second.bin";
    const auto missing_path = directory.path() / "missing.bin";

    write_file(first_path, "AAAA");
    write_file(second_path, "BBBBBBBB");

    const std::vector<std::filesystem::path> paths{
        first_path,
        second_path,
        missing_path
    };

    const auto results = bsim::analyze_batch(paths, 2);

    assert(results.size() == 3);

    assert(results[0].has_value());
    assert(results[0]->path == first_path);
    assert(results[0]->features.file_size == 4);

    assert(results[1].has_value());
    assert(results[1]->path == second_path);
    assert(results[1]->features.file_size == 8);

    assert(!results[2].has_value());
}

} // namespace

int main() {
    test_batch_analysis();

    std::cout << "Batch analysis integration test passed\n";

    return 0;
}