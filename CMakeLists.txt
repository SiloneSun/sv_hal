cmake_minimum_required(VERSION 3.16)

project(hal_demo LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 平台选择只影响构建，HAL 和 App 不包含芯片选择逻辑。
set(HAL_PLATFORM "sigma_308" CACHE STRING "Target HAL platform")
set_property(CACHE HAL_PLATFORM PROPERTY STRINGS sigma_308 hisi test)
if(HAL_PLATFORM STREQUAL "sigma_308")
    set(HAL_PLATFORM_DIR "hal_adaptor/sigma")
elseif(HAL_PLATFORM STREQUAL "hisi")
    set(HAL_PLATFORM_DIR "hal_adaptor/hisi")
elseif(HAL_PLATFORM STREQUAL "test")
    set(HAL_PLATFORM_DIR "hal_adaptor/test")
else()
    message(FATAL_ERROR "Unsupported HAL_PLATFORM: ${HAL_PLATFORM}")
endif()
message(STATUS "HAL platform: ${HAL_PLATFORM}")

set(HAL_SOURCE_FILES)
set(HAL_HEADER_FILES)
set(HAL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}")

set(HAL_MODULE_DIRS app hal "${HAL_PLATFORM_DIR}")
if(HAL_PLATFORM STREQUAL "test")
    list(APPEND HAL_MODULE_DIRS bsp/test)
endif()
foreach(module IN LISTS HAL_MODULE_DIRS)
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

find_package(Threads REQUIRED)

add_executable(hal_demo ${HAL_SOURCE_FILES} ${HAL_HEADER_FILES})
target_include_directories(hal_demo PRIVATE ${HAL_INCLUDE_DIRS})
target_link_libraries(hal_demo PRIVATE Threads::Threads)

include(CTest)
if(BUILD_TESTING AND HAL_PLATFORM STREQUAL "test")
    add_executable(test_sensor_test
        tests/test_sensor_test.cpp
        hal/hal_base_node.cpp
        hal/hal_pipeline.cpp
        hal_adaptor/test/test_sensor.cpp
        bsp/test/color_sensor.cpp
        bsp/test/node.cpp
        bsp/test/file_save.cpp
    )
    target_include_directories(test_sensor_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
    target_link_libraries(test_sensor_test PRIVATE Threads::Threads)
    add_test(NAME test_pipeline COMMAND test_sensor_test)
    set_tests_properties(test_pipeline PROPERTIES TIMEOUT 15)
endif()
