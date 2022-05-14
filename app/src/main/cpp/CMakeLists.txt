cmake_minimum_required(VERSION 3.18.1)
project("NDK App Template" VERSION 1.0.0 LANGUAGES C CXX)

set(CLANG_TIDY_PATH
  "${ANDROID_TOOLCHAIN_ROOT}/bin/clang-tidy${ANDROID_TOOLCHAIN_SUFFIX}")
set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_PATH} "-checks=android-*")

find_package(googletest REQUIRED CONFIG)
find_package(junit-gtest REQUIRED CONFIG)

# The library sources are compiled as an OBJECT library rather than as a SHARED
# library to avoid ODR issues with libc++_static when linking the tests. This is
# similar to repackaging a static library as a shared library, but is the CMake
# way of doing it, and doesn't waste time building the intermediate static
# library.
add_library(app_impl OBJECT app.cpp)
target_include_directories(app_impl PUBLIC include)
target_compile_features(app_impl PUBLIC cxx_std_20)
target_compile_options(app_impl
  PRIVATE
    -Wall
    -Wextra
    -Werror
)

# If you change the name of the library, remember to update the
# System.loadLibrary call in MainActivity.kt.
add_library(app SHARED $<TARGET_OBJECTS:app_impl>)
target_link_options(app
  PRIVATE
    -flto
    -Wl,--version-script,${CMAKE_SOURCE_DIR}/libapp.map.txt
)

add_library(app_tests SHARED app_test.cpp)
target_link_libraries(app_tests
  PRIVATE
    $<TARGET_OBJECTS:app_impl>
    googletest::gtest
    googletest::gmock
    junit-gtest::junit-gtest
)