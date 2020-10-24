
SET(SRCS wav_irotary.cpp  Rotary/Rotary.cpp )
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Rotary)
generate_arduino_library(${libPrefix}RotaryEncoder
                            SRCS ${SRCS}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )
