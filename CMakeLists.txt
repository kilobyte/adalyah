cmake_minimum_required(VERSION 2.8.9)
project(colorized-logs CXX)

set(CMAKE_CXX_FLAGS "-Wall -Werror=format-security -Wmissing-declarations -Wredundant-decls -Wno-parentheses ${CMAKE_CXX_FLAGS}")

add_executable(adalyah
	coord.cc
	colours.cc
	errors.cc
	keyboard.cc
	los.cc
	main.cc
	map.cc
	object.cc
	random.cc
	term.cc
	test.cc
	unicode.cc
)

install(TARGETS adalyah
	DESTINATION games)
