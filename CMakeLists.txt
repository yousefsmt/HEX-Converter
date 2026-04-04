cmake_minimum_required(VERSION 3.16)

# =========================
# Project
# =========================
project(hexconvert C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# =========================
# Compiler flags
# =========================
add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wshadow
    -Wnull-dereference
    -Wdouble-promotion
    -Wundef
    -fno-common
    -Wconversion
    -fshort-enums
    -fno-stack-protector
)

# =========================
# Find FFTW (pkg-config)
# =========================
# find_package(PkgConfig REQUIRED)
# pkg_check_modules(FFTW3 REQUIRED fftw3)

# =========================
# Source files
# =========================
file(GLOB SRC_FILES
    src/*.c
)

# =========================
# Copy .bin file (input signal)
# =========================
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/data_1M.bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test)
# file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ofdm_config.conf DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# =========================
# Main executable
# =========================
add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        # ${FFTW3_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        # ${FFTW3_LIBRARIES}
        m
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

# =========================
# Testing
# =========================
# enable_testing()
# add_subdirectory(test)
