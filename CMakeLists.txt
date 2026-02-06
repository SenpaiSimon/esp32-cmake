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


set(IDF_SIZE ${python} -m esp_idf_size)
set(MAP_FILE "${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map")
target_link_options(${ELF_FILE} PRIVATE "-Wl,--defsym=IDF_TARGET_${idf_target}=0,--Map=${MAP_FILE}")

idf_build_get_property(idf_path IDF_PATH)
idf_build_get_property(python PYTHON)

add_custom_command(TARGET ${ELF_FILE} POST_BUILD
    COMMAND ${python} ${idf_path}/tools/idf_size.py ${MAP_FILE} --format=table
    COMMENT "Flash usage:"
    VERBATIM
)