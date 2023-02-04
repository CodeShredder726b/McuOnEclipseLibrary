/*
 * Copyright (c) 2023, Erich Styger
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MCUSEMIHOST_CONFIG_H_
#define MCUSEMIHOST_CONFIG_H_

#define McuSemihost_DEBUG_CONNECTION_GENERIC     (0) /*!< generic debug probe */
#define McuSemihost_DEBUG_CONNECTION_LINKSERVER  (1) /*!< NXP Linkserver debug probe */
#define McuSemihost_DEBUG_CONNECTION_SEGGER      (2) /*!< SEGGER J-Link debug probe */
#define McuSemihost_DEBUG_CONNECTION_PEMICRO     (3) /*!< P&E Multilink debug probe */
#define McuSemihost_DEBUG_CONNECTION_PYOCD       (4) /*!< P&E Multilink debug probe */

#ifndef McuSemihost_CONFIG_DEBUG_CONNECTION
  #define McuSemihost_CONFIG_DEBUG_CONNECTION    McuSemihost_DEBUG_CONNECTION_GENERIC
#endif

#ifndef McuSemihost_CONFIG_INIT_STDIO_HANDLES
  #define McuSemihost_CONFIG_INIT_STDIO_HANDLES  (0)
    /*!< if standard I/O handles (stdin, stdout, stderr) shall be initialized (1) or not (0) */
#endif

/* certain functionality is not implemented depending on the debug connection */
#ifndef McuSemihost_CONFIG_HAS_SYS_RENAME
  /* SEGGER does not allow it for security reason, PyOCD reports 'unimplemented request' */
  #define McuSemihost_CONFIG_HAS_SYS_RENAME  (!(McuSemihost_CONFIG_DEBUG_CONNECTION==McuSemihost_DEBUG_CONNECTION_SEGGER || McuSemihost_CONFIG_DEBUG_CONNECTION==McuSemihost_DEBUG_CONNECTION_PYOCD))
#endif

#ifndef McuSemihost_CONFIG_HAS_SYS_REMOVE
  /* SEGGER does not allow it for security reason, PyOCD reports 'unimplemented request' */
  #define McuSemihost_CONFIG_HAS_SYS_REMOVE  (!(McuSemihost_CONFIG_DEBUG_CONNECTION==McuSemihost_DEBUG_CONNECTION_SEGGER || McuSemihost_CONFIG_DEBUG_CONNECTION==McuSemihost_DEBUG_CONNECTION_PYOCD))
#endif

#endif /* MCUSEMIHOST_CONFIG_H_ */
