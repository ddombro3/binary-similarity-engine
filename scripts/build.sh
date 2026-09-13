#!/usr/bin/env bash

set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "${ROOT_DIR}"

PRESET="${1:-debug}"

echo "Configuring preset: ${PRESET}"
cmake --preset "${PRESET}"

echo "Building preset: ${PRESET}"
cmake --build --preset "${PRESET}"
