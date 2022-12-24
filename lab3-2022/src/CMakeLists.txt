add_executable(ads-lab3-measure)
target_sources(ads-lab3-measure
	PRIVATE measure.cpp
	PUBLIC sorts.hh
)

add_executable(ads-lab3-generate)
target_sources(ads-lab3-generate
	PRIVATE generate.cpp
)
