cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME arduino_pro4)
project(${PROJECT_NAME})

set(${CMAKE_PROJECT_NAME}_SKETCH arduino_pro4.ino)

#### Uncomment below additional settings as needed.
set(${CMAKE_PROJECT_NAME}_BOARD nano)
set(${CMAKE_PROJECT_NAME}_PORT /dev/ttyACM0)
set(nano.build.mcu atmega2560)
set(nano.upload.protocol wiring)
set(nano.upload.speed 115200)
set(BOARD_ID.build.mcu ARCHITECTURE)


generate_arduino_firmware(${CMAKE_PROJECT_NAME})
