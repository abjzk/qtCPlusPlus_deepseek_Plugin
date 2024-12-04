cmake_minimum_required(VERSION 3.16)
add_subdirectory(ImageToIcoPlugin)
set_target_properties(
    ImageToIcoPlugin
    PROPERTIES
    FOLDER "Plugin"
)