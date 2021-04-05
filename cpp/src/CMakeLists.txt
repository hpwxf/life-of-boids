FIND_PACKAGE(glfw3 REQUIRED)
#FIND_PACKAGE(glad REQUIRED)

#include("/Users/pascal/haveneer/life-of-boids/tmp/glfw-install/lib/cmake/glfw3/glfw3Config.cmake")


link_libraries(glfw)



set(GLAD_GL
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/glad/gl.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/deps/glad_gl.c")


include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps")

add_executable(life-of-bois main.cpp ${GLAD_GL})
#target_link_libraries(life-of-bois PRIVATE glfw::glfw)
#target_include_directories(life-of-bois PRIVATE ${glfw_INCLUDE_DIRS})