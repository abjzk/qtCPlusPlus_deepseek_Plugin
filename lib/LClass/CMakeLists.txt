cmake_minimum_required(VERSION 3.16)

project("LClass")

add_subdirectory(LCore)
add_subdirectory(LWidget)
set_target_properties(
    LCore
    LWidget
    PROPERTIES
    FOLDER "LClass"
)