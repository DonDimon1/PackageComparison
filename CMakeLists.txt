cmake_minimum_required(VERSION 3.5)

project(PackageComparison LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
if(CURL_FOUND)
    message(STATUS "Found libcurl: ${CURL_INCLUDE_DIRS}")
    include_directories(${CURL_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "libcurl not found")
endif()

#FIND_PACKAGE(Boost COMPONENTS system REQUIRED)
#find_package(Boost REQUIRED COMPONENTS system thread json)
#if(Boost_FOUND)
#    message(STATUS "Found BOOST ${Boost_VERSION_STRING}")
#endif()


#include_directories(${Boost_INCLUDE_DIRS})

add_executable(PackageComparison main.cpp)

install(TARGETS PackageComparison LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

target_link_libraries(PackageComparison CURL::libcurl)

#TARGET_LINK_LIBRARIES(PackageComparison PRIVATE ${Boost_LIBRARIES})
