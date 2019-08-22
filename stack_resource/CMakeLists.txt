cmake_minimum_required(VERSION 3.1)

project(stack_resource)

include(template)
make_simple_example_project(${PROJECT_NAME})

### Test
if(${BUILD_TESTS})
    add_subdirectory(test)
endif()