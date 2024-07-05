cmake_minimum_required(VERSION 3.5)

project(PackageComparison LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(Boost COMPONENTS system REQUIRED)
if(Boost_FOUND)
    message(STATUS "Found BOOST ${Boost_VERSION_STRING}")
endif()

add_executable(PackageComparison main.cpp)

install(TARGETS PackageComparison
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

TARGET_LINK_LIBRARIES(PackageComparison PRIVATE ${Boost_LIBRARIES})
