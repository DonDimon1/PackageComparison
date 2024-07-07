cmake_minimum_required(VERSION 3.5)

project(PackageComparisonProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(libPackageComparison)

add_executable(PackageComparison main.cpp)
target_include_directories(PackageComparison PRIVATE libPackageComparison)

# Добавляем rpath, чтобы указать путь к libPackageComparison.so
set_target_properties(PackageComparison PROPERTIES
    INSTALL_RPATH "$ORIGIN/../build/libPackageComparison"
)

target_link_libraries(PackageComparison PackageComparisonLib)
