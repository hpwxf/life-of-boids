#!/usr/bin/env bash
set -eo pipefail

if [[ "$DEBUG_CI" == "true" ]]; then
  CTEST_FLAGS=--verbose
  set -x
else
  CTEST_FLAGS=--output-on-failure
fi

cd build

if [[ "$ENABLE_COVERAGE" == "on" ]]; then
    cmake --build . --target coverage --config "${MODE}"
elif [[ "$ENABLE_MEMCHECK" == "on" ]]; then
    ctest -C "${MODE}" ${CTEST_FLAGS} -T memcheck
else
    ctest -C "${MODE}" ${CTEST_FLAGS}
fi
