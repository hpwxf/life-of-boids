#!/usr/bin/env bash
set -eo pipefail

if [[ "$DEBUG_CI" == "true" ]]; then
    echo "------------------------------------"

    if ( command -v c++ >/dev/null 2>&1 ); then
      echo "C++ config: $(command -v c++)"
      c++ --version 2>&1 | sed 's/^/  /'
    else
      echo "No C++ command found"
    fi

    echo "------------------------------------"

    if ( command -v clang-format >/dev/null 2>&1 ); then
      echo "clang-format config: $(command -v clang-format)"
      clang-format --version 2>&1 | sed 's/^/  /'
    else
      echo "No clang-format command found"
    fi

    echo "------------------------------------"

    if ( command -v cmake >/dev/null 2>&1 ); then
      echo "CMake config: $(command -v cmake)"
      cmake --version 2>&1 | sed 's/^/  /'
    else
      echo "No cmake command found"
    fi
          
    echo "------------------------------------"

    if ( command -v make >/dev/null 2>&1 ); then
      echo "Make config: $(command -v make)"
      make --version 2>&1 | sed 's/^/  /'
    else
      echo "No make command found"
    fi

    echo "------------------------------------"

    if ( command -v conan >/dev/null 2>&1 ); then
      echo "conan config: $(command -v conan)"
      conan --version 2>&1 | sed 's/^/  /'
    else
      echo "No conan command found"
    fi

    echo "------------------------------------"

    echo "EXTRA_CMAKE_OPTIONS = ${EXTRA_CMAKE_OPTIONS}"

    echo "------------------------------------"

fi
