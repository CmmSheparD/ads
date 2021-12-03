cmake_minimum_required(VERSION 3.16)

# Adding googletest to test things. Hope it works
# I don't get how it works. Hope to get it in the future..
include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(ads-lab2-searches-test)
target_sources(ads-lab2-searches-test
    PRIVATE lab2-searches-test.cpp
    PUBLIC ../src/searches.hh
)

target_link_libraries(ads-lab2-searches-test ads-lab2-searches gtest_main)

add_executable(ads-lab2-sorts-test)
target_sources(ads-lab2-sorts-test
    PRIVATE lab2-sorts-test.cpp
    PUBLIC ../src/sorts.hh
)

target_link_libraries(ads-lab2-sorts-test ads-lab2-sorts gtest_main)
