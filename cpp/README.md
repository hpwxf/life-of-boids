
# Build

```
mkdir build
cd build
conan install ../..
cmake -DCMAKE_TOOLCHAIN_FILE=conan_paths.cmake ..
```

# CMake options

* `-DENABLE_STATIC_ANALYSIS=ON|OFF` : enable/disable static analysis while compiling

* `-DCMAKE_CXX_COMPILER_LAUNCHER=ccache` : enable `ccache` as compiler cache
