file(GLOB_RECURSE LORIEN_INCLUDE_FILES "include/*.h")

set(LORIEN_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)
set(LORIEN_SOURCES
    "src/lorien.c"
)   

add_library(lorien-static STATIC 
    ${LORIEN_INCLUDE_FILES} # needed for visual studio to show the files in the project
    ${LORIEN_SOURCES}
)

add_library(lorien-shared SHARED 
    ${LORIEN_INCLUDE_FILES} # needed for visual studio to show the files in the project
    ${LORIEN_SOURCES}
)

target_include_directories(lorien-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(lorien-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
