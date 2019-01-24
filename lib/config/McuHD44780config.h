/**
 * \file
 * \brief Configuration header file for LCDHTA
 *
 * This header file is used to configure settings of the character LCD module.
 */

#ifndef __McuHD44780_CONFIG_H
#define __McuHD44780_CONFIG_H

#define McuHD44780_CONFIG_LCD_NOF_LINES                        (2) /* 1, 2, 3 or 4 */

#define McuHD44780_CONFIG_LCD_DATA_BUS_WIDTH                   (4) /* 4 or 8 */

#include "RS1.h" /* RS signal pin header file */
#include "EN1.h" /* EN signal pin header file */
#if McuHD44780_CONFIG_LCD_DATA_BUS_WIDTH==8 /* extra includes for 8bit data bus */
  #include "DB0.h" /* DB0 signal pin header file */
  #include "DB1.h" /* DB1 signal pin header file */
  #include "DB2.h" /* DB2 signal pin header file */
  #include "DB3.h" /* DB3 signal pin header file */
#endif /* McuHD44780_CONFIG_LCD_DATA_BUS_WIDTH==8 */
#include "DB41.h" /* DB4 signal pin header file */
#include "DB51.h" /* DB5 signal pin header file */
#include "DB61.h" /* DB6 signal pin header file */
#include "DB71.h" /* DB7 signal pin header file */

#ifndef McuHD44780_CONFIG_USE_E2_SIGNAL
  #define McuHD44780_CONFIG_USE_E2_SIGNAL   (0)
  /*!< 1: Use E2 signal to control upper and lower half of display. 0: do not use E2 signal */
#endif

#if McuHD44780_CONFIG_USE_E2_SIGNAL
  #include "E2.h"
#endif

#ifndef McuHD44780_CONFIG_USE_DISPLAY_READ
  #define McuHD44780_CONFIG_USE_DISPLAY_READ   (0)
  /*!< 1: Read from the display. Requires R/W signal. 0: do not read from he display */
#endif

#ifndef McuHD44780_CONFIG_USE_DISPLAY_READ_CHECK_BUSY_FLAG
  #define McuHD44780_CONFIG_USE_DISPLAY_READ_CHECK_BUSY_FLAG   (0 && McuHD44780_CONFIG_USE_DISPLAY_READ)
  /*!< 1: Check the busy flag Requires reading from the display. 0: do not check busy flag */
#endif

#ifndef McuHD44780_CONFIG_USE_RW_SIGNAL
  #define McuHD44780_CONFIG_USE_RW_SIGNAL   (0 && McuHD44780_CONFIG_USE_DISPLAY_READ)
  /*!< 1: Use RW signal to read/write from the display. 0: do not use RW signal */
#endif

#if McuHD44780_CONFIG_USE_RW_SIGNAL
  #include "RW.h"
#endif


#define McuHD44780_CONFIG_WAIT_DISPLAY_US                      0 /* wait time as specified in properties */
#define McuHD44780_CONFIG_WAIT_LCD_CMD_AFTER_4BIT_DATA1_US     15 /* wait time after first 4bit data */
#define McuHD44780_CONFIG_WAIT_LCD_CMD_AFTER_4BIT_DATA2_US     60 /* wait time after second 4bit data */


#endif /* __McuHD44780_CONFIG_H */
