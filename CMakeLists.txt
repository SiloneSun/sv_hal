cmake_minimum_required(VERSION 3.16)

project(hal_demo LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(HAL_SOURCE_FILES)
set(HAL_HEADER_FILES)
set(HAL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")

foreach(module IN ITEMS app hal hal_adaptor sigma)
    set(module_dir "${CMAKE_CURRENT_SOURCE_DIR}/${module}")
    file(GLOB_RECURSE module_sources CONFIGURE_DEPENDS
        "${module_dir}/*.c"
        "${module_dir}/*.cpp"
    )
    file(GLOB_RECURSE module_headers CONFIGURE_DEPENDS
        "${module_dir}/*.h"
    )
    list(APPEND HAL_SOURCE_FILES ${module_sources})
    list(APPEND HAL_HEADER_FILES ${module_headers})
    list(APPEND HAL_INCLUDE_DIRS "${module_dir}")
    foreach(header IN LISTS module_headers)
        get_filename_component(header_dir "${header}" DIRECTORY)
        list(APPEND HAL_INCLUDE_DIRS "${header_dir}")
    endforeach()
endforeach()

list(REMOVE_DUPLICATES HAL_INCLUDE_DIRS)

add_executable(hal_demo ${HAL_SOURCE_FILES} ${HAL_HEADER_FILES})
target_include_directories(hal_demo PRIVATE ${HAL_INCLUDE_DIRS})
