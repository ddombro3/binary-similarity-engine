#include <bsim/io/mapped_file.hpp>

#include <cerrno>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <utility>

namespace bsim {

MappedFile::MappedFile(int fd, std::byte* data, std::size_t size) noexcept
    : fd_{fd},
      data_{data},
      size_{size} {
}

std::expected<MappedFile, std::error_code>
MappedFile::open(const std::filesystem::path& path) {
    const int fd = ::open(path.c_str(), O_RDONLY);

    if (fd == -1) {
        return std::unexpected(
            std::error_code{errno, std::generic_category()}
        );
    }

    struct stat file_stat {};

    if (::fstat(fd, &file_stat) == -1) {
        const std::error_code error{errno, std::generic_category()};
        ::close(fd);
        return std::unexpected(error);
    }

    const auto size = static_cast<std::size_t>(file_stat.st_size);

    if (size == 0) {
        return MappedFile{fd, nullptr, 0};
    }

    void* const mapping =
        ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0); //nullptr bc linux chooses address for me

    if (mapping == MAP_FAILED) {
        const std::error_code error{errno, std::generic_category()};
        ::close(fd);
        return std::unexpected(error);
    }

    return MappedFile{
        fd,
        static_cast<std::byte*>(mapping),
        size
    };
}

MappedFile::~MappedFile() {
    if (data_ != nullptr) {
        ::munmap(data_, size_);
    }

    if (fd_ != -1) {
        ::close(fd_);
    }
}

MappedFile::MappedFile(MappedFile&& other) noexcept
    : fd_{std::exchange(other.fd_, -1)},
      data_{std::exchange(other.data_, nullptr)},
      size_{std::exchange(other.size_, 0)} {
}

MappedFile& MappedFile::operator=(MappedFile&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (data_ != nullptr) {
        ::munmap(data_, size_);
    }

    if (fd_ != -1) {
        ::close(fd_);
    }

    fd_ = std::exchange(other.fd_, -1);
    data_ = std::exchange(other.data_, nullptr);
    size_ = std::exchange(other.size_, 0);

    return *this;
}

std::span<const std::byte> MappedFile::bytes() const noexcept {
    return {data_, size_};
}

std::size_t MappedFile::size() const noexcept {
    return size_;
}

bool MappedFile::empty() const noexcept {
    return size_ == 0;
}

} // namespace bsim