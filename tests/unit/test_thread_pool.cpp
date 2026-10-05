#include <bsim/core/thread_pool.hpp>

#include <array>
#include <atomic>
#include <cassert>
#include <cstddef>
#include <future>
#include <iostream>
#include <stdexcept>

void test_worker_count() {
    bsim::ThreadPool pool{4};

    assert(pool.worker_count() == 4);
}

void test_submit_returns_value() {
    bsim::ThreadPool pool{2};

    auto future = pool.submit([] {
        return 42;
    });

    assert(future.get() == 42);
}

void test_multiple_tasks() {
    bsim::ThreadPool pool{4};

    std::array<std::future<int>, 8> futures;

    for (std::size_t i = 0; i < futures.size(); ++i) {
        futures[i] = pool.submit([i] {
            return static_cast<int>(i * i);
        });
    }

    for (std::size_t i = 0; i < futures.size(); ++i) {
        assert(futures[i].get() == static_cast<int>(i * i));
    }
}

void test_void_tasks() {
    bsim::ThreadPool pool{4};

    std::atomic<int> counter{0};
    std::array<std::future<void>, 8> futures;

    for (auto& future : futures) {
        future = pool.submit([&counter] {
            ++counter;
        });
    }

    for (auto& future : futures) {
        future.get();
    }

    assert(counter == 8);
}

void test_zero_workers_rejected() {
    bool threw = false;

    try {
        bsim::ThreadPool pool{0};
    } catch (const std::invalid_argument&) {
        threw = true;
    }

    assert(threw);
}

int main() {
    test_worker_count();
    test_submit_returns_value();
    test_multiple_tasks();
    test_void_tasks();
    test_zero_workers_rejected();

    std::cout << "ThreadPool tests passed\n";

    return 0;
}