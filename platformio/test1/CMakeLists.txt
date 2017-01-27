cmake_minimum_required(VERSION 3.2)
project(test1)

set(ENV{PATH} "/home/marierm/bin:/home/marierm/scripts:/usr/local/sbin:/usr/local/bin:/usr/bin:/usr/lib/jvm/default/bin:/usr/bin/site_perl:/usr/bin/vendor_perl:/usr/bin/core_perl")
set(PLATFORMIO_CMD "/usr/bin/platformio")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoavr/variants/eightanaloginputs")
include_directories("$ENV{HOME}/.platformio/packages/framework-arduinoavr/cores/arduino")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-atmelavr/avr/include")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-atmelavr/lib/gcc/avr/4.8.1/include")
include_directories("$ENV{HOME}/.platformio/packages/toolchain-atmelavr/lib/gcc/avr/4.8.1/include-fixed")

add_definitions(-DF_CPU=8000000L)
add_definitions(-DARDUINO_ARCH_AVR)
add_definitions(-DARDUINO_AVR_FIO)
add_definitions(-DARDUINO=10607)
add_definitions(-DPLATFORMIO=020700)
add_definitions(-D__AVR_ATmega328P__)

add_custom_target(
    platformio_build ALL
    COMMAND ${PLATFORMIO_CMD} -f -c emacs run
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    platformio_upload ALL
    COMMAND ${PLATFORMIO_CMD} -f -c emacs run --target upload
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    platformio_clean ALL
    COMMAND ${PLATFORMIO_CMD} -f -c emacs run --target clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(
    platformio_update ALL
    COMMAND ${PLATFORMIO_CMD} -f -c emacs update
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

#
# To enable code auto-completion, please specify path
# to main source file (*.c, *.cpp) and uncomment line below
#
# add_executable(test1 src/main_change_me.cpp)
