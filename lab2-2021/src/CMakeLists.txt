add_library(ads-lab2-searches STATIC)
target_sources(ads-lab2-searches
	PRIVATE searches.cpp
	PUBLIC searches.hh
)

add_library(ads-lab2-sorts STATIC)
target_sources(ads-lab2-sorts
	PRIVATE sorts.cpp
	PUBLIC sorts.hh
)

add_executable(ads-lab2-measure)
target_sources(ads-lab2-measure
    PRIVATE measure.cpp
    PUBLIC sorts.hh
)

target_link_libraries(ads-lab2-measure ads-lab2-sorts)
