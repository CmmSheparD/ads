add_library(expression STATIC)
target_sources(expression
	PRIVATE expression.cpp
	PUBLIC expression.hh
)

add_library(parsing-table STATIC)
target_sources(parsing-table
	PRIVATE parsing-table.cpp
	PUBLIC parsing-table.hh
)

add_library(parsing STATIC)
target_sources(parsing
	PRIVATE parsing.cpp
	PUBLIC parsing.hh
)

add_executable(converter)
target_sources(converter
	PRIVATE main.cpp
	PUBLIC bst.hh expression.hh parsing.hh
)

target_link_libraries(converter parsing parsing-table expression)
