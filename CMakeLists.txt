# Select platform and installation for extra arduino files AND toolchain (compiler...)
SET(PLATFORM_ARCHITECTURE "STM32F1")
# Do we have patch around ?
#
include(./platformConfig.cmake)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(cmake_tools)
include(FindPatch)
SET(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake)
Project("BatteryTester" C CXX ASM)
#
# Patch Arduino_stm32 if needed to add gd32f303 support
#
include(./platformConfig.cmake)
IF(NOT EXISTS "${PLATFORM_PATH}/patched")
    MESSAGE(STATUS "Patching file in ${PLATFORM_PATH} to add gd32f303 support")
    patch_file_p(2 "${PLATFORM_PATH}" "${CMAKE_SOURCE_DIR}/patches/spi_dma_async_patch2.diff")
    file(WRITE "${PLATFORM_PATH}/patched" "patched")
ENDIF(NOT EXISTS "${PLATFORM_PATH}/patched")





SET(EXTENSION             "_STM32F103")
set(ARDUINO_LD_FILE      jtag)      # Use that instead of default jtag-c8 to get the full 128 kB
set(ARDUINO_DEFAULT_BOARD genericSTM32F103C)        # Default Board ID, when not specified
set(ARDUINO_CPU           STM32F103C8)
SET(ARDUINO_UPLOAD_METHOD BMP) # Use blackmagic link, if you comment it out you'll use DFU => 8kB flash

#
#print_board_list()
set(ARDUINO_DEFAULT_PORT ttyACM0) # Default Port, when not specified
set(ARDUINO_CPUMENU .menu.cpu.${ARDUINO_CPU})
SET(libPrefix ${ARDUINO_DEFAULT_BOARD}_)
#
MESSAGE(STATUS "**** Configuration is ** ${CONFIGURATION} **")
SET(SUB_COMPONENTS FreeRTOS adc)
SET(SUB_LIBRARIES TFT_eSPI )

#add_definitions("-O0")



include_directories(${CMAKE_SOURCE_DIR}/stubs)
include_directories(${CMAKE_SOURCE_DIR})
include_directories(FreeRTOS/Source/include)
include_directories(FreeRTOS/)
include_directories(FreeRTOS/Source/portable/Stm32duino/)
include_directories(utils)
include_directories(fonts)
include_directories(adc)


# Build 
add_subdirectory(TFT_eSPI)
FOREACH(comp ${SUB_LIBRARIES} ) # Logical name    
    IMPORT_LIBRARY(${comp})
ENDFOREACH(comp ${SUB_LIBRARIES} )
FOREACH(comp ${SUB_COMPONENTS} )
    add_subdirectory(${comp})
ENDFOREACH(comp ${SUB_COMPONENTS} )

add_definitions("-DPRINTF_DISABLE_SUPPORT_EXPONENTIAL -DPRINTF_DISABLE_SUPPORT_LONG_LONG -DPRINTF_DISABLE_SUPPORT_PTRDIFF_T ")


#
# Our code
#
SET(UTILS utils/fancyLock.cpp     utils/myPwm.cpp utils/i2c_stubs.c)
generate_arduino_firmware(componentTester${EXTENSION}
                
                SRCS 
                            mainCode.cpp 
                            embedded_printf/printf.c    
                            settings.cpp
                            dso_logger.cpp
                            heap.cpp
                            stopWatch.cpp
                            # 42876 => 41772
                            utils/i2c_stubs.c
                            utils/usb_stubs.c
                            #
                            ${UTILS}
                            fonts.cpp
                PORT ${ARDUINO_DEFAULT_PORT}
                BOARD_CPU ${ARDUINO_CPU}
                )
FOREACH(comp ${SUB_COMPONENTS})
    target_link_libraries(componentTester${EXTENSION} ${libPrefix}${comp})
ENDFOREACH(comp ${SUB_COMPONENTS})

FOREACH(comp ${SUB_LIBRARIES} )
    LINK_TO_LIBRARIES(componentTester${EXTENSION} ${comp})
ENDFOREACH(comp ${SUB_LIBRARIES} )



# EOF
