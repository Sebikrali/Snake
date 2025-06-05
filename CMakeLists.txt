cmake_minimum_required(VERSION 3.30)
project(snake DESCRIPTION "My Snake clone" LANGUAGES C)

set(CMAKE_C_STANDARD 17)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")
file(GLOB_RECURSE SOURCE_FILES 
    "${SOURCE_DIR}*.c"
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC
    include
)

# Define the link libraries
target_link_directories(${PROJECT_NAME} PRIVATE "lib")
set(LINK_LIBRARIES 
    # debug glfw3d optimized glfw3
    # debug glmd optimized glm
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LINK_LIBRARIES})

install(
    TARGETS ${PROJECT_NAME} CONFIGURATIONS Debug DESTINATION "debug"
)
install(
    TARGETS ${PROJECT_NAME} CONFIGURATIONS Release DESTINATION "release"
)

if(${CMAKE_BUILD_TYPE} MATCHES debug)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Og -Wall -Werror -Wpedantic")
elseif(${CMAKE_BUILD_TYPE} MATCHES release)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Werror")
endif()
