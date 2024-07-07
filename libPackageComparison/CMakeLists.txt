cmake_minimum_required(VERSION 3.5)

project(libPackageComparison)

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

add_library(PackageComparisonLib SHARED libPackageComparison.cpp)
target_include_directories(PackageComparisonLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(PackageComparisonLib CURL::libcurl nlohmann_json::nlohmann_json)
