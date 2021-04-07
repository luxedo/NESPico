cmake_minimum_required(VERSION 3.13)

# initialize pico-sdk from submodule
# note: this must happen before project()
include(pico-sdk/pico_sdk_init.cmake)

project(PicoNES VERSION 1.0)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico_nes src/pico_nes.c src/usb_descriptors.c)
target_include_directories(pico_nes PRIVATE include)

target_link_libraries(pico_nes PRIVATE pico_stdlib pico_unique_id tinyusb_device tinyusb_board)
pico_add_extra_outputs(pico_nes)
