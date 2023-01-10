add_library(ads-lab2-snail STATIC)
target_sources(ads-lab2-snail
	PRIVATE snail.cpp
	PUBLIC snail.hh
)

add_library(ads-lab2-solve STATIC)
target_sources(ads-lab2-solve
	PRIVATE solve.cpp
	PUBLIC solve.hh
)

add_library(ads-lab2-generate STATIC)
target_sources(ads-lab2-generate
	PRIVATE generate.cpp
	PUBLIC generate.hh
)

add_executable(ads-lab2-main)
target_sources(ads-lab2-main
    PRIVATE main.cpp
    PUBLIC solve.hh snail.hh generate.hh
)

target_link_libraries(ads-lab2-main ads-lab2-solve ads-lab2-snail ads-lab2-generate)

add_executable(ads-lab2-generator)
target_sources(ads-lab2-generator
    PRIVATE generator.cpp
    PUBLIC generate.hh snail.hh
)

target_link_libraries(ads-lab2-generator ads-lab2-generate ads-lab2-snail)
