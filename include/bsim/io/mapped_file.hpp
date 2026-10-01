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
    open(const std::filesystem::path& path); //open our main function for actually getting the file

    ~MappedFile(); //destructor

    MappedFile(const MappedFile&) = delete; //copy constructor big no
    MappedFile& operator=(const MappedFile&) = delete; //copy assignment op

    MappedFile(MappedFile&& other) noexcept; //move con
    MappedFile& operator=(MappedFile&& other) noexcept; //enable move op

    [[nodiscard]] std::span<const std::byte> bytes() const noexcept; //our functions, nodiscard dont ignore return val, const dont modify obj, noexcept func wont throw expection
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

private:
    MappedFile(int fd, std::byte* data, std::size_t size) noexcept; //constrcutor

    int fd_{-1};                // Open file descriptor; -1 means no valid file is open, fd is int OS gives file to label it
    std::byte* data_{nullptr};  // Pointer to the first byte of the memory-mapped file
    std::size_t size_{0};       // Size of the mapped file/region in byte
};

} // namespace bsim