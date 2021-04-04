#!/usr/bin/env bash
set -eo pipefail

if [[ "$DEBUG_CI" == "true" ]]; then
  set -x
fi

if [[ "$ENABLE_COVERAGE" == "on" ]]; then
    sudo gem install coveralls-lcov
fi

if [[ "$ENABLE_COVERAGE" == "on" ]] && [[ "$ENABLE_MEMCHECK" == "on" ]]; then
  echo "Mixing coverage mode and memcheck is not supported"
  exit 1
fi

# install conan
pip3 install wheel setuptools
pip3 install conan
sudo ln -s ~/.local/bin/conan /usr/local/bin/conan
