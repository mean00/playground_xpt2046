
SET(SRCS 
    UI_widget.cpp
    UI_widget_button.cpp
    UI_screen.cpp
) 
generate_arduino_library(${libPrefix}UI_engine
                            SRCS ${SRCS}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )
target_include_directories(${libPrefix}UI_engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
EXPORT_LIBRARY(UI_engine FLAGS "" INCLUDE_PATHS "${CMAKE_CURRENT_SOURCE_DIR}" NAME "${libPrefix}UI_engine")
