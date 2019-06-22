cmake_minimum_required(VERSION 3.12)
project(json_streamer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "/usr/local/lib")
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.69.0 COMPONENTS filesystem)

include_directories(external/gtest/googlemock/include)
include_directories(external/gtest/googletest/include)
add_subdirectory(external/gtest/googlemock)

include_directories(src)
include_directories(test)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(Tests
        ${CMAKE_CURRENT_LIST_DIR}/test/write.h)

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/src/json_streamer.h)


enable_testing()

add_executable(json_streamer main.cpp ${Sources} ${Tests})
target_link_libraries(json_streamer ${Boost_LIBRARIES})
target_link_libraries(json_streamer gtest)