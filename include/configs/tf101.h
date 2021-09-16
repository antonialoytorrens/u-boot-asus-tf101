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

#define BOARD_EXTRA_ENV_SETTINGS \
	"bootargs=root=/dev/mmcblk1p2 rw nvmem=128M@384M mem=1024M@0M vmalloc=128M gpt console=tty0 usbcore.oldscheme_first=1\0" \
	"kernel_addr_r=0x12000000\0" \
	"dtb_addr_r=0x13000000\0" \
	"ramdisk_addr_r=0x14000000\0" \
	"kernel_file=zImage\0" \
	"dtb_file=tegra20-asus-tf101.dtb\0" \
	"ramdisk_file=uInitrd\0"

/* Boot from fat on uSD */
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"echo Loading DTB;" \
	"fatload mmc 1:1 ${dtb_addr_r} ${dtb_file};" \
	"echo Loading Kernel;" \
	"fatload mmc 1:1 ${kernel_addr_r} ${kernel_file};" \
	"echo Booting Kernel;" \
	"bootz ${kernel_addr_r} - ${dtb_addr_r}"

/* Board-specific serial config */
#define CONFIG_TEGRA_ENABLE_UARTD
#define CONFIG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

#define CONFIG_MACH_TYPE		MACH_TYPE_VENTANA

/* Environment in eMMC, at the end of 2nd "boot sector" */

#include "tegra-common-post.h"

#endif /* __CONFIG_H */
