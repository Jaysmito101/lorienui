file(GLOB_RECURSE LORIEN_INCLUDE_FILES "include/*.h")

set(LORIEN_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    PARENT_SCOPE
)
set(LORIEN_SOURCES
    "src/lorien_allocator.c"
    "src/lorien_application.c"
    "src/lorien_input_state.c"
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

# Compiler specific warning flags
set(MSVC_WARNING_FLAGS "/W4")
set(GCC_CLANG_WARNING_FLAGS "-Wall -Wextra -pedantic -Werror")

target_compile_options(lorien-static PRIVATE
    $<$<COMPILE_LANG_AND_ID:C,MSVC>:${MSVC_WARNING_FLAGS}>
    $<$<COMPILE_LANG_AND_ID:C,GNU>:${GCC_CLANG_WARNING_FLAGS}>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:${GCC_CLANG_WARNING_FLAGS}>
)

target_compile_options(lorien-shared PRIVATE
    $<$<COMPILE_LANG_AND_ID:C,MSVC>:${MSVC_WARNING_FLAGS}>
    $<$<COMPILE_LANG_AND_ID:C,GNU>:${GCC_CLANG_WARNING_FLAGS}>
    $<$<COMPILE_LANG_AND_ID:C,Clang>:${GCC_CLANG_WARNING_FLAGS}>
)

target_compile_definitions(lorien-static PUBLIC LOR_STATIC)
target_compile_definitions(lorien-shared 
    PUBLIC LOR_SHARED
    PUBLIC LOR_DLL_EXPORTS
)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(lorien-static PUBLIC LOR_DEBUG)
    target_compile_definitions(lorien-shared PUBLIC LOR_DEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(lorien-static PUBLIC LOR_RELEASE)
    target_compile_definitions(lorien-shared PUBLIC LOR_RELEASE)
else()
    message(WARNING "CMAKE_BUILD_TYPE is not set to Debug or Release. No LOR_DEBUG or LOR_RELEASE defined.")
endif()
