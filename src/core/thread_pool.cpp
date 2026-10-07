#include <bsim/core/thread_pool.hpp>

namespace bsim {

ThreadPool::ThreadPool(std::size_t worker_count) {
    if (worker_count == 0) {
        throw std::invalid_argument{"ThreadPool reuires at least one worker"};
    }

    workers_.reserve(worker_count);

    for (auto i{0uz}; i < worker_count; ++i) {
        workers_.emplace_back([this] {
            worker_loop();              // n = worker_count, this is essentially saying this->worker_loop() n times. hpp has worker_ of type jthread. emplace constructs directly in container
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard lock{mutex_};
        stopping_ = true;
    }

    condition_.notify_all();
}

std::size_t ThreadPool::worker_count() const noexcept {
    return workers_.size();
}

void ThreadPool::worker_loop() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock lock{mutex_};

            condition_.wait(lock, [this] {
                return stopping_ || !tasks_.empty();
            });

            if (stopping_ && tasks_.empty()) {
                return;
            }

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        task(); //unique_ptr unlocks after it leaves scope, execute the task, multiple can run in parallel
    }
}

} // namespace bsim