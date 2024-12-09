cmake_minimum_required(VERSION 3.22)
project(borka_bp C)

set(CMAKE_C_STANDARD 23)

IF (WIN32)
    set_source_files_properties(${borka_bp_SRCS}
            PROPERTIES LANGUAGE CXX)
ENDIF (WIN32)

include_directories(
        src
        "${CMAKE_CURRENT_SOURCE_DIR}/lib60870-C/src/inc/api"
        "${CMAKE_CURRENT_SOURCE_DIR}/lib60870-C/src/hal/inc"
)
#simple_server.c
#s_server.c
add_executable(borka_bp src/s_server.c)
target_compile_definitions(borka_bp PUBLIC MBEDTLS_PLATFORM_C)
add_subdirectory(lib60870-C)
target_link_libraries(borka_bp PRIVATE lib60870  m)
