#include <bsim/io/mapped_file.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <system_error>
#include <utility>

//ai unit tests

namespace {

class TempDirectory {
public:
    TempDirectory() {
        const auto id =
            std::chrono::steady_clock::now().time_since_epoch().count();

        path_ = std::filesystem::temp_directory_path()
              / ("bsim_mapped_file_tests_" + std::to_string(id));

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

void write_file(
    const std::filesystem::path& path,
    std::span<const std::byte> bytes
) {
    std::ofstream file{path, std::ios::binary};

    assert(file.is_open());

    file.write(
        reinterpret_cast<const char*>(bytes.data()),
        static_cast<std::streamsize>(bytes.size())
    );

    assert(file.good());
}

void test_open_regular_file(const std::filesystem::path& directory) {
    const auto path = directory / "regular.bin";

    constexpr std::array expected{
        std::byte{0x4D},
        std::byte{0x5A},
        std::byte{0x90},
        std::byte{0x00},
        std::byte{0xFF}
    };

    write_file(path, expected);

    auto result = bsim::MappedFile::open(path);

    assert(result.has_value());
    assert(result->size() == expected.size());
    assert(!result->empty());
    assert(std::ranges::equal(result->bytes(), expected));
}

void test_open_empty_file(const std::filesystem::path& directory) {
    const auto path = directory / "empty.bin";

    {
        std::ofstream file{path, std::ios::binary};
        assert(file.is_open());
    }

    auto result = bsim::MappedFile::open(path);

    assert(result.has_value());
    assert(result->size() == 0);
    assert(result->empty());
    assert(result->bytes().empty());
}

void test_missing_file(const std::filesystem::path& directory) {
    const auto path = directory / "does_not_exist.bin";

    auto result = bsim::MappedFile::open(path);

    assert(!result.has_value());

    assert(
        result.error()
        == std::make_error_code(std::errc::no_such_file_or_directory)
    );
}

void test_move_constructor(const std::filesystem::path& directory) {
    const auto path = directory / "move_constructor.bin";

    constexpr std::array expected{
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03}
    };

    write_file(path, expected);

    auto result = bsim::MappedFile::open(path);

    assert(result.has_value());

    bsim::MappedFile moved{std::move(*result)};

    assert(moved.size() == expected.size());
    assert(std::ranges::equal(moved.bytes(), expected));

    assert(result->empty());
    assert(result->size() == 0);
}

void test_move_assignment(const std::filesystem::path& directory) {
    const auto first_path = directory / "move_assignment_first.bin";
    const auto second_path = directory / "move_assignment_second.bin";

    constexpr std::array first_data{
        std::byte{0x10},
        std::byte{0x20}
    };

    constexpr std::array second_data{
        std::byte{0xAA},
        std::byte{0xBB},
        std::byte{0xCC},
        std::byte{0xDD}
    };

    write_file(first_path, first_data);
    write_file(second_path, second_data);

    auto first_result = bsim::MappedFile::open(first_path);
    auto second_result = bsim::MappedFile::open(second_path);

    assert(first_result.has_value());
    assert(second_result.has_value());

    bsim::MappedFile first{std::move(*first_result)};
    bsim::MappedFile second{std::move(*second_result)};

    first = std::move(second);

    assert(first.size() == second_data.size());
    assert(std::ranges::equal(first.bytes(), second_data));

    assert(second.empty());
    assert(second.size() == 0);
}

} // namespace

int main() {
    const TempDirectory temp_directory;

    test_open_regular_file(temp_directory.path());
    test_open_empty_file(temp_directory.path());
    test_missing_file(temp_directory.path());
    test_move_constructor(temp_directory.path());
    test_move_assignment(temp_directory.path());

    std::cout << "MappedFile tests passed\n";

    return 0;
}