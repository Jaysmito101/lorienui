file(GLOB_RECURSE IMAGE_VIEWER_INCLUDE_FILES "include/*.h")

add_executable(image-viewer 
    ${IMAGE_VIEWER_INCLUDE_FILES} # needed for visual studio to show the files in the project
    "src/main.c"
)

target_include_directories(image-viewer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(image-viewer PUBLIC ${LORIEN_INCLUDE_DIRS})
target_include_directories(image-viewer PUBLIC ${LORIEN_PLATFORM_ANY_GLFW_OPENGL3_INCLUDE_DIRS})

target_link_libraries(
    image-viewer
    PUBLIC lorien-static
    PUBLIC lorien-platform-any-glfw-opengl3-static
)