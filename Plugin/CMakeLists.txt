 
cmake_minimum_required(VERSION 3.16) 

# 获取当前目录下所有子目录
file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

# 循环遍历所有子目录并添加
foreach(SUBDIR ${SUBDIRS})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR})
        add_subdirectory(${SUBDIR})
        message(STATUS "Found subdirectory: ${SUBDIR}")
        set_target_properties(${SUBDIR} PROPERTIES FOLDER "Plugin")
    endif()
endforeach()