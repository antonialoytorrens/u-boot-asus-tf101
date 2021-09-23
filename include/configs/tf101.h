
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 *  (C) Copyright 2010,2011
 *  NVIDIA Corporation <www.nvidia.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>
#include "tegra20-common.h"

/* High-level configuration options */
#define CONFIG_TEGRA_BOARD_STRING	"ASUS Transformer"

/*
*  Boot sequence order:
*  1. USB (checks the two ports on TF101 dock)
*  2. MicroSD
*  3. eMMC
*
*  NOTE: sleep 5 seconds before booting, so the user can read
*/
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"echo Boot order: 1. USB, 2. MicroSD, 3. eMMC;" \
	"echo NOTICE: If you have an SD Card attached to the TF1O1 dock, please disconnect it and reboot.;" \
	"sleep 5;" \
	"echo Searching for uboot-script.cmd...;" \
	"if load usb 0:1 ${scriptaddr} uboot-script.cmd; then " \
		"echo uboot-script.cmd found in first USB.;" \
		"echo Running uboot-script.cmd...;" \
		"sleep 5;" \
		"env import -t -r ${scriptaddr} ${filesize};" \
	"elif load usb 1:1 ${scriptaddr} uboot-script.cmd; then " \
		"echo uboot-script.cmd found in second USB.;" \
		"echo Running uboot-script.cmd...;" \
		"sleep 5;" \
		"env import -t -r ${scriptaddr} ${filesize};" \
	"elif load mmc 1:1 ${scriptaddr} uboot-script.cmd; then " \
		"echo uboot-script.cmd found in MicroSD card.;" \
		"echo Running uboot-script.cmd...;" \
		"sleep 5;" \
		"env import -t -r ${scriptaddr} ${filesize};" \
	"else " \
		"echo Boot Configuration NOT FOUND!; " \
	"fi;"

/* Board-specific serial config */
#define CONFIG_TEGRA_ENABLE_UARTD
#define CONFIG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

#define CONFIG_MACH_TYPE		MACH_TYPE_VENTANA

/* Environment in eMMC, at the end of 2nd "boot sector" */

#include "tegra-common-post.h"

#endif /* __CONFIG_H */
