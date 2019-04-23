/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuSSD1306.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : SSD1306
**     Version     : Component 01.044, Driver 01.00, CPU db: 3.00.000
**     Compiler    : GNU C Compiler
**     Date/Time   : 2019-04-23, 06:31, # CodeGen: 489
**     Abstract    :
**         Display driver for the SSD1306 OLED module
**     Settings    :
**          Component name                                 : McuSSD1306
**          Type                                           : 128x64
**          Driver                                         : SSD1306
**          Orientation                                    : landscape
**          Width                                          : 128
**          Height                                         : 64
**          Bytes in rows                                  : no
**          Bytes in x direction                           : yes
**          MSB first                                      : no
**          Bits per pixel                                 : 1
**          Window capability                              : no
**          Display Memory Write                           : no
**          Display Memory Read                            : no
**          Use RAM Buffer                                 : yes
**          Clear display in init                          : no
**          Initialize on Init                             : yes
**          Init Delay (ms)                                : 5
**          HW                                             : 
**            I2C Device Address                           : 0x3C
**            I2C Transaction Delay (us)                   : 100
**            Bock Transfer                                : yes
**            I2C                                          : McuGenericI2C
**            Reset                                        : Disabled
**          System                                         : 
**            Wait                                         : McuWait
**            SDK                                          : McuLib
**     Contents    :
**         GetWidth              - McuSSD1306_PixelDim McuSSD1306_GetWidth(void);
**         GetHeight             - McuSSD1306_PixelDim McuSSD1306_GetHeight(void);
**         GetLongerSide         - McuSSD1306_PixelDim McuSSD1306_GetLongerSide(void);
**         GetShorterSide        - McuSSD1306_PixelDim McuSSD1306_GetShorterSide(void);
**         SetDisplayOrientation - void McuSSD1306_SetDisplayOrientation(McuSSD1306_DisplayOrientation...
**         GetDisplayOrientation - McuSSD1306_DisplayOrientation McuSSD1306_GetDisplayOrientation(void);
**         PutPixel              - void McuSSD1306_PutPixel(McuSSD1306_PixelDim x, McuSSD1306_PixelDim y,...
**         Clear                 - void McuSSD1306_Clear(void);
**         UpdateFull            - void McuSSD1306_UpdateFull(void);
**         UpdateRegion          - void McuSSD1306_UpdateRegion(McuSSD1306_PixelDim x, McuSSD1306_PixelDim y,...
**         InitCommChannel       - void McuSSD1306_InitCommChannel(void);
**         SetContrast           - uint8_t McuSSD1306_SetContrast(uint8_t contrast);
**         DisplayOn             - uint8_t McuSSD1306_DisplayOn(bool on);
**         DisplayInvert         - uint8_t McuSSD1306_DisplayInvert(bool invert);
**         GetLCD                - void McuSSD1306_GetLCD(void);
**         GiveLCD               - void McuSSD1306_GiveLCD(void);
**         SetRowCol             - uint8_t McuSSD1306_SetRowCol(uint8_t row, uint8_t col);
**         PrintString           - void McuSSD1306_PrintString(uint8_t line, uint8_t col, uint8_t *str);
**         ClearLine             - void McuSSD1306_ClearLine(uint8_t line);
**         Deinit                - void McuSSD1306_Deinit(void);
**         Init                  - void McuSSD1306_Init(void);
**
** * Copyright (c) 2017-2019, Erich Styger
**  * Web:         https://mcuoneclipse.com
**  * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**  * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**  * All rights reserved.
**  *
**  * Redistribution and use in source and binary forms, with or without modification,
**  * are permitted provided that the following conditions are met:
**  *
**  * - Redistributions of source code must retain the above copyright notice, this list
**  *   of conditions and the following disclaimer.
**  *
**  * - Redistributions in binary form must reproduce the above copyright notice, this
**  *   list of conditions and the following disclaimer in the documentation and/or
**  *   other materials provided with the distribution.
**  *
**  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**  * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**  * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**  * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**  * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**  * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**  * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuSSD1306.h
** @version 01.00
** @brief
**         Display driver for the SSD1306 OLED module
*/         
/*!
**  @addtogroup McuSSD1306_module McuSSD1306 module documentation
**  @{
*/         

#ifndef __McuSSD1306_H
#define __McuSSD1306_H

/* MODULE McuSSD1306. */
#include "McuLib.h" /* SDK and API used */
#include "McuSSD1306config.h" /* configuration */
#include <stddef.h> /* for size_t */


#if McuSSD1306_CONFIG_SSD1306_SIZE_TYPE==12864
  #define McuSSD1306_DISPLAY_HW_NOF_COLUMNS  128u /* number of columns in hardware */
  #define McuSSD1306_DISPLAY_HW_NOF_ROWS      64u /* number of rows in hardware */
  #define McuSSD1306_DISPLAY_HW_NOF_PAGES      8u /* number of pages in hardware */
#elif McuSSD1306_CONFIG_SSD1306_SIZE_TYPE==12832
  #define McuSSD1306_DISPLAY_HW_NOF_COLUMNS  128u /* number of columns in hardware */
  #define McuSSD1306_DISPLAY_HW_NOF_ROWS      32u /* number of rows in hardware */
  #define McuSSD1306_DISPLAY_HW_NOF_PAGES      4u /* number of pages in hardware */
#else
  #error "unknown display type, must be 128x64 or 128x32"
#endif

typedef bool McuSSD1306_PixelColor;    /* type to hold color information */
typedef uint8_t McuSSD1306_PixelDim;   /* one byte is enough to describe the x/y position */
typedef uint16_t McuSSD1306_PixelCount; /* needed type to hold the number of pixels on the display. */

extern uint8_t McuSSD1306_DisplayBuf[((McuSSD1306_DISPLAY_HW_NOF_ROWS-1)/8)+1][McuSSD1306_DISPLAY_HW_NOF_COLUMNS]; /* buffer for the display */

#define McuSSD1306_PIXEL_BLACK  0 /* 0 is a black pixel */
#define McuSSD1306_PIXEL_WHITE  1 /* 1 is a color/white pixel */
#define McuSSD1306_COLOR_PIXEL_SET      McuSSD1306_PIXEL_WHITE /* color for a pixel set */
#define McuSSD1306_COLOR_PIXEL_CLR      McuSSD1306_PIXEL_BLACK /* color for a pixel cleared */

#define McuSSD1306_COLOR_BLACK          McuSSD1306_PIXEL_BLACK
#define McuSSD1306_COLOR_WHITE          McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_RED            McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_BRIGHT_RED     McuSSD1306_COLOR_WHITE
#define McuSSD1306_COLOR_DARK_RED       McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_GREEN          McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_BRIGHT_GREEN   McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_DARK_GREEN     McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_BLUE           McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_BRIGHT_BLUE    McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_DARK_BLUE      McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_YELLOW         McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_BRIGHT_YELLOW  McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_ORANGE         McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_GREY           McuSSD1306_PIXEL_WHITE
#define McuSSD1306_COLOR_BRIGHT_GREY    McuSSD1306_PIXEL_WHITE

#define McuSSD1306_PIXEL_ON  McuSSD1306_COLOR_WHITE /* value of a pixel if it is 'on' */
#define McuSSD1306_PIXEL_OFF McuSSD1306_COLOR_BLACK /* value of a pixel if it is 'off' */

#define McuSSD1306_HW_LONGER_SIDE  McuSSD1306_DISPLAY_HW_NOF_COLUMNS            /* Hardware display longer side in pixels */
#define McuSSD1306_HW_SHORTER_SIDE McuSSD1306_DISPLAY_HW_NOF_ROWS               /* Hardware display shorter side in pixels */

typedef enum {
  McuSSD1306_ORIENTATION_PORTRAIT    = 0,
  McuSSD1306_ORIENTATION_PORTRAIT180 = 1,
  McuSSD1306_ORIENTATION_LANDSCAPE   = 2,
  McuSSD1306_ORIENTATION_LANDSCAPE180= 3
} McuSSD1306_DisplayOrientation;

#define McuSSD1306_WritePixel(data)  \
  McuSSD1306_WriteDataWord(data)

#define McuSSD1306_ReadPixel(data)  \
  0 /* with the I2C interface it is NOT possible to read from display memory */



void McuSSD1306_Clear(void);
/*
** ===================================================================
**     Method      :  Clear (component SSD1306)
**
**     Description :
**         Clears the whole display memory.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_Init(void);
/*
** ===================================================================
**     Method      :  Init (component SSD1306)
**
**     Description :
**         Display driver initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_UpdateFull(void);
/*
** ===================================================================
**     Method      :  UpdateFull (component SSD1306)
**
**     Description :
**         Updates the whole display from the microcontroller RAM
**         display buffer.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_UpdateRegion(McuSSD1306_PixelDim x, McuSSD1306_PixelDim y, McuSSD1306_PixelDim w, McuSSD1306_PixelDim h);
/*
** ===================================================================
**     Method      :  UpdateRegion (component SSD1306)
**
**     Description :
**         Updates a region of the display. This is only a stub for
**         this display as we are using windowing.
**     Parameters  :
**         NAME            - DESCRIPTION
**         x               - x coordinate
**         y               - y coordinate
**         w               - width of the region
**         h               - Height of the region
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_OpenWindow(McuSSD1306_PixelDim x0, McuSSD1306_PixelDim y0, McuSSD1306_PixelDim x1, McuSSD1306_PixelDim y1);
/*
** ===================================================================
**     Method      :  McuSSD1306_OpenWindow (component SSD1306)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

#define McuSSD1306_CloseWindow()  /* nothing to do */

/*
** ===================================================================
**     Method      :  McuSSD1306_CloseWindow (component SSD1306)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void McuSSD1306_WriteData(uint8_t data);
/*
** ===================================================================
**     Method      :  McuSSD1306_WriteData (component SSD1306)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

McuSSD1306_DisplayOrientation McuSSD1306_GetDisplayOrientation(void);
/*
** ===================================================================
**     Method      :  GetDisplayOrientation (component SSD1306)
**
**     Description :
**         Returns the current display orientation
**     Parameters  : None
**     Returns     :
**         ---             - current display orientation
** ===================================================================
*/

void McuSSD1306_SetDisplayOrientation(McuSSD1306_DisplayOrientation newOrientation);
/*
** ===================================================================
**     Method      :  SetDisplayOrientation (component SSD1306)
**
**     Description :
**         Sets the display orientation. If you enable this method,
**         then the orientation of the display can be changed at
**         runtime. However, this requires additional resources.
**     Parameters  :
**         NAME            - DESCRIPTION
**         newOrientation  - new orientation to
**                           be used
**     Returns     : Nothing
** ===================================================================
*/

#if McuSSD1306_CONFIG_DYNAMIC_DISPLAY_ORIENTATION
  McuSSD1306_PixelDim McuSSD1306_GetWidth(void);
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT
  #define McuSSD1306_GetWidth()    McuSSD1306_HW_SHORTER_SIDE
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT180
  #define McuSSD1306_GetWidth()    McuSSD1306_HW_SHORTER_SIDE
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE
  #define McuSSD1306_GetWidth()    McuSSD1306_HW_LONGER_SIDE
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE180
  #define McuSSD1306_GetWidth()    McuSSD1306_HW_LONGER_SIDE
#endif
/*
** ===================================================================
**     Method      :  GetWidth (component SSD1306)
**
**     Description :
**         Returns the width of the display in pixels (in x direction)
**     Parameters  : None
**     Returns     :
**         ---             - Width of display
** ===================================================================
*/

#if McuSSD1306_CONFIG_DYNAMIC_DISPLAY_ORIENTATION
  McuSSD1306_PixelDim McuSSD1306_GetHeight(void);
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT
  #define McuSSD1306_GetHeight()    McuSSD1306_HW_LONGER_SIDE
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT180
  #define McuSSD1306_GetHeight()    McuSSD1306_HW_LONGER_SIDE
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE
  #define McuSSD1306_GetHeight()    McuSSD1306_HW_SHORTER_SIDE
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE180
  #define McuSSD1306_GetHeight()    McuSSD1306_HW_SHORTER_SIDE
#endif
/*
** ===================================================================
**     Method      :  GetHeight (component SSD1306)
**
**     Description :
**         Returns the height of the display in pixels (in y direction)
**     Parameters  : None
**     Returns     :
**         ---             - Height of display
** ===================================================================
*/

#define McuSSD1306_GetLongerSide() \
  McuSSD1306_HW_LONGER_SIDE
/*
** ===================================================================
**     Method      :  GetLongerSide (component SSD1306)
**
**     Description :
**         Returns the size of the longer side of the display
**     Parameters  : None
**     Returns     :
**         ---             - number of pixels
** ===================================================================
*/

#define McuSSD1306_GetShorterSide() \
  McuSSD1306_HW_SHORTER_SIDE
/*
** ===================================================================
**     Method      :  GetShorterSide (component SSD1306)
**
**     Description :
**         Returns the size of the shorter side of the display
**     Parameters  : None
**     Returns     :
**         ---             - number of pixels
** ===================================================================
*/

uint16_t McuSSD1306_ReadDataWord(void);
/*
** ===================================================================
**     Method      :  McuSSD1306_ReadDataWord (component SSD1306)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

void McuSSD1306_GetLCD(void);
/*
** ===================================================================
**     Method      :  GetLCD (component SSD1306)
**
**     Description :
**         Method to be called for mutual exclusive access to the LCD
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_GiveLCD(void);
/*
** ===================================================================
**     Method      :  GiveLCD (component SSD1306)
**
**     Description :
**         Method to be called for mutual exclusive access to the LCD
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_InitCommChannel(void);
/*
** ===================================================================
**     Method      :  InitCommChannel (component SSD1306)
**
**     Description :
**         Method to initialize communication to the LCD. Needed if the
**         bus to the LCD is shared with other components and settings
**         are different.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

uint8_t McuSSD1306_SetContrast(uint8_t contrast);
/*
** ===================================================================
**     Method      :  SetContrast (component SSD1306)
**
**     Description :
**         Sets the display contrast level (default:0x7F)
**     Parameters  :
**         NAME            - DESCRIPTION
**         contrast        - Contrast level (default 0x7F).
**                           Contrast increases as the value increases.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuSSD1306_DisplayOn(bool on);
/*
** ===================================================================
**     Method      :  DisplayOn (component SSD1306)
**
**     Description :
**         Turns the display on or off (sleep)
**     Parameters  :
**         NAME            - DESCRIPTION
**         on              - turns the display on or off
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

uint8_t McuSSD1306_DisplayInvert(bool invert);
/*
** ===================================================================
**     Method      :  DisplayInvert (component SSD1306)
**
**     Description :
**         Used to inverse the display. In normal mode, 1 one in the
**         display RAM is a pixel set and a 0 means pixel clear.
**     Parameters  :
**         NAME            - DESCRIPTION
**         invert          - if TRUE, a 1 is pixel clear and a 0
**                           is a pixel set.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void McuSSD1306_PrintString(uint8_t line, uint8_t col, uint8_t *str);
/*
** ===================================================================
**     Method      :  PrintString (component SSD1306)
**
**     Description :
**         Simple low level method printing text to the display.
**         Newline is supported.
**     Parameters  :
**         NAME            - DESCRIPTION
**         line            - line number, starting with 0
**         col             - column number, starting with 0
**       * str             - Pointer to string to be printed on display
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_PutPixel(McuSSD1306_PixelDim x, McuSSD1306_PixelDim y, McuSSD1306_PixelColor color);
/*
** ===================================================================
**     Method      :  PutPixel (component SSD1306)
**
**     Description :
**         Draws a pixel into the display buffer (not on the display).
**     Parameters  :
**         NAME            - DESCRIPTION
**         x               - x position of the pixel
**         y               - y position of the pixel
**         color           - color value of the pixel
**     Returns     : Nothing
** ===================================================================
*/

void McuSSD1306_Deinit(void);
/*
** ===================================================================
**     Method      :  Deinit (component SSD1306)
**
**     Description :
**         Driver de-initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

uint8_t McuSSD1306_SetRowCol(uint8_t row, uint8_t col);
/*
** ===================================================================
**     Method      :  SetRowCol (component SSD1306)
**
**     Description :
**         Sets the column and row position, useful for start writing
**         text with PrintString()
**     Parameters  :
**         NAME            - DESCRIPTION
**         row             - row (or line) number, starting with 0
**         col             - column number, starting with 0
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void McuSSD1306_ClearLine(uint8_t line);
/*
** ===================================================================
**     Method      :  ClearLine (component SSD1306)
**
**     Description :
**         Clear a text line on the display
**     Parameters  :
**         NAME            - DESCRIPTION
**         line            - Line number, starting with zero
**     Returns     : Nothing
** ===================================================================
*/

/* END McuSSD1306. */

#endif
/* ifndef __McuSSD1306_H */
/*!
** @}
*/
