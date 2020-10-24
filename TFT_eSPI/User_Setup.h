//                            USER DEFINED SETTINGS
//   Set driver type, fonts to be loaded, pins used and SPI control method etc
//
//   See the User_Setup_Select.h file if you wish to be able to define multiple
//   setups and then easily select which setup file is used by the compiler.
//
//   If this file is edited correctly then all the library example sketches should
//   run without the need to make any more changes for a particular hardware setup!
//   Note that some sketches are designed for a particular TFT pixel width/height
#define ILI9341_DRIVER 1

#include "Fonts/GFXFF/gfxfont.h"

#define SPI_FREQUENCY  40000000
// Optional reduced SPI frequency for reading TFT
#define SPI_READ_FREQUENCY  40000000
#define SPI_TOUCH_FREQUENCY  2500000

#define TFT_WIDTH  320
#define TFT_HEIGHT 240