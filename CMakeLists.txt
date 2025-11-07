cmake_minimum_required(VERSION 3.10)
project(RasterizationTests VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/tests")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

include_directories(${PROJECT_SOURCE_DIR}/include)

# 排除 main.cpp，只包含库文件
file(GLOB_RECURSE COMMON_SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
list(FILTER COMMON_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

message(STATUS "Found common sources:")
foreach(src ${COMMON_SOURCES})
    message(STATUS " ${src}")    
endforeach()

file(GLOB TEST_FILES
    "${PROJECT_SOURCE_DIR}/tests/*.cpp"
)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/output)

foreach(test_file ${TEST_FILES})
    get_filename_component(test_name ${test_file} NAME_WE)

    message(STATUS "Configuring test: ${test_name}")

    file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/output/${test_name})

    add_executable(${test_name}
        ${test_file}
        ${COMMON_SOURCES}
    )

    set_target_properties(${test_name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
    )

    list(APPEND ALL_TESTS ${test_name})
endforeach()

if(ALL_TESTS)
    add_custom_target(all-tests)
    add_dependencies(all-tests ${ALL_TESTS})
    message(STATUS "Total tests configured: ${ALL_TESTS}")
endif()

# 添加主程序目标
add_executable(main_program
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
    ${COMMON_SOURCES}
)

set_target_properties(main_program PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build
)

message(STATUS "Main program configured")