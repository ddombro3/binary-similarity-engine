#pragma once

#include <cstddef>
#include <expected>
#include <filesystem>
#include <span>
#include <system_error>

namespace bsim {

class MappedFile {
public:
    static std::expected<MappedFile, std::error_code>
    open(const std::filesystem::path& path);

    ~MappedFile();

    MappedFile(const MappedFile&) = delete;
    MappedFile& operator=(const MappedFile&) = delete;

    MappedFile(MappedFile&& other) noexcept;
    MappedFile& operator=(MappedFile&& other) noexcept;

    [[nodiscard]] std::span<const std::byte> bytes() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

private:
    MappedFile(int fd, std::byte* data, std::size_t size) noexcept;

    int fd_{-1};
    std::byte* data_{nullptr};
    std::size_t size_{0};
};

} // namespace bsim