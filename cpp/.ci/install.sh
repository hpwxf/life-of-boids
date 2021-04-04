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

if (! command -v conan >/dev/null 2>&1 ); then
  # install conan if not already installed
  sudo pip3 install wheel setuptools
  sudo pip3 install conan
  # sudo ln -s ~/.local/bin/conan /usr/local/bin/conan
fi
