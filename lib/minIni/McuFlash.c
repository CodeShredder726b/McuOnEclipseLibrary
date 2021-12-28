/*
 * Copyright (c) 2021, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "McuFlash.h"
#include "McuLib.h"
#if McuLib_CONFIG_CPU_IS_LPC || McuLib_CONFIG_CPU_IS_KINETIS /* currently limited support, only for these CPUs */
#include "McuLog.h"
#include "McuUtility.h"
#if McuLib_CONFIG_CPU_IS_LPC
  #include "fsl_iap.h"
#elif McuLib_CONFIG_CPU_IS_KINETIS
  #include "fsl_flash.h"
  #include "fsl_smc.h"
  #include "McuWait.h"
#endif

#if McuLib_CONFIG_CPU_IS_LPC && McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_LPC845
  /* nothing needed */
#elif   McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN22 \
     || McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02 \
     || McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02 \
     || McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_LPC55S16
  static flash_config_t s_flashDriver;
#endif

bool McuFlash_IsAccessible(const void *addr, size_t nofBytes) {
#if McuLib_CONFIG_CPU_IS_LPC55xx
  /* see https://community.nxp.com/t5/LPC-Microcontrollers-Knowledge/LPC55xx-Erased-Memory-State-0-or-1/ta-p/1135084 and
   * https://www.nxp.com/docs/en/application-note/AN12949.pdf
   * Accessing erased (and not written yet) memory causes a hard-fault.
   */
  status_t status;

  status = FLASH_VerifyErase(&s_flashDriver, (uint32_t)addr, nofBytes);
  if (status==kStatus_Success) {
    return false; /* if it is an erased FLASH: accessing it will cause a hard fault! */
  }
  return true;
#else
  return true;
#endif
}

bool McuFlash_IsErased(const void *addr, size_t nofBytes) {
#if McuLib_CONFIG_CPU_IS_LPC55xx
  /* see https://community.nxp.com/t5/LPC-Microcontrollers-Knowledge/LPC55xx-Erased-Memory-State-0-or-1/ta-p/1135084 and
   * https://www.nxp.com/docs/en/application-note/AN12949.pdf
   * Accessing erased (and not written yet) memory causes a hard-fault.
   */
  status_t status;

  status = FLASH_VerifyErase(&s_flashDriver, (uint32_t)addr, nofBytes);
  return status==kStatus_Success;  /* true if it is an erased FLASH: accessing it will cause a hard fault! */
#else
  uint8_t *ptr = (uint8_t*)addr;
  while (nofBytes>0) {
    if (*ptr!=0xFF) {
      return false;  /* byte not erased */
    }
    ptr++;
    nofBytes--;
  }
  return true;
#endif
}

uint8_t McuFlash_ProgramFlash(void *addr, const void *data, size_t dataSize) {
#if McuLib_CONFIG_CPU_IS_KINETIS
  status_t status;
  uint8_t res = ERR_OK;

  if (McuFlash_Erase(addr, dataSize)!=ERR_OK) {
    return ERR_FAILED;
  }
#if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02 || McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN22
  /* need to switch to normal RUN mode for flash programming,
   * with Fcore=60MHz Fbus=Fflash=20MHz
   * see https://community.nxp.com/thread/377633
   */
  status = SMC_SetPowerModeRun(SMC);
  if (status!=kStatus_Success) {
    return ERR_FAILED;
  }
  McuWait_Waitms(1); /* give time to switch clock, otherwise flash programming might fail below */
#endif
  /* program */
#if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02
    uint32_t primask = DisableGlobalIRQ(); /* workaround: need to disable interrupts? */
#endif
  for(;;) { /* breaks, switch back to HSRUN if things fail */
    status = FLASH_Program(&s_flashDriver, (uint32_t)addr, (uint8_t*)data, dataSize);
    if (status!=kStatus_FTFx_Success) {
      res = ERR_FAILED;
      break;
    }
    break;
  } /* for */
#if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02
  EnableGlobalIRQ(primask); /* workaround: need to disable interrupts? */
#endif
#if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02 || McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN22
  status = SMC_SetPowerModeHsrun(SMC);
  if (status!=kStatus_Success) {
    res = ERR_FAILED;
  }
#endif
  return res;
#elif McuLib_CONFIG_CPU_IS_LPC55xx
  status_t status;
  uint32_t failedAddress, failedData;

  if ((dataSize%s_flashDriver.PFlashPageSize)!=0) { /* must be multiple of flash page size! */
    return ERR_FAILED;
  }
  /* check first if flash is accessible */
  if (!McuFlash_IsAccessible(addr, dataSize)) {
    /* Memory is not accessible or erased. Accessing non-accessile flash it will cause a hard fault! Need to properly erase and program it. */
    status = FLASH_Erase(&s_flashDriver, (uint32_t)addr, dataSize, kFLASH_ApiEraseKey);
    if (status!=kStatus_Success ) {
      McuLog_fatal("erasing failed with error code %d", status);
      return ERR_FAILED;
    }
    /* check if it is erased */
    status = FLASH_VerifyErase(&s_flashDriver, (uint32_t)addr, dataSize);
    if (status!=kStatus_Success) {
      McuLog_fatal("erase check failed");
      return ERR_FAILED;
    }
  }
  /* here the flash is erased, ready for getting programmed */
  status = FLASH_Program(&s_flashDriver, (uint32_t)addr, (uint8_t*)data, dataSize);
  if (status!=kStatus_Success) {
    McuLog_fatal("failed programming flash, error %d", status);
    return ERR_FAILED;
  }
  status = FLASH_VerifyProgram(&s_flashDriver, (uint32_t)addr, dataSize, (const uint8_t *)data, &failedAddress, &failedData);
  if (status!=kStatus_Success) {
    McuLog_fatal("failed verify at address %08x, data %08x", failedAddress, failedData);
    return ERR_FAILED;
  }
  return ERR_OK;
#elif McuLib_CONFIG_CPU_IS_LPC && McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_LPC845
  uint32_t startSector = (uint32_t)addr/McuFlash_CONFIG_FLASH_BLOCK_SIZE; /* sector is 1k in size */
  uint32_t endSector = ((uint32_t)addr+(McuFlash_CONFIG_FLASH_BLOCK_SIZE-1))/McuFlash_CONFIG_FLASH_BLOCK_SIZE;
  uint8_t result = ERR_FAILED; /* default */
  status_t res;

  if (McuFlash_Erase(addr, dataSize)!=ERR_OK) {
    return ERR_FAILED;
  }
  res = IAP_PrepareSectorForWrite(startSector, endSector); /* sector size is 1k */
  if (res!=kStatus_IAP_Success) {
    result = ERR_FAILED;
  } else {
    /* destination address should be on a 64byte boundary.
     * Source address should be word (4byte) boundary
     * data size (number of bytes) shall be 64, 128, 256, 512, 1024 bytes */
    res = IAP_CopyRamToFlash((uint32_t)addr, (uint32_t*)data, dataSize, SystemCoreClock);
    if (res!=kStatus_IAP_Success) {
      result = ERR_FAILED;
    } else {
      res = IAP_Compare((uint32_t)addr, (uint32_t*)data, dataSize);
      if (res!=kStatus_IAP_Success) {
        result = ERR_FAILED;
      } else {
        result = ERR_OK;
      }
    }
  }
  return result;
#else
  #error "target not supported yet!"
  return ERR_FAILED;
#endif /* McuLib_CONFIG_CPU_IS_KINETIS or McuLib_CONFIG_CPU_IS_LPC */
}

uint8_t McuFlash_Erase(void *addr, size_t nofBytes) {
#if McuLib_CONFIG_CPU_IS_LPC && McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_LPC845
  /* determine sector numbers based on block/sector size */
  uint32_t startSector = (uint32_t)addr/McuFlash_CONFIG_FLASH_BLOCK_SIZE;
  uint32_t endSector = (((uint32_t)addr+(McuFlash_CONFIG_FLASH_BLOCK_SIZE-1))/McuFlash_CONFIG_FLASH_BLOCK_SIZE);
  status_t res;

  if (McuFlash_IsErased(addr, nofBytes)) { /* already eased? */
    return ERR_OK; /* yes, nothing to do */
  }
  res = IAP_BlankCheckSector(startSector, endSector);
  if (res==kStatus_IAP_Success) { /* already erased */
    return ERR_OK;
  }
  /* erase sector */
  res = IAP_PrepareSectorForWrite(startSector, endSector);
  if (res!=kStatus_IAP_Success) {
    return ERR_FAILED;
  }
  res = IAP_EraseSector(startSector, endSector, SystemCoreClock);
  if (res!=kStatus_IAP_Success) {
    return ERR_FAILED;
  }
  res = IAP_BlankCheckSector(startSector, endSector);
  if (res!=kStatus_IAP_Success) {
    return ERR_FAILED;
  }
  return ERR_OK;
#elif McuLib_CONFIG_CPU_IS_KINETIS
  uint32_t pflashSectorSize = 0;
  status_t status;
  uint8_t res = ERR_OK;

   if (McuFlash_IsErased(addr, nofBytes)) { /* already eased? */
    return ERR_OK; /* yes, nothing to do */
  }
  #if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02 || McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN22
  /* need to switch to normal RUN mode for flash programming,
   * with Fcore=60MHz Fbus=Fflash=20MHz
   * see https://community.nxp.com/thread/377633
   */
  status = SMC_SetPowerModeRun(SMC);
  if (status!=kStatus_Success) {
    return ERR_FAILED;
  }
  McuWait_Waitms(1); /* give time to switch clock, otherwise flash programming might fail below */
  #endif
  /* erase */
  status = FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0SectorSize, &pflashSectorSize);
  if (status!=kStatus_Success) {
    return ERR_FAILED;
  }
  if (pflashSectorSize!=McuFlash_CONFIG_FLASH_BLOCK_SIZE) {
    return ERR_FAILED;
  }

  for(;;) { /* breaks, switch back to HSRUN on Kinetis if things fail */
  #if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02
    uint32_t primask = DisableGlobalIRQ(); /* workaround: need to disable interrupts? */
  #endif
    status = FLASH_Erase(&s_flashDriver, (uint32_t)addr, nofBytes, kFTFx_ApiEraseKey);
    if (status!=kStatus_FTFx_Success) {
      res = ERR_FAILED;
    #if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02
      EnableGlobalIRQ(primask); /* workaround: need to disable interrupts? */
    #endif
      break; /* error, leave for(;;) loop */
    }
  #if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02
    EnableGlobalIRQ(primask); /* workaround: need to disable interrupts? */
  #endif
    /* Verify sector if it's been erased. */
    status = FLASH_VerifyErase(&s_flashDriver, (uint32_t)addr, nofBytes, kFTFx_MarginValueUser);
    if (status!=kStatus_FTFx_Success) {
      res = ERR_FAILED;
      break;
    }
    break; /* leave loop */
  } /* for */
  #if McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN02 || McuLib_CONFIG_CPU_VARIANT==McuLib_CONFIG_CPU_VARIANT_NXP_FN22
  status = SMC_SetPowerModeHsrun(SMC);
  if (status!=kStatus_Success) {
    res = ERR_FAILED;
  }
  #endif
  return res;
#elif McuLib_CONFIG_CPU_IS_LPC55xx
  static const uint8_t zeroBuffer[McuFlash_CONFIG_FLASH_BLOCK_SIZE]; /* initialized with zeros, buffer in FLASH to save RAM */
  uint8_t res;

  if ((nofBytes%McuFlash_CONFIG_FLASH_BLOCK_SIZE)!=0) { /* check if size is multiple of page size */
    McuLog_fatal("wrong erase data size %d", nofBytes);
    return ERR_FAILED;
  }
  for(int i=0; i<nofBytes/McuFlash_CONFIG_FLASH_BLOCK_SIZE; i++) { /* erase and program each page */
    res = McuFlash_ProgramFlash(addr, zeroBuffer, sizeof(zeroBuffer));
    if (res!=ERR_OK) {
      return res;
    }
  }
  return res;
#endif
}

static uint8_t PrintStatus(const McuShell_StdIOType *io) {
  uint8_t buf[48];

  McuShell_SendStatusStr((unsigned char*)"McuFlash", (unsigned char*)"McuFlash status\r\n", io->stdOut);
  McuUtility_strcpy(buf, sizeof(buf), (unsigned char*)"size 0x");
  McuUtility_strcatNum16Hex(buf, sizeof(buf), McuFlash_CONFIG_FLASH_BLOCK_SIZE);
  McuUtility_strcat(buf, sizeof(buf), (unsigned char*)"\r\n");
  McuShell_SendStatusStr((unsigned char*)"  block", buf, io->stdOut);
  return ERR_OK;
}

uint8_t McuFlash_ParseCommand(const unsigned char *cmd, bool *handled, const McuShell_StdIOType *io) {
  if (McuUtility_strcmp((char*)cmd, McuShell_CMD_HELP)==0 || McuUtility_strcmp((char*)cmd, "McuFlash help")==0) {
    McuShell_SendHelpStr((unsigned char*)"McuFlash", (const unsigned char*)"Group of flash ini commands\r\n", io->stdOut);
    McuShell_SendHelpStr((unsigned char*)"  help|status", (unsigned char*)"Print help or status information\r\n", io->stdOut);
    *handled = TRUE;
    return ERR_OK;
  } else if ((McuUtility_strcmp((char*)cmd, McuShell_CMD_STATUS)==0) || (McuUtility_strcmp((char*)cmd, "McuFlash status")==0)) {
    *handled = TRUE;
    return PrintStatus(io);
  }
  return ERR_OK;
}

void McuFlash_Deinit(void) {
}

void McuFlash_Init(void) {
#if McuLib_CONFIG_CPU_IS_KINETIS || McuLib_CONFIG_CPU_IS_LPC55xx
  status_t result;    /* Return code from each flash driver function */

  memset(&s_flashDriver, 0, sizeof(flash_config_t));
  /* Setup flash driver structure for device and initialize variables. */
  result = FLASH_Init(&s_flashDriver);
#if McuLib_CONFIG_CPU_IS_KINETIS
  if (result!=kStatus_FTFx_Success) {
    McuLog_fatal("NVMC_Init() failed!");
    for(;;) { /* error */ }
  }
#elif McuLib_CONFIG_CPU_IS_LPC55xx
  if (result!=kStatus_Success) {
    McuLog_fatal("NVMC_Init() failed!");
    for(;;) { /* error */ }
  }
#endif
#endif
}

#endif
