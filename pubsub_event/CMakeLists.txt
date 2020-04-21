cmake_minimum_required(VERSION 3.1)

project(pubsub_event)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(compiler_settings)

include(template)
make_simple_example_project(${PROJECT_NAME})

### Test
if(${BUILD_TESTS})
    add_subdirectory(test)
endif()