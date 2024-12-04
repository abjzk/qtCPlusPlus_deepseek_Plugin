cmake_minimum_required(VERSION 3.16)
add_subdirectory(QHotkey)
add_subdirectory(LClass)
add_subdirectory(Utility)
add_subdirectory(spdlog)
set_target_properties(
    spdlog
    qhotkey
    Utility
    LCore
    LWidget
    PROPERTIES
    FOLDER "lib"
)