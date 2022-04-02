cmake_minimum_required(VERSION 3.17)
project(image_processor)

set(CMAKE_CXX_STANDARD 20)
add_compile_options("-Wall" "-Werror" "-Wextra" "-O0")

#add_link_options("-lasan")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Werror -Wall -Wextra -fsanitize=address,undefined -fno-sanitize-recover=all")

add_executable(image_processor
    image_processor.cpp argument_parses.cpp image.cpp filters/filters.cpp
        filters/blur.cpp filters/crop.cpp filters/sharpening.cpp filters/edge.cpp filters/grayscale.cpp filters/negative.cpp filters/crystallize.cpp
)
