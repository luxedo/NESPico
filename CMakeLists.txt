cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(NESPico VERSION 1.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(nespico src/nespico.c src/usb_descriptors.c)
target_include_directories(nespico PRIVATE include)

target_link_libraries(nespico PRIVATE pico_stdlib pico_unique_id tinyusb_device tinyusb_board)
pico_add_extra_outputs(nespico)

configure_file(${CMAKE_CURRENT_BINARY_DIR}/nespico.uf2 ${PROJECT_SOURCE_DIR}/bin/nespico.uf2 COPYONLY)

