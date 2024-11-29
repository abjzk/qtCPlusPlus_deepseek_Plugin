cmake_minimum_required(VERSION 3.16)
set(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_PATH})
link_directories(
    $<$<CONFIG:Debug>:${OUTPUT_PATH}/Debug>
    $<$<CONFIG:Release>:${OUTPUT_PATH}/Release>
    $<$<CONFIG:MinSizeRel>:${OUTPUT_PATH}/MinSizeRel>
    $<$<CONFIG:RelWithDebInfo>:${OUTPUT_PATH}/RelWithDebInfo>
)
add_subdirectory(Tools)
add_subdirectory(ThirdParty)