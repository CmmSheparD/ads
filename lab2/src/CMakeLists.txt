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
