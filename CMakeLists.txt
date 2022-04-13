cmake_minimum_required(VERSION 2.8.9)
project (ft_ls)

# includes
include_directories(src/includes)

# sources
file(GLOB_RECURSE SOURCES "src/*.c")

# compile
add_executable(ft_ls ${SOURCES})
