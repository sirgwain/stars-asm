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

# Deterministic regression builds replace only WinMain's initial RNG seed.
# Keep the checked-in generated source intact so regeneration needs no patch.
set(STARS_TEST_SEED "" CACHE STRING "Fixed startup RNG seed for regression tests (empty uses the clock)")
if(NOT STARS_TEST_SEED STREQUAL "")
    if(NOT STARS_TEST_SEED MATCHES "^(0|[1-9][0-9]*)$" OR STARS_TEST_SEED GREATER 4294967295)
        message(FATAL_ERROR "STARS_TEST_SEED must be a decimal uint32")
    endif()
    set(STARS_MAIN "${CMAKE_CURRENT_SOURCE_DIR}/decompiled/stars.c")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${STARS_MAIN}")
    file(READ "${STARS_MAIN}" STARS_MAIN_TEXT)
    string(REGEX MATCHALL "Randomize2\\(GetTickCount\\(\\)\\)" STARS_SEED_CALLS "${STARS_MAIN_TEXT}")
    list(LENGTH STARS_SEED_CALLS STARS_SEED_CALL_COUNT)
    if(NOT STARS_SEED_CALL_COUNT EQUAL 1)
        message(FATAL_ERROR "Expected exactly one startup Randomize2(GetTickCount()) call in stars.c")
    endif()
    string(REPLACE "Randomize2(GetTickCount())" "Randomize2(${STARS_TEST_SEED}u)"
        STARS_MAIN_TEXT "${STARS_MAIN_TEXT}")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/stars-seeded.c" "${STARS_MAIN_TEXT}")
    list(REMOVE_ITEM STARS_SOURCES "${STARS_MAIN}")
    list(APPEND STARS_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/stars-seeded.c")
endif()

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
