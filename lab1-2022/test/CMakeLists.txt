cmake_minimum_required(VERSION 3.16)

# Adding googletest to test things. Hope it works
# I don't get how it works. Hope to get it in the future..
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(ads-lab1-2022-test)
target_sources(ads-lab1-2022-test
    PRIVATE lab1-test.cpp
    PUBLIC ../src/list.hh
)

target_link_libraries(ads-lab1-2022-test gtest_main)
