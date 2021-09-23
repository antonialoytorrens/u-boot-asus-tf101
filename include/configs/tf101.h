
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
*  VARIABLES CREATED
*
*  ${dev_type}: Device type, can be usb or mmc
*  ${bootdev}: From ${dev_type} (usb or mmc), which device is booted.
*  For example, `mmc list` command shows two devices: 0:X for internal eMMC and 1:X
*  for MicroSD. These numbers, 0 and 1, are defined as this variable called ${bootdev}.
*
*  Boot sequence order:
*  1. USB (checks the two ports on TF101 dock)
*  2. MicroSD
*  3. eMMC (tegra PER partition, aka /dev/mmcblk0p5)
*
*  NOTE: sleep 5 seconds before booting, so the user can read.
*
*  When running `source ${scriptaddr}`, file needs to be boot.scr (executable).
*  uboot-script.cmd is not valid as U-Boot executable unfortunately.
*
*  !! WARNING: !!
*  KNOWN ISSUES:
*
*  1. Booting from internal eMMC does not work at the moment (mmc 0:X is not detected).
*  Tegra partition support driver is needed (it can be picked from grate linux kernel tree).
*
*  2. NOTICE! SD Card is detected as USB Storage Device. So, the Asus TF101 has at maximum 3 USB ports.
*  U-Boot calculates how many USB Devices are there (for example, it puts 4 USB Devices when only 1 [Storage] USB Device is connected).
*  But that's not a problem when booting, because it detects proper *storage* USB Devices with boot.scr included.
*  NOTE: For U-Boot, USB Devices != USB Storage Devices.
*  NOTE: USB can be ´usb 0:1`, `usb 1:1` or even `usb 2:1` in TF101 when booting, but we don't know which sdX is assigned for each of them.
*
*  3. `Requested init=/init.sh failed` when booting the kernel if putting init=/init.sh as bootargs (PostmarketOS).
*  WORKAROUND: Boot without initramfs.
*
*  4. NOTICE! Cannot boot from SD Card. It finds boot.scr but we cannot boot from it, even if we have correct partition.
*  (Nasty) WORKAROUND: We can ask to users to disconnect SD Card before booting, so USB detection is way easier.
*  NOTE: usb 0:1 != sda ; usb 1:1 != sdb ; usb 2:1 != sdc ; These are totally independent.
*
*  5. We cannot type U-Boot commands with the dock keyboard.
*  That needs the asusec driver ported to U-boot (it can be picked from grate linux kernel tree).
*  WORKAROUND: Connect standard USB Keyboard to the dock keyboard. The device is detected and you can successfully type anything.
*
*  6. Maybe (?) fdt resizing does not work (but this needs to be tested properly, not sure about this last issue).
*/
#undef CONFIG_BOOTCOMMAND
#define CONFIG_BOOTCOMMAND \
	"echo Boot order: 1. USB, 2. MicroSD, 3. eMMC;" \
	"echo NOTICE: If you have an SD Card attached to the TF1O1 dock, please disconnect it and reboot.;" \
	"sleep 5;" \
	"echo Searching for boot.scr...;" \
	"if load usb 0:1 ${scriptaddr} boot.scr; then " \
		"setenv dev_type usb;" \
		"setenv bootdev 0;" \
		"echo boot.scr found in first USB.;" \
		"echo Running boot.scr...;" \
		"sleep 5;" \
		"source ${scriptaddr};" \
	"elif load usb 1:1 ${scriptaddr} boot.scr; then " \
		"setenv dev_type usb;" \
		"setenv bootdev 1;" \
		"echo boot.scr found in second USB.;" \
		"echo Running boot.scr...;" \
		"sleep 5;" \
		"source ${scriptaddr};" \
	"elif load mmc 1:1 ${scriptaddr} boot.scr; then " \
		"setenv dev_type mmc;" \
		"setenv bootdev 1;" \
		"echo boot.scr found in MicroSD card.;" \
		"echo Running boot.scr...;" \
		"sleep 5;" \
		"source ${scriptaddr};" \
	"elif load mmc 0:5 ${scriptaddr} boot.scr; then " \
		"setenv dev_type mmc;" \
		"setenv bootdev 0;" \
		"echo boot.scr found in eMMC." \
		"echo Running boot.scr...;" \
		"sleep 5;" \
		"source ${scriptaddr};" \
	"else " \
		"echo Boot Configuration (boot.scr) NOT FOUND!; " \
	"fi;"

/* Board-specific serial config */
#define CONFIG_TEGRA_ENABLE_UARTD
#define CONFIG_SYS_NS16550_COM1		NV_PA_APB_UARTD_BASE

#define CONFIG_MACH_TYPE		MACH_TYPE_VENTANA

/* Environment in eMMC, at the end of 2nd "boot sector" */

#include "tegra-common-post.h"

#endif /* __CONFIG_H */
