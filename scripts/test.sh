#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT_DIR}"

cmake --preset debug-tests
cmake --build --preset debug-tests

ctest \
    --test-dir build/debug-tests \
    --output-on-failure
