cmake_minimum_required(VERSION 2.8.9)

# settings
project (ft_ls)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wall -Werror -g -DMACOS_RENDER")

# includes
include_directories(src/includes)

# sources
file(GLOB_RECURSE SOURCES "src/*.c")

# compile
add_executable(ft_ls ${SOURCES})
