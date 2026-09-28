cmake_minimum_required(VERSION 3.23)
project(stars LANGUAGES C RC)

if(NOT MINGW)
    message(FATAL_ERROR "Configure using the MinGW toolchain: cmake --preset mingw-debug")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB STARS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/decompiled/*.c"
)

add_executable(stars WIN32
    ${STARS_SOURCES}
    decompiled/res/stars.rc
)

set_target_properties(stars PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

target_include_directories(stars PRIVATE
    decompiled
    decompiled/res
)

# Match the existing MinGW compile-analysis flags.
target_compile_options(stars PRIVATE
    $<$<COMPILE_LANGUAGE:C>:-Wno-pointer-sign>
)
target_link_libraries(stars PRIVATE user32 gdi32 comdlg32)

# Recompile resources when an embedded bitmap, icon, cursor, or data file changes.
file(GLOB_RECURSE STARS_RESOURCE_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/decompiled/res/*"
)
list(REMOVE_ITEM STARS_RESOURCE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/decompiled/res/stars.rc"
)
set_source_files_properties(decompiled/res/stars.rc PROPERTIES
    OBJECT_DEPENDS "${STARS_RESOURCE_FILES}"
)

find_program(WINE_EXECUTABLE NAMES wine REQUIRED)
set_property(TARGET stars PROPERTY CROSSCOMPILING_EMULATOR "${WINE_EXECUTABLE}")

add_custom_target(run_in_wine
    COMMAND "${WINE_EXECUTABLE}" "$<TARGET_FILE:stars>"
    DEPENDS stars
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    USES_TERMINAL
    VERBATIM
)
