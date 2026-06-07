
cmake_minimum_required(VERSION 3.10)
project(SceneReaderProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(pugixml CONFIG REQUIRED)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "include/*.h")

add_executable(program ${SOURCES} ${HEADERS})


target_include_directories(program PRIVATE include)


target_link_libraries(program PRIVATE pugixml::pugixml)