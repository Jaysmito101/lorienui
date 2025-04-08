file(GLOB_RECURSE LORIEN_PLATFORM_ANY_GLFW_OPENGL3_INCLUDE_FILES "include/*.h")

set(LORIEN_PLATFORM_ANY_GLFW_OPENGL3_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)
set(LORIEN_PLATFORM_ANY_GLFW_OPENGL3_SOURCES
    "src/platform.c"
)   

add_library(lorien-platform-any-glfw-opengl3-static STATIC 
    ${LORIEN_PLATFORM_ANY_GLFW_OPENGL3_INCLUDE_FILES} # needed for visual studio to show the files in the project
    ${LORIEN_PLATFORM_ANY_GLFW_OPENGL3_SOURCES}
)

add_library(lorien-platform-any-glfw-opengl3-shared SHARED 
    ${LORIEN_PLATFORM_ANY_GLFW_OPENGL3_INCLUDE_FILES} # needed for visual studio to show the files in the project
    ${LORIEN_PLATFORM_ANY_GLFW_OPENGL3_SOURCES}
)

target_include_directories(lorien-platform-any-glfw-opengl3-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lorien-platform-any-glfw-opengl3-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
