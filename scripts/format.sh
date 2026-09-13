#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT_DIR}"

FORMATTER=""

if command -v clang-format-23 >/dev/null 2>&1; then
    FORMATTER="clang-format-23"
elif command -v clang-format >/dev/null 2>&1; then
    FORMATTER="clang-format"
else
    echo "clang-format was not found."
    exit 1
fi

find \
    apps \
    include \
    src \
    tests \
    benchmarks \
    fuzz \
    -type f \
    \( -name '*.cpp' -o -name '*.hpp' -o -name '*.h' \) \
    -print0 |
    xargs -0 "${FORMATTER}" -i

echo "Formatting complete."
