cmake_minimum_required(VERSION 2.8.11)

project(fuzzypp)



include_directories(
    ./
)

add_definitions(
    -DNDEBUG
)

# Linker options
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS}  ")

# Library path
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L\".\" ")

# Define the CXX sources
set ( CXX_SRCS
    BlockhashContext.cpp
    FuzzyConstants.cpp
    fuzzypp.cpp
    Hasher.cpp
    Roll.cpp
)

set_source_files_properties(
    ${CXX_SRCS} PROPERTIES COMPILE_FLAGS 
    " -O2 -std=c++11 -Wall")

add_executable(fuzzypp ${CXX_SRCS} ${C_SRCS})

