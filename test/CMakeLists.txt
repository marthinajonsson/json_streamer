#cmake_minimum_required(VERSION 3.14)
#project(testapp)
#set(CMAKE_CXX_STANDARD 17)
set (Boost_USE_STATIC_LIBS OFF)


find_package (Boost REQUIRED COMPONENTS unit_test_framework)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(Sources
        ${CMAKE_CURRENT_LIST_DIR}/testmain.cpp
        ${CMAKE_CURRENT_LIST_DIR}/write.h)

add_executable (testapp ${Sources})

set_target_properties(testapp PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries (testapp ${Boost_LIBRARIES})