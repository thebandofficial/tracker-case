#!/usr/bin/env bash
set -euo pipefail

repo="$(cd "$(dirname "$0")/.." && pwd)"
image="tracker-case-build"

docker build -q -t "$image" "$repo" > /dev/null
docker run --rm --user "$(id -u):$(id -g)" -v "$repo:/work" -w /work "$image" bash -c '
  cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug > /dev/null
  cmake --build build
  ./build/association_test
'
