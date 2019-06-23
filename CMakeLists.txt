cmake_minimum_required(VERSION 3.14)
project(json_streamer)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.69.0 COMPONENTS filesystem)

include_directories(src)
add_subdirectory(test)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/json_streamer.h)

add_library(json_streamer STATIC ${Sources})

set_target_properties(json_streamer PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(json_streamer ${Boost_LIBRARIES})