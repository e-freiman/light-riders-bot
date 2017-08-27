cmake_minimum_required(VERSION 2.8)

set(HEADERS
	src/Board.h
	src/Bot.h
	src/Parser.h
	src/GameEnums.h
	src/RandomBot.h
	src/BobikBot.h)

set(SOURCES 
	src/Board.cpp 
	src/Bot.cpp 
	src/Parser.cpp
	src/RandomBot.cpp
	src/BobikBot.cpp 
	main.cpp)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -static -Isrc -O2 -lm")
else ()
	message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} is not supported")
endif ()

add_executable(Bobik ${SOURCES} ${HEADERS})
