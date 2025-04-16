file(GLOB_RECURSE GLAD_INCLUDE_FILES "include/*.h")

add_library(glad
    ${GLAD_INCLUDE_FILES} # needed for visual studio to show the files in the project
    "src/glad.c"
)


target_include_directories(
    glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    glad
)

set(GLAD_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)