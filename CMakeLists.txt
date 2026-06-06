cmake_minimum_required(VERSION 3.10)
project(SceneReaderProject)

set(CMAKE_CXX_STANDARD 17)

find_package(pugixml CONFIG REQUIRED)

# Header dosyalarının bulunduğu klasörü tanıtmak yeterlidir
include_directories(include)

# Sadece .cpp dosyalarını listele
add_executable(program
    main.cpp
    matrix.cpp
    scene_reader.cpp
    transform.cpp
    camera.cpp
    light.cpp
    material.cpp
    surface.cpp
    scene.cpp
    renderer.cpp
    mesh.cpp
    sphere.cpp
    ray.cpp
    include/parser.h
    parser.cpp
    include/image_loader.h
    image_loader.cpp
)

target_link_libraries(program PRIVATE pugixml::pugixml)