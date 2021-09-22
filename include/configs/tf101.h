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

/*
* Defined parameters by tegra20-common.h:
*
* "scriptaddr=0x10000000\0" \
* "pxefile_addr_r=0x10100000\0" \
* "kernel_addr_r=" __stringify(CONFIG_LOADADDR) "\0" \
* "fdtfile=" FDTFILE "\0" \
* "fdt_addr_r=0x03000000\0" \
* "ramdisk_addr_r=0x03100000\0"
*
*/
#define BOARD_EXTRA_ENV_SETTINGS \
	"loadbootscript=load ${dev_type} ${mmcdev}:${mmcpart} ${scriptaddr} boot.scr\0" \
	"loadkernel=load ${dev_type} ${mmcdev}:${mmcpart} ${kernel_addr_r} ${kernelname}\0" \
	"loadinitrd=load ${dev_type} ${mmcdev}:${mmcpart} ${ramdisk_addr_r} ${initrdname}\0" \
	"loaddtb=load ${dev_type} ${mmcdev}:${mmcpart} ${fdt_addr_r} ${fdtfile}\0" \
	"check_ramdisk=" \
		"if run loadinitrd; then " \
			"setenv initrd_addr ${ramdisk_addr_r};" \
		"else " \
			"setenv initrd_addr -;" \
		"fi; \0" \
	"check_dtb=" \
		"if run loaddtb; then " \
			"setenv fdt_addr ${fdt_addr_r};" \
		"else " \
			"setenv fdt_addr;" \
		"fi; \0" \
	"kernel_args=" \
		"setenv bootargs rw nvmem=128M@384M mem=1024M@0M vmalloc=128M gpt" \
		" usbcore.oldscheme_first=1 lp0_vec=8192@0x1fbed000 ${console} ${opts}\0" \
	"boot_script=" \
		"run loadbootscript;" \
		"source ${scriptaddr}\0" \
	"console=tty0\0" \
	"mmcdev=1\0" \
	"mmcpart=1\0" \
	"dev_type=mmc\0" \
	"kernelname=zImage\0" \
	"initrdname=initramfs\0" \
	"scriptaddr=0x12000000\0" \
	"kernel_addr_r=0x13000000\0" \
	"fdt_addr_r=0x14000000\0"

/* Configurate from PER on eMMC */
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"if load usb 0:1 ${scriptaddr} boot.scr; then " \
		"setenv mmcdev 0;" \
		"setenv mmcpart 1;" \
		"setenv dev_type usb;" \
		"run boot_script; " \
	"elif load usb 1:1 ${scriptaddr} boot.scr; then " \
		"setenv mmcdev 0;" \
		"setenv mmcpart 1;" \
		"setenv dev_type usb;" \
		"run boot_script; " \
	"elif load mmc 1:1 ${scriptaddr} boot.scr; then " \
		"setenv mmcdev 1;" \
		"setenv mmcpart 1;" \
		"setenv dev_type mmc;" \
		"run boot_script; " \
	"elif load mmc 0:5 ${scriptaddr} boot.scr; then " \
		"setenv mmcdev 1;" \
		"setenv mmcpart 1;" \
		"setenv dev_type mmc;" \
		"run boot_script; " \
	"fi;\0"

/* Board-specific serial config */
#define CONFIG_TEGRA_ENABLE_UARTD
#define CONFIG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

#define CONFIG_MACH_TYPE		MACH_TYPE_VENTANA

/* Environment in eMMC, at the end of 2nd "boot sector" */

#include "tegra-common-post.h"

#endif /* __CONFIG_H */
