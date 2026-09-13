option(BSIM_BUILD_TESTS "Build unit and integration tests" OFF)
option(BSIM_BUILD_BENCHMARKS "Build performance benchmarks" OFF)
option(BSIM_BUILD_FUZZERS "Build fuzzing targets" OFF)

option(BSIM_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

option(BSIM_ENABLE_ASAN "Enable AddressSanitizer" OFF)
option(BSIM_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(BSIM_ENABLE_TSAN "Enable ThreadSanitizer" OFF)

option(BSIM_ENABLE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)
option(BSIM_ENABLE_IPO "Enable interprocedural optimization / LTO" OFF)
