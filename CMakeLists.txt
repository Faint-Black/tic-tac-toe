cmake_minimum_required(VERSION 3.10)
project(TicTacToe VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C Standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Specify source files
file(GLOB_RECURSE ALL_SOURCE_FILES CONFIGURE_DEPENDS "src/*.c" "src/*.h")

# Enable Clang-Tidy warnings and errors
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY}")
    message(STATUS "clang-tidy found: ${CLANG_TIDY}")
else()
    message(WARNING "clang-tidy not found.")
endif()

# Enable Clang formatting
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(
        clang-format
        COMMAND ${CLANG_FORMAT} -i ${ALL_SOURCE_FILES}
        COMMENT "Running clang-format on source files"
    )
    message(STATUS "clang-format found: ${CLANG_FORMAT}")
else()
    message(WARNING "clang-format not found.")
endif()

# Source files to executable (or library)
add_executable(executable.out ${ALL_SOURCE_FILES})

# Add compiler options
target_compile_options(executable.out PRIVATE -Wall -Wextra -O2)

# Link libraries if any (for example, Boost, pthread, etc.)
target_link_libraries(executable.out PUBLIC m pthread)

# Add include directories
target_include_directories(executable.out PUBLIC "${PROJECT_SOURCE_DIR}/src/")

# clang-tidy can be set up here for each target, automatically applied during compilation
if(CLANG_TIDY)
    set_target_properties(
        executable.out
        PROPERTIES C_CLANG_TIDY "${CLANG_TIDY}"
    )
endif()
