#!/usr/bin/env bash
set -euo pipefail

# Resolve paths relative to this script, independently of the caller's directory.
project_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
build_dir="${project_dir}/build"

if ! command -v cmake >/dev/null 2>&1; then
    printf 'Error: cmake is required but was not found.\n' >&2
    exit 1
fi

cmake -S "${project_dir}" -B "${build_dir}" -DCMAKE_BUILD_TYPE=Debug
cmake --build "${build_dir}" --parallel

printf 'Executable: %s\n' "${build_dir}/hal_demo"
