cmake_minimum_required(VERSION 3.5)

project(PackageComparison LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

if(CURL_FOUND)
    message(STATUS "Found libcurl: ${CURL_INCLUDE_DIRS}")
    include_directories(${CURL_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "libcurl not found")
endif()


add_executable(PackageComparison main.cpp libPackageComparison.hpp libPackageComparison.cpp)

target_link_libraries(PackageComparison CURL::libcurl)


