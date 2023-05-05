// SPDX-License-Identifier: GPL-2.0+
/*
 *  (C) Copyright 2010,2011
 *  NVIDIA Corporation <www.nvidia.com>
 *
 *  (C) Copyright 2021
 *  Svyatoslav Ryhel <clamor95@gmail.com>
 */

/* T20 Transformers derive from Ventana board */

#include <common.h>
#include <dm.h>
#include <env.h>
#include <i2c.h>
#include <log.h>
#include <asm/io.h>
#include <asm/mach-types.h>
#include <asm/arch/tegra.h>
#include <asm/arch-tegra/board.h>
#include <asm/arch-tegra/fuse.h>
#include <asm/arch/clock.h>
#include <asm/arch/funcmux.h>
#include <asm/arch/gpio.h>
#include <asm/arch/pinmux.h>
#include <asm/gpio.h>
#include <linux/delay.h>

#define TPS6586X_I2C_ADDRESS	0x34

#define TPS6586X_SUPPLY_ENC	0x12
#define   LDO3_ONC		BIT(2)

#define TPS6586X_SUPPLYENE	0x14
#define   EXITSLREQ_BIT		BIT(1)
#define   SLEEP_MODE_BIT	BIT(3)

#define TPS6586X_SUPPLY_V4	0x44
#define   VLDO3_3v3		0x07

#ifdef CONFIG_CMD_POWEROFF
int do_poweroff(struct cmd_tbl *cmdtp,
		int flag, int argc, char *const argv[])
{
	struct udevice *dev;
	uchar data_buffer[1];
	int ret;

	ret = i2c_get_chip_for_busnum(0, TPS6586X_I2C_ADDRESS, 1, &dev);
	if (ret) {
		log_debug("cannot find PMIC I2C chip\n");
		return 0;
	}

	ret = dm_i2c_read(dev, TPS6586X_SUPPLYENE, data_buffer, 1);
	if (ret)
		return ret;

	data_buffer[0] &= ~EXITSLREQ_BIT;

	ret = dm_i2c_write(dev, TPS6586X_SUPPLYENE, data_buffer, 1);
	if (ret)
		return ret;

	data_buffer[0] |= SLEEP_MODE_BIT;

	ret = dm_i2c_write(dev, TPS6586X_SUPPLYENE, data_buffer, 1);
	if (ret)
		return ret;

	// wait some time and then print error
	mdelay(5000);
	printf("Failed to power off!!!\n");
	return 1;
}
#endif

#ifdef CONFIG_MMC_SDHCI_TEGRA
/*
 * Routine: pin_mux_mmc
 * Description: setup the pin muxes/tristate values for the SDMMC(s)
 */
void pin_mux_mmc(void)
{
	funcmux_select(PERIPH_ID_SDMMC4, FUNCMUX_SDMMC4_ATB_GMA_GME_8_BIT);
	funcmux_select(PERIPH_ID_SDMMC3, FUNCMUX_SDMMC3_SDB_4BIT);
}
#endif

void nvidia_board_late_init(void)
{
	char serialno_str[17];

	/* Set chip id as serialno */
	sprintf(serialno_str, "%016llx", tegra_chip_uid());
	env_set("serial#", serialno_str);
	env_set("platform", "Tegra 2 T20");
}
