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
#define CONFIG_TEGRA_BOARD_STRING	"ASUS Eee Pad Transformer TF101"

/* Configurate from PER on eMMC */
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"if load usb 0:1 ${scriptaddr} boot.scr; then " \
		"setenv mmcdev 0;" \
		"setenv mmcpart 1;" \
		"setenv dev_type usb;" \
		"load ${dev_type} ${mmcdev}:${mmcpart} ${scriptaddr} boot.scr;" \
	"elif load usb 1:1 ${scriptaddr} boot.scr; then " \
		"setenv mmcdev 0;" \
		"setenv mmcpart 1;" \
		"setenv dev_type usb;" \
		"load ${dev_type} ${mmcdev}:${mmcpart} ${scriptaddr} boot.scr" \
	"elif load mmc 1:1 ${scriptaddr} boot.scr; then " \
		"setenv mmcdev 1;" \
		"setenv mmcpart 1;" \
		"setenv dev_type mmc;" \
		"load ${dev_type} ${mmcdev}:${mmcpart} ${scriptaddr} boot.scr" \
	"fi;\0"

/* Board-specific serial config */
#define CONFIG_TEGRA_ENABLE_UARTD
#define CONFIG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

#define CONFIG_MACH_TYPE		MACH_TYPE_VENTANA

/* Environment in eMMC, at the end of 2nd "boot sector" */

#include "tegra-common-post.h"

#endif /* __CONFIG_H */
