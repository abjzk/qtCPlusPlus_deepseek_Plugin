cmake_minimum_required(VERSION 3.16)

project(DeepSeekanalysisPlugin LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_AUTOUIC_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/ui)
#找依赖库
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network Sql Xml )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network Sql Xml )

set(Lib_DIR ${PROJECT_SOURCE_DIR}/../../lib)
include_directories(${PROJECT_SOURCE_DIR}/include
    ${Lib_DIR}/QHotkey/QHotkey
    ${Lib_DIR}/LClass/LCore/include
    ${Lib_DIR}/LClass/LWidget/include
    ${Lib_DIR}/spdlog/include/spdlog
    ${Lib_DIR}/Utility/include
    ${Lib_DIR}/DeepSeek/include
)

#创建动态库
FILE(GLOB_RECURSE RC_FILES ${PROJECT_SOURCE_DIR}/res/*.rc) 
FILE(GLOB_RECURSE QRC_FILES ${PROJECT_SOURCE_DIR}/res/*.qrc)
FILE(GLOB_RECURSE UI_FILES ${PROJECT_SOURCE_DIR}/ui/*.ui)
FILE(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/include/*.h)
include(CMakePrintHelpers)
cmake_print_variables(HEADER_FILES)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_FILES)
# FILE(GLOB_RECURSE INCLUDE_FILES ${PROJECT_SOURCE_DIR}/../include/*.h)
add_library(DeepSeekanalysisPlugin SHARED
  ${SOURCE_FILES}
  ${HEADER_FILES}
  ${UI_FILES}
  ${RC_FILES}
  ${QRC_FILES}
)


#链接找到的依赖库
target_link_libraries(DeepSeekanalysisPlugin PRIVATE Qt${QT_VERSION_MAJOR}::Core
  LWidget
  LCore
  spdlog
  Utility
  DeepSeek
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Xml
  # Qt${QT_VERSION_MAJOR}::WebEngineWidgets
  Qt${QT_VERSION_MAJOR}::Widgets

)

target_compile_definitions(DeepSeekanalysisPlugin PRIVATE DeepSeekanalysisPlugin_LIBRARY)
