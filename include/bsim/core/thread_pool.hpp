#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

namespace bsim {

class ThreadPool {
public:
    explicit ThreadPool(std::size_t worker_count);
    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

    template <typename Function>
    [[nodiscard]] auto submit(Function&& function) -> std::future<std::invoke_result_t<Function>>;

    [[nodiscard]] std::size_t worker_count() const noexcept;

private:
    void worker_loop();

    std::vector<std::jthread> workers_;
    std::queue<std::function<void()>> tasks_;

    std::mutex mutex_;
    std::condition_variable condition_;

    bool stopping_{false};
};

template <typename Function>
auto ThreadPool::submit(Function&& function) -> std::future<std::invoke_result_t<Function>> {
    using Result = std::invoke_result_t<Function>;

    auto task = std::make_shared<std::packaged_task<Result()>>(std::forward<Function>(function));
    auto future = task->get_future();

    {
        std::lock_guard lock{mutex_};

        if (stopping_) {
            throw std::runtime_error{"cannot submit task to stopped ThreadPool"};
        }

        tasks_.emplace([task] {
            (*task)();
        });
    }

    condition_.notify_one();

    return future;
}

} // namespace bsim // hyperthreading benchmark? 