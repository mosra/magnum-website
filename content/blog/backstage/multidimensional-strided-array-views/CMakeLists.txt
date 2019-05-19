cmake_minimum_required(VERSION 3.1)
project(ImagePrinter)

find_package(Magnum REQUIRED Trade)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(thing main.cpp)
target_link_libraries(thing PRIVATE
    Magnum::Magnum
    Magnum::Trade)
