/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuSDCard.h
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : SD_Card
**     Version     : Component 01.184, Driver 01.00, CPU db: 3.00.000
**     Repository  : Legacy User Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2018-06-20, 13:08, # CodeGen: 327
**     Abstract    :
**         Implements interface to SD card for FatFs
**     Settings    :
**          Component name                                 : McuSDCard
**          Block size                                     : 512
**          Cmd wait counter                               : 10
**          Wait Ready Timeout (ms)                        : 500
**          Wait Cmd Timeout (ms)                          : 100
**          Receive Block Timeout (ms)                     : 500
**          SPI Block Transfer                             : no
**          Hardware                                       : 
**            SW SPI                                       : Enabled
**              SPI                                        : McuGenericSWSPI
**            HW SPI                                       : Disabled
**            SPI Read/Write Macros                        : Disabled
**            Slave Select                                 : Enabled
**              LDD SS                                     : Enabled
**                Slave Select Pin                         : LDDSS
**              non-LDD SS                                 : Disabled
**            Activate                                     : Disabled
**            Card detection                               : Disabled
**            Report 'Card present' if no Card detection pin: yes
**            Write protection                             : Disabled
**          System                                         : 
**            Wait                                         : McuWait
**            Timeout                                      : McuTimeout
**            RTOS                                         : Enabled
**              RTOS                                       : McuRTOS
**     Contents    :
**         Activate         - void McuSDCard_Activate(void);
**         Deactivate       - void McuSDCard_Deactivate(void);
**         isWriteProtected - bool McuSDCard_isWriteProtected(void);
**         CardPresent      - bool McuSDCard_CardPresent(void);
**         WaitReady        - uint8_t McuSDCard_WaitReady(void);
**         ReceiveDataBlock - bool McuSDCard_ReceiveDataBlock(uint8_t *data, uint16_t nofBytes);
**         SendDataBlock    - bool McuSDCard_SendDataBlock(uint8_t *data, uint8_t token, uint16_t nofBytes);
**         SendCmd          - uint8_t McuSDCard_SendCmd(uint8_t cmd, uint32_t arg);
**         SetSlowMode      - void McuSDCard_SetSlowMode(void);
**         SetFastMode      - void McuSDCard_SetFastMode(void);
**         InitCommChannel  - void McuSDCard_InitCommChannel(void);
**         Deinit           - uint8_t McuSDCard_Deinit(void* unused);
**         Init             - uint8_t McuSDCard_Init(void* unused);
**
**     * Copyright (c) 2012-2017, Erich Styger
**      * Web:         https://mcuoneclipse.com
**      * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**      * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**      * All rights reserved.
**      *
**      * Redistribution and use in source and binary forms, with or without modification,
**      * are permitted provided that the following conditions are met:
**      *
**      * - Redistributions of source code must retain the above copyright notice, this list
**      *   of conditions and the following disclaimer.
**      *
**      * - Redistributions in binary form must reproduce the above copyright notice, this
**      *   list of conditions and the following disclaimer in the documentation and/or
**      *   other materials provided with the distribution.
**      *
**      * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**      * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**      * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**      * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**      * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**      * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**      * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**      * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**      * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**      * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuSDCard.h
** @version 01.00
** @brief
**         Implements interface to SD card for FatFs
*/         
/*!
**  @addtogroup McuSDCard_module McuSDCard module documentation
**  @{
*/         

#ifndef __McuSDCard_H
#define __McuSDCard_H

/* MODULE McuSDCard. */

/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
/* Include inherited beans */
#include "McuGenericSWSPI.h"
#include "SS1.h"
#include "McuWait.h"
#include "McuTimeout.h"
#include "McuRTOS.h"
/* interface for FatFS low level disk functions */
#include "diskio.h"

DSTATUS McuSDCard_disk_initialize (
        uint8_t drv                     /* Physical drive number (0..) */
);
DSTATUS McuSDCard_disk_status (
        uint8_t drv                     /* Physical drive number (0..) */
);
DRESULT McuSDCard_disk_read (
        uint8_t drv,                    /* Physical drive number (0..) */
        uint8_t *buff,                  /* Data buffer to store read data */
        uint32_t sector,                /* Sector address (LBA) */
        unsigned int count              /* Number of sectors to read (1..255) */
);
#if _READONLY == 0
DRESULT McuSDCard_disk_write (
        uint8_t drv,                    /* Physical drive number (0..) */
        const uint8_t *buff,            /* Data to be written */
        uint32_t sector,                /* Sector address (LBA) */
        unsigned int count              /* Number of sectors to write (1..255) */
);
#endif
DRESULT McuSDCard_disk_ioctl (
        uint8_t drv,                    /* Physical drive number (0..) */
        uint8_t ctrl,                   /* Control code */
        void *buff                      /* Buffer to send/receive control data */
);

#include "Cpu.h"


/* User definitions */

/* distinguished modes for bus activation/deactivation */
#define McuSDCard_ACTIVATE_MODE_SLOW   0
#define McuSDCard_ACTIVATE_MODE_FAST   1
#define McuSDCard_ACTIVATE_MODE_NONE   2

#define McuSDCard_BLOCK_SIZE   512      /* user defined block size */

/******************************* SD Card Standard Commands **********************************/
#define McuSDCard_CMD0  (0x40+0)        /* Resets the SD Memory Card */
#define McuSDCard_CMD1  (0x40+1)        /* Sends host capacity support information and activates the card's
                                           initialization process. HCS is effective when card receives SEND_IF_COND
                                           command. Reserved bits shall be set to '0'. */
#define McuSDCard_CMD6  (0x40+6)        /* Checks switchable function (mode 0) and switches card function (mode 1).*/
#define McuSDCard_CMD8  (0x40+8)        /* Sends SD Memory Card interface condition that includes host supply voltage
                                           information and asks the accessed card whether card can operate in supplied
                                           voltage range. Reserved bits shall be set to '0'.*/
#define McuSDCard_CMD9  (0x40+9)        /* Asks the selected card to send its cardspecific data (CSD)*/
#define McuSDCard_CMD10 (0x40+10)       /* Asks the selected card to send its card identification (CID) */
#define McuSDCard_CMD12 (0x40+12)       /* Forces the card to stop transmission in Multiple Block Read Operation */
#define McuSDCard_CMD13 (0x40+13)       /* Asks the selected card to send its status register. */
#define McuSDCard_CMD16 (0x40+16)       /* Sets a block length (in bytes) for all following block commands (read and
                                           write) of a Standard Capacity Card. Block length of the read and write
                                           commands are fixed to 512 bytes in a High Capacity Card. The length of
                                           LOCK_UNLOCK command is set by this command in both capacity cards.*/
#define McuSDCard_CMD17 (0x40+17)       /* Reads a block of the size selected by the SET_BLOCKLEN command.*/
#define McuSDCard_CMD18 (0x40+18)       /* Continuously transfers data blocks from card to host until interrupted by a
                                           STOP_TRANSMISSION command.*/
#define McuSDCard_CMD24 (0x40+24)       /* Writes a block of the size selected by the SET_BLOCKLEN command. */
#define McuSDCard_CMD25 (0x40+25)       /* Continuously writes blocks of data until �Stop Tran� token is sent
                                          (instead �Start Block�).*/
#define McuSDCard_CMD27 (0x40+27)       /* Programming of the programmable bits of the CSD. */
#define McuSDCard_CMD28 (0x40+28)       /* If the card has write protection features, this command sets the write protection bit
                                           of the addressed group. The properties of write protection are coded in the card
                                           specific data (WP_GRP_SIZE). The High Capacity Card does not support this command.*/
#define McuSDCard_CMD29 (0x40+29)       /* If the card has write protection features, this command clears the write protection
                                           bit of the addressed group. The High Capacity Card does not support this command. */
#define McuSDCard_CMD30 (0x40+30)       /* If the card has write protection features, this command asks the card to send the
                                           status of the write protection bits.6 The High Capacity Card does not support this command. */
#define McuSDCard_CMD32 (0x40+32)       /* Sets the address of the first write block to be erased.*/
#define McuSDCard_CMD33 (0x40+33)       /* Sets the address of the last write block of the continuous range to be erased. */
#define McuSDCard_CMD38 (0x40+38)       /* Erases all previously selected write blocks */
#define McuSDCard_CMD42 (0x40+42)       /* Used to Set/Reset the Password or lock/unlock the card. A transferred data block includes
                                           all the command details - refer to Chapter 4.3.7. The size of the Data Block is defined
                                           with SET_BLOCK_LEN command. Reserved bits in the argument and in Lock Card Data Structure
                                           shall be set to 0. */
#define McuSDCard_CMD55 (0x40+55)       /* Defines to the card that the next command is an application specific command
                                           rather than a standard command */
#define McuSDCard_CMD56 (0x40+56)       /* Used either to transfer a Data Block to the card or to get a Data Block from the card
                                           for general purpose/application specific commands. In case of Standard Capacity SD
                                           Memory Card, the size of the Data Block shall be defined with SET_BLOCK_LEN command.
                                           Block length of this command is fixed to 512-byte in High Capacity Card. */
#define McuSDCard_CMD58 (0x40+58)       /* Reads the OCR register of a card. CCS bit is assigned to OCR[30]. */
#define McuSDCard_CMD59 (0x40+59)       /* Turns the CRC option on or off. A �1� in the CRC option bit will turn the option on,
                                           a �0� will turn it off */
#define McuSDCard_ACMD41 (0xC0+41)      /* SEND_OP_COND (SDC) */
#define McuSDCard_ACMD13 (0xC0+13)      /* SD_STATUS (SDC) */
#define McuSDCard_ACMD23 (0xC0+23)      /* SET_WR_BLK_ERASE_COUNT (SDC) */



uint8_t McuSDCard_Init(void* unused);
/*
** ===================================================================
**     Method      :  McuSDCard_Init (component SD_Card)
**     Description :
**         Initializes the driver
**     Parameters  :
**         NAME            - DESCRIPTION
**       * unused          - unused parameter
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void McuSDCard_Activate(void);
/*
** ===================================================================
**     Method      :  McuSDCard_Activate (component SD_Card)
**     Description :
**         If multiple devices are used on the same SPI bus, then the
**         device needs to be activated. That way, the different SPI
**         protocol is selected.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuSDCard_Deactivate(void);
/*
** ===================================================================
**     Method      :  McuSDCard_Deactivate (component SD_Card)
**     Description :
**         Removes/deactivates the card from the bus
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

#define McuSDCard_isWriteProtected() \
  FALSE                                 /* no hardware to detect write protection, thus none */

/*
** ===================================================================
**     Method      :  McuSDCard_isWriteProtected (component SD_Card)
**     Description :
**         Determines if the card is write protected. Note that this is
**         an indicator only, as it is still possible to write to the
**         card even if the write protection is set on the card!
**     Parameters  : None
**     Returns     :
**         ---             - True if the card has the write protection
**                           set, false otherwise
** ===================================================================
*/

uint8_t McuSDCard_WaitReady(void);
/*
** ===================================================================
**     Method      :  McuSDCard_WaitReady (component SD_Card)
**     Description :
**         Wait until the card is ready
**     Parameters  : None
**     Returns     :
**         ---             - Error code
**                           ERR_OK: device is ready
**                           ERR_BUSY: device is still busy
** ===================================================================
*/

bool McuSDCard_ReceiveDataBlock(uint8_t *data, uint16_t nofBytes);
/*
** ===================================================================
**     Method      :  McuSDCard_ReceiveDataBlock (component SD_Card)
**     Description :
**         Retrieve a data block from the device
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Pointer to data buffer
**         nofBytes        - number of bytes to retrieve,
**                           must be a multiple of 4
**     Returns     :
**         ---             - TRUE if reading was going fine, FALSE
**                           otherwise.
** ===================================================================
*/

uint8_t McuSDCard_SendCmd(uint8_t cmd, uint32_t arg);
/*
** ===================================================================
**     Method      :  McuSDCard_SendCmd (component SD_Card)
**     Description :
**         Sends a command to the device and returns the response
**     Parameters  :
**         NAME            - DESCRIPTION
**         cmd             - Command to send
**         arg             - command argument
**     Returns     :
**         ---             - device response
** ===================================================================
*/

uint8_t McuSDCard_ReceiveByte(void);
/*
** ===================================================================
**     Method      :  McuSDCard_ReceiveByte (component SD_Card)
**
**     Description :
**         Receives a byte from the SPI bus
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/

#define McuSDCard_CardPresent() \
  TRUE                                  /* no card detection pin, but user wants to report TRUE */

/*
** ===================================================================
**     Method      :  McuSDCard_CardPresent (component SD_Card)
**     Description :
**         Returns true in case a card is present. If there is no card
**         detection pin, then this routine will always return true.
**     Parameters  : None
**     Returns     :
**         ---             - Returns true if card is present, false
**                           otherwise.
** ===================================================================
*/

bool McuSDCard_SendDataBlock(uint8_t *data, uint8_t token, uint16_t nofBytes);
/*
** ===================================================================
**     Method      :  McuSDCard_SendDataBlock (component SD_Card)
**     Description :
**         Send a data block to the device
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Pointer to data blocks with 512 bytes
**                           each
**         token           - data/stop token
**         nofBytes        - Number of bytes to send
**     Returns     :
**         ---             - Returns TRUE for success, FALSE for
**                           failure.
** ===================================================================
*/

void McuSDCard_SetFastMode(void);
/*
** ===================================================================
**     Method      :  McuSDCard_SetFastMode (component SD_Card)
**     Description :
**         Switches to fast mode SPI communication speed.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void McuSDCard_InitCommChannel(void);
/*
** ===================================================================
**     Method      :  McuSDCard_InitCommChannel (component SD_Card)
**     Description :
**         Method to initialize the communication channel. This is
**         needed if the bus to the SD card is shared with other
**         devices.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

uint8_t McuSDCard_Deinit(void* unused);
/*
** ===================================================================
**     Method      :  McuSDCard_Deinit (component SD_Card)
**     Description :
**         Driver deinitialization routine.
**     Parameters  :
**         NAME            - DESCRIPTION
**       * unused          - dummy parameter
**     Returns     :
**         ---             - Error code
** ===================================================================
*/

void McuSDCard_SetSlowMode(void);
/*
** ===================================================================
**     Method      :  McuSDCard_SetSlowMode (component SD_Card)
**     Description :
**         Switches to slow mode SPI communication speed.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/* END McuSDCard. */

#endif
/* ifndef __McuSDCard_H */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
