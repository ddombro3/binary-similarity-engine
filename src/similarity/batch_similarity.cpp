#include <bsim/similarity/batch_similarity.hpp>

#include <bsim/core/thread_pool.hpp>
#include <bsim/similarity/similarity_engine.hpp>

#include <future>

namespace bsim {

std::vector<PairwiseSimilarity> compare_batch(const std::vector<BinaryImage>& images, std::size_t worker_count) {
    ThreadPool pool{worker_count};

    const std::size_t pair_count = (images.size() * (images.size() - 1)) / 2;

    std::vector<std::future<PairwiseSimilarity>> futures;
    futures.reserve(pair_count);

    for (auto i{0uz}; i < images.size(); ++i) {
        for (std::size_t j{i + 1}; j < images.size(); ++j) {
            futures.push_back(pool.submit([&images, i, j] {
                return PairwiseSimilarity{
                    i,
                    j,
                    compare_binary_images(images[i], images[j])
                };
            }));
        }
    }

    std::vector<PairwiseSimilarity> results;
    results.reserve(pair_count);

    for (auto& future : futures) {
        results.push_back(future.get());
    }

    return results;
}

} // namespace bsim