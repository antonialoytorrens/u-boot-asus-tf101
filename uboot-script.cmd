# From U-Boot source code, see the following files o see which environment (env) variables you have available: 
# include/configs/tf101.h
# include/configs/tegra20-common.h
# include/configs/tegra20-common.h
# include/configs/tegra-common-post.h
#
#
# Issues:
#
# 1. Booting from internal eMMC does not work at the moment (mmc 0:X is not detected).
# Tegra partition support driver is needed (it can be picked from grate linux kernel tree).
#
# 2. NOTICE! SD Card is detected as USB Storage Device. So, the Asus TF101 has at maximum 3 USB ports.
# U-Boot calculates how many USB Devices are there (for example, it puts 4 USB Devices when only 1 [Storage] USB Device is connected).
# But that's not a problem when booting, because it detects proper *storage* USB Devices with boot.scr included.
# NOTE: For U-Boot, USB Devices != USB Storage Devices.
# NOTE: USB can be ´usb 0:1`, `usb 1:1` or even `usb 2:1` in TF101 when booting, but we don't know which sdX is assigned for each of them.
#
# 3. `Requested init=/init.sh failed` when booting the kernel if putting init=/init.sh as bootargs. (PostmarketOS)
#
# 4. boot.scr has to be present on MicroSD card (U-Boot reads boot.scr from MMC, and MicroSD is mmc 1:1). 
# Otherwise u-boot does not find the script. We should modify U-Boot to search boot.scr from all the devices (same order as when searching boot.scr). [SOLVED]
#
# 5. NOTICE! Cannot boot from SD Card. It finds boot.scr but we cannot boot from it, even if we have correct partition.
#  (Nasty) WORKAROUND: We can ask to users to disconnect SD Card before booting, so USB detection is way easier.
#  NOTE: usb 0:1 != sda ; usb 1:1 != sdb ; usb 2:1 != sdc ; These are totally independent.
#
# 6. We cannot type U-Boot commands with the dock keyboard. 
# That needs the asusec driver ported to U-boot (it can be picked from grate linux kernel tree).
# WORKAROUND: Connect standard USB Keyboard to the dock keyboard. The device is detected and you can successfully type anything.
#
# 7. Maybe (?) fdt resizing does not work (but this needs to be tested properly, not sure about this last issue).
#
#
# Defined parameters by tegra20-common.h:
#
# scriptaddr=0x10000000
# pxefile_addr_r=0x10100000
# kernel_addr_r=" __stringify(CONFIG_LOADADDR) "
# fdtfile=" FDTFILE "
# fdt_addr_r=0x03000000
# ramdisk_addr_r=0x03100000
#
# Note: fdt stands for Flattened Device Tree (which is tegra20-tf101.dtb)
# We will overwrite fdtfile because we have tegra20-asus-tf101.dtb.

fdtfile=tegra20-asus-tf101.dtb

echo "Booting boot.scr...";
sleep 2;

# Try to boot from USB first
if test ${dev_type} = "usb"; then

    # Boot from first USB (usb 0:1)
    if ${bootdev} -eq 0; then
        setenv bootpartition sda1;
        setenv rootpartition sda2;
        setenv bootpart 1;
        setenv forceresize PMOS_FORCE_PARTITION_RESIZE;

        echo "Booting from first USB...";
        sleep 5;

    # Boot from second USB (usb 1:1)
    else
        setenv bootpartition sdb1;
        setenv rootpartition sdb2;
        setenv bootpart 1;
        setenv forceresize PMOS_FORCE_PARTITION_RESIZE;

        echo "Booting from second USB...";
        sleep 5;
    fi;

# Otherwise, boot from mmc
else
    
    # Boot from MicroSD (mmc 1:1)
    if test ${bootdev} -eq 1; then
        setenv bootpartition sda1;
        setenv rootpartition sda2;
        setenv bootpart 1;
        setenv forceresize PMOS_FORCE_PARTITION_RESIZE;

        echo "Booting from Micro SD Card...";
        sleep 5;
    
    # Boot from internal eMMC (rootfs is UDA, aka Userdata partition: mmc 0:7)
    else
        setenv bootpartition mmcblk0p4;
        setenv rootpartition mmcblk0p7;
        setenv bootpart 7;

        echo "Booting from eMMC...";
        sleep 5;
    fi;
fi;

setenv bootargs root=/dev/${rootpartition} rw nvmem=128M@384M mem=1024M@0M vmalloc=128M gpt console=tty0 usbcore.oldscheme_first=1 lp0_vec=8192@0x1fbed000 PMOS_NO_OUTPUT_REDIRECT ${forceresize} pmos_boot=/dev/${bootpartition} pmos_root=/dev/${rootpartition}

echo Loading DTB
load ${dev_type} ${bootdev}:${bootpart} ${fdt_addr_r} ${fdtfile}

echo Loading Initramfs
load ${dev_type} ${bootdev}:${bootpart} ${ramdisk_addr_r} initramfs
setenv ramdisk_size ${filesize}

echo Loading Kernel
load ${dev_type} ${bootdev}:${bootpart} ${kernel_addr_r} vmlinuz

#echo Resizing FDT
#fdt addr ${fdt_addr_r}
#fdt resize

echo Booting Kernel
bootz ${kernel_addr_r} ${ramdisk_addr_r}:${ramdisk_size} ${fdt_addr_r}
