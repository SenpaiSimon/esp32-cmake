cmake_minimum_required(VERSION 3.22)
project(my_custom_app CXX C)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include CMake file that provides ESP-IDF CMake build system APIs.
# include($ENV{IDF_PATH}/tools/cmake/toolchain-${TARGET}.cmake)
include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

idf_build_process("${TARGET}"          
    COMPONENTS freertos esptool_py           
    SDKCONFIG ${CMAKE_CURRENT_LIST_DIR}/sdkconfig
    BUILD_DIR ${CMAKE_BINARY_DIR}
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
idf_build_set_property(MINIMAL_BUILD ON)

## Add components
add_subdirectory(Components)

## main target
set(ELF_FILE ${CMAKE_PROJECT_NAME}.elf)

# Create the executable from `main.c` so we can attach IDF libraries to it,
# then hand it over to the IDF helper to finish configuration.
add_executable(${ELF_FILE} main.cpp)

target_link_libraries(${ELF_FILE} 
    PRIVATE
        idf::freertos 
        idf::spi_flash 
        Logger
        Thread
)

# Let the IDF build system create and configure the executable target.
idf_build_executable(${ELF_FILE})