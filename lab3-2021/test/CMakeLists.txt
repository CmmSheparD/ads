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

add_executable(ads-lab3-queue-test)
target_sources(ads-lab3-queue-test
    PRIVATE lab3-queue-test.cpp
    PUBLIC ../src/queue.hh
)

target_link_libraries(ads-lab3-queue-test gtest_main)

add_executable(ads-lab3-stack-test)
target_sources(ads-lab3-stack-test
    PRIVATE lab3-stack-test.cpp
    PUBLIC ../src/stack.hh
)

target_link_libraries(ads-lab3-stack-test gtest_main)

add_executable(ads-lab3-binary-heap-test)
target_sources(ads-lab3-binary-heap-test
    PRIVATE lab3-binary-heap-test.cpp
    PUBLIC ../src/binary_heap.hh
)

target_link_libraries(ads-lab3-binary-heap-test ads-lab3-binary-heap gtest_main)
