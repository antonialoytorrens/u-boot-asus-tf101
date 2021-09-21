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
	"script_addr_r=0x1000\0" \
	"kernel_addr_r=0x12000000\0" \
	"dtb_addr_r=0x13000000\0" \
	"ramdisk_addr_r=0x14000000\0" \
	"bootkernel=bootz ${kernel_addr_r} - ${dtb_addr_r}\0" \
	"bootrdkernel=bootz ${kernel_addr_r} ${ramdisk_addr_r} ${dtb_addr_r}\0"


/* Configurate from PER on eMMC */
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"echo Loading BCT;" \
	"if fatload mmc 0:5 ${script_addr_r} uboot-transformer.cmd;" \
	"then env import -t -r ${script_addr_r} ${filesize};" \
	"else echo Boot Configuration NOT FOUND!; fi;" \
	"echo Loading DTB;" \
	"${fs_type} ${dev_type} ${mmcdev}:${mmcpart} ${dtb_addr_r} ${dtb_file};" \
	"echo Loading Kernel;" \
	"${fs_type} ${dev_type} ${mmcdev}:${mmcpart} ${kernel_addr_r} ${kernel_file};" \
	"echo Loading Initramfs;" \
	"if ${fs_type} ${dev_type} ${mmcdev}:${mmcpart} ${ramdisk_addr_r} ${ramdisk_file};" \
	"then setenv rd yes; else setenv rd no; fi;" \
	"echo Booting Kernel;" \
	"if test ${rd} = yes; then run bootrdkernel;" \
	"else run bootkernel; fi;"

/* Board-specific serial config */
#define CONFIG_TEGRA_ENABLE_UARTD
#define CONFIG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

#define CONFIG_MACH_TYPE		MACH_TYPE_VENTANA

/* Environment in eMMC, at the end of 2nd "boot sector" */

#include "tegra-common-post.h"

#endif /* __CONFIG_H */
