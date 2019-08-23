cmake_minimum_required(VERSION 3.1)

#
set(TEST_NAME "${PROJECT_NAME}_test")
set(TEST_LABELS "${PROJECT_NAME}")

message("TEST_NAME: " ${TEST_NAME})
message("TEST_LABELS: " ${TEST_LABELS})

# Target source files directory
set(TARGET_SRC_DIR "../src")

# Target include directory
include_directories(${TARGET_SRC_DIR})

#
set(ALL_FILES
    test_stack_resource_v1.cpp
    test_stack_resource_v2.cpp
    # Add a new file here.
    )

#
add_executable(${TEST_NAME} ${ALL_FILES})
target_link_libraries(${TEST_NAME} gtest gmock_main)
add_test(NAME ${TEST_NAME} COMMAND $<TARGET_FILE:${TEST_NAME}>)

# run with: ctest -L xxx
set_property(TEST ${TEST_NAME} PROPERTY LABELS ${TEST_LABELS})