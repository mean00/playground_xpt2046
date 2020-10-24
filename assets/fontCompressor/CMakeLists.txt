include_directories(../backgrounds)
include_directories(../fonts)
SET(THEFONT FreeSans24pt7b)

ADD_DEFINITIONS("-DPROGMEM=\"\" -DTHEFONT=${THEFONT}" -g3)
ADD_DEFINITIONS(-include stdint.h -include gfxfont.h -include ${THEFONT}.h)

add_executable(fontCompressor fontCompressor.cpp)
