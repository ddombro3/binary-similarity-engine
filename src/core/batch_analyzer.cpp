#include <bsim/core/batch_analyzer.hpp>

#include <bsim/core/thread_pool.hpp>

#include <future>
#include <utility>

namespace bsim {

std::vector<std::expected<BinaryImage, std::error_code>> analyze_batch(
    const std::vector<std::filesystem::path>& paths,
    std::size_t worker_count
) {
    ThreadPool pool{worker_count};

    std::vector<std::future<std::expected<BinaryImage, std::error_code>>> futures;
    futures.reserve(paths.size());

    for (const auto& path : paths) {
        futures.push_back(pool.submit([path] {
            return analyze_binary(path);
        }));
    }

    std::vector<std::expected<BinaryImage, std::error_code>> results;
    results.reserve(paths.size());

    for (auto& future : futures) {
        results.push_back(future.get());
    }

    return results;
}

} // namespace bsim