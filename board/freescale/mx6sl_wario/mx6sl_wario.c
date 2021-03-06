/*
 * Copyright (C) 2013-2014 Freescale Semiconductor, Inc.
 *
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/arch/clock.h>
#include <asm/arch/iomux.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/crm_regs.h>
#include <asm/arch/mx6-pins.h>
#include <asm/arch/sys_proto.h>
#include <asm/gpio.h>
#include <asm/imx-common/iomux-v3.h>
#include <asm/imx-common/boot_mode.h>
#include <asm/io.h>
#include <linux/sizes.h>
#include <common.h>
#include <fsl_esdhc.h>
#include <mmc.h>
#include <netdev.h>
#ifdef CONFIG_SYS_I2C_MXC
#include <i2c.h>
#include <asm/imx-common/mxc_i2c.h>
#endif
#if defined(CONFIG_MXC_EPDC)
#include <lcd.h>
#include <mxc_epdc_fb.h>
#endif
#include "pinmux.h"

#ifdef CONFIG_FASTBOOT
#include "mx6sl_wario_board.h"
#include <fastboot.h>
#ifdef CONFIG_ANDROID_RECOVERY
#include <recovery.h>
#endif
#endif /*CONFIG_FASTBOOT*/


DECLARE_GLOBAL_DATA_PTR;


int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)PHYS_SDRAM, PHYS_SDRAM_SIZE);

	return 0;
}

static iomux_v3_cfg_t const board_unused_pads[] = {
	//3V3
	MX6_PAD_KEY_COL0__GPIO_3_24 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_KEY_COL1__GPIO_3_26 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD1_DAT7__GPIO_5_10 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_SD1_DAT6__GPIO_5_7 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EPDC_D10__GPIO_1_17 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EPDC_D11__GPIO_1_18 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EPDC_D12__GPIO_1_19 | MUX_PAD_CTRL(NO_PAD_CTRL),
	//1V8
	MX6_PAD_ECSPI2_SS0__GPIO_4_15 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EPDC_PWRCOM__GPIO_2_11 | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_FEC_RXD1__GPIO_4_18 | MUX_PAD_CTRL(NO_PAD_CTRL)
};


static void setup_iomux_unused_pins(void)
{
	imx_iomux_v3_setup_multiple_pads(board_unused_pads, ARRAY_SIZE(board_unused_pads));
	//Set unused GPIOs to input
	gpio_direction_input(IMX_GPIO_NR(3,24));
	gpio_direction_input(IMX_GPIO_NR(3,26));
	gpio_direction_input(IMX_GPIO_NR(5,10));
	gpio_direction_input(IMX_GPIO_NR(5,7));
	gpio_direction_input(IMX_GPIO_NR(1,17));
	gpio_direction_input(IMX_GPIO_NR(1,18));
	gpio_direction_input(IMX_GPIO_NR(1,19));
	gpio_direction_input(IMX_GPIO_NR(4,15));
	gpio_direction_input(IMX_GPIO_NR(2,11));
	gpio_direction_input(IMX_GPIO_NR(4,18));
}

#ifdef CONFIG_FSL_ESDHC

static struct fsl_esdhc_cfg usdhc_cfg[3] = {
	{USDHC1_BASE_ADDR, 0, 4},
	{USDHC2_BASE_ADDR, 0, 8},
	{USDHC3_BASE_ADDR, 0, 4},
};

int mmc_get_env_devno(void)
{
	u32 soc_sbmr = readl(SRC_BASE_ADDR + 0x4);
	u32 dev_no;
	u32 bootsel;

	bootsel = (soc_sbmr & 0x000000FF) >> 6 ;

	/* If not boot from sd/mmc, use default value */
	if (bootsel != 1)
		return CONFIG_SYS_MMC_ENV_DEV;

	/* BOOT_CFG2[3] and BOOT_CFG2[4] */
	dev_no = (soc_sbmr & 0x00001800) >> 11;

	return dev_no;
}

int mmc_map_to_kernel_blk(int dev_no)
{
	return dev_no;
}

int board_mmc_getcd(struct mmc *mmc)
{
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;

	switch (cfg->esdhc_base) {
	case USDHC1_BASE_ADDR:
		ret = 0;	/* unsupported */
		break;
	case USDHC2_BASE_ADDR:
		ret = 1;	/* always present: emmc */
		break;
	case USDHC3_BASE_ADDR:
		ret = 1;	/* always present: wifi */
		break;
	}

	return ret;
}

int board_mmc_init(bd_t *bis)
{
	int i;

	/*
	 * According to the board_mmc_init() the following map is done:
	 * (U-boot device node)    (Physical Port)
	 * mmc0                    USDHC1
	 * mmc1                    USDHC2
	 * mmc2                    USDHC3
	 */
	for (i = 0; i < CONFIG_SYS_FSL_USDHC_NUM; i++) {
		switch (i) {
		case 0:
			printf("currently unsupported\n");
			break;
		case 1:
			setup_iomux_usdhc2();
			usdhc_cfg[1].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
			break;
		case 2:
			setup_iomux_usdhc3();
			usdhc_cfg[2].sdhc_clk = mxc_get_clock(MXC_ESDHC2_CLK);
			break;
		default:
			printf("Warning: you configured more USDHC controllers"
				"(%d) than supported by the board\n", i + 1);
			return 0;
			}

			if (fsl_esdhc_initialize(bis, &usdhc_cfg[i]))
				printf("Warning: failed to initialize mmc dev %d\n", i);
	}

	return 0;
}

int check_mmc_autodetect(void)
{
	char *autodetect_str = getenv("mmcautodetect");

	if ((autodetect_str != NULL) &&
		(strcmp(autodetect_str, "yes") == 0)) {
		return 1;
	}

	return 0;
}

void board_late_mmc_env_init(void)
{
	char cmd[32];
	char mmcblk[32];
	u32 dev_no = mmc_get_env_devno();

	if (!check_mmc_autodetect())
		return;

	setenv_ulong("mmcdev", dev_no);

	/* Set mmcblk env */
	sprintf(mmcblk, "/dev/mmcblk%dp2 rootwait rw",
		mmc_map_to_kernel_blk(dev_no));
	setenv("mmcroot", mmcblk);

	sprintf(cmd, "mmc dev %d", dev_no);
	run_command(cmd, 0);
}
#endif

#ifdef CONFIG_MXC_EPDC
#ifdef CONFIG_SPLASH_SCREEN
extern int mmc_get_env_devno(void);
int setup_splash_img(void)
{
#ifdef CONFIG_SPLASH_IS_IN_MMC
	int mmc_dev = mmc_get_env_devno();
	ulong offset = CONFIG_SPLASH_IMG_OFFSET;
	ulong size = CONFIG_SPLASH_IMG_SIZE;
	ulong addr = 0;
	char *s = NULL;
	struct mmc *mmc = find_mmc_device(mmc_dev);
	uint blk_start, blk_cnt, n;

	s = getenv("splashimage");

	if (NULL == s) {
		puts("env splashimage not found!\n");
		return -1;
	}
	addr = simple_strtoul(s, NULL, 16);

	if (!mmc) {
		printf("MMC Device %d not found\n", mmc_dev);
		return -1;
	}

	if (mmc_init(mmc)) {
		puts("MMC init failed\n");
		return -1;
	}

	blk_start = ALIGN(offset, mmc->read_bl_len) / mmc->read_bl_len;
	blk_cnt = ALIGN(size, mmc->read_bl_len) / mmc->read_bl_len;
	n = mmc->block_dev.block_read(mmc_dev, blk_start,
				      blk_cnt, (u_char *)addr);
	flush_cache((ulong)addr, blk_cnt * mmc->read_bl_len);

	return (n == blk_cnt) ? 0 : -1;
#endif

	return 0;
}
#endif

vidinfo_t panel_info = {
	.vl_refresh = 85,
	.vl_col = 800,
	.vl_row = 600,
	.vl_pixclock = 26666667,
	.vl_left_margin = 8,
	.vl_right_margin = 100,
	.vl_upper_margin = 4,
	.vl_lower_margin = 8,
	.vl_hsync = 4,
	.vl_vsync = 1,
	.vl_sync = 0,
	.vl_mode = 0,
	.vl_flag = 0,
	.vl_bpix = 3,
	.cmap = 0,
};

struct epdc_timing_params panel_timings = {
	.vscan_holdoff = 4,
	.sdoed_width = 10,
	.sdoed_delay = 20,
	.sdoez_width = 10,
	.sdoez_delay = 20,
	.gdclk_hp_offs = 419,
	.gdsp_offs = 20,
	.gdoe_offs = 0,
	.gdclk_offs = 5,
	.num_ce = 1,
};

static void setup_epdc_power(void)
{
	/* Setup epdc voltage */

	/* EPDC_PWRSTAT - GPIO2[13] for PWR_GOOD status */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRSTAT__GPIO_2_13 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));
	gpio_direction_input(IMX_GPIO_NR(2, 13));

	/* EPDC_VCOM0 - GPIO2[3] for VCOM control */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_VCOM0__GPIO_2_3 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));

	/* Set as output */
	gpio_direction_output(IMX_GPIO_NR(2, 3), 1);

	/* EPDC_PWRWAKEUP - GPIO2[14] for EPD PMIC WAKEUP */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRWAKEUP__GPIO_2_14 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));
	/* Set as output */
	gpio_direction_output(IMX_GPIO_NR(2, 14), 1);

	/* EPDC_PWRCTRL0 - GPIO2[7] for EPD PWR CTL0 */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRCTRL0__GPIO_2_7 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));
	/* Set as output */
	gpio_direction_output(IMX_GPIO_NR(2, 7), 1);
}

int setup_waveform_file(void)
{
#ifdef CONFIG_WAVEFORM_FILE_IN_MMC
	int mmc_dev = mmc_get_env_devno();
	ulong offset = CONFIG_WAVEFORM_FILE_OFFSET;
	ulong size = CONFIG_WAVEFORM_FILE_SIZE;
	ulong addr = CONFIG_WAVEFORM_BUF_ADDR;
	struct mmc *mmc = find_mmc_device(mmc_dev);
	uint blk_start, blk_cnt, n;

	if (!mmc) {
		printf("MMC Device %d not found\n", mmc_dev);
		return -1;
	}

	if (mmc_init(mmc)) {
		puts("MMC init failed\n");
		return -1;
	}

	blk_start = ALIGN(offset, mmc->read_bl_len) / mmc->read_bl_len;
	blk_cnt = ALIGN(size, mmc->read_bl_len) / mmc->read_bl_len;
	n = mmc->block_dev.block_read(mmc_dev, blk_start,
				      blk_cnt, (u_char *)addr);
	flush_cache((ulong)addr, blk_cnt * mmc->read_bl_len);

	return (n == blk_cnt) ? 0 : -1;
#else
	return -1;
#endif
}

static void epdc_enable_pins(void)
{
	/* epdc iomux settings */
	imx_iomux_v3_setup_multiple_pads(epdc_enable_pads,
				ARRAY_SIZE(epdc_enable_pads));
}

static void epdc_disable_pins(void)
{
	/* Configure MUX settings for EPDC pins to GPIO  and drive to 0 */
	imx_iomux_v3_setup_multiple_pads(epdc_disable_pads,
				ARRAY_SIZE(epdc_disable_pads));
}

static void setup_epdc(void)
{
	unsigned int reg;
	struct mxc_ccm_reg *ccm_regs = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	/*** epdc Maxim PMIC settings ***/

	/* EPDC PWRSTAT - GPIO2[13] for PWR_GOOD status */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRSTAT__GPIO_2_13 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));

	/* EPDC VCOM0 - GPIO2[3] for VCOM control */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_VCOM0__GPIO_2_3 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));

	/* UART4 TXD - GPIO2[14] for EPD PMIC WAKEUP */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRWAKEUP__GPIO_2_14 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));

	/* EIM_A18 - GPIO2[7] for EPD PWR CTL0 */
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRCTRL0__GPIO_2_7 |
				MUX_PAD_CTRL(EPDC_PAD_CTRL));

	/*** Set pixel clock rates for EPDC ***/

	/* EPDC AXI clk from PFD_400M, set to 396/2 = 198MHz */
	reg = readl(&ccm_regs->chsccdr);
	reg &= ~0x3F000;
	reg |= (0x4 << 15) | (1 << 12);
	writel(reg, &ccm_regs->chsccdr);

	/* EPDC AXI clk enable */
	reg = readl(&ccm_regs->CCGR3);
	reg |= 0x0030;
	writel(reg, &ccm_regs->CCGR3);

	/* EPDC PIX clk from PFD_540M, set to 540/4/5 = 27MHz */
	reg = readl(&ccm_regs->cscdr2);
	reg &= ~0x03F000;
	reg |= (0x5 << 15) | (4 << 12);
	writel(reg, &ccm_regs->cscdr2);

	reg = readl(&ccm_regs->cbcmr);
	reg &= ~0x03800000;
	reg |= (0x3 << 23);
	writel(reg, &ccm_regs->cbcmr);

	/* EPDC PIX clk enable */
	reg = readl(&ccm_regs->CCGR3);
	reg |= 0x0C00;
	writel(reg, &ccm_regs->CCGR3);

	panel_info.epdc_data.working_buf_addr = CONFIG_WORKING_BUF_ADDR;
	panel_info.epdc_data.waveform_buf_addr = CONFIG_WAVEFORM_BUF_ADDR;

	panel_info.epdc_data.wv_modes.mode_init = 0;
	panel_info.epdc_data.wv_modes.mode_du = 1;
	panel_info.epdc_data.wv_modes.mode_gc4 = 3;
	panel_info.epdc_data.wv_modes.mode_gc8 = 2;
	panel_info.epdc_data.wv_modes.mode_gc16 = 2;
	panel_info.epdc_data.wv_modes.mode_gc32 = 2;

	panel_info.epdc_data.epdc_timings = panel_timings;

	setup_epdc_power();

	/* Assign fb_base */
	gd->fb_base = CONFIG_FB_BASE;
}

void epdc_power_on(void)
{
	unsigned int reg;
	struct gpio_regs *gpio_regs = (struct gpio_regs *)GPIO2_BASE_ADDR;

	/* Set EPD_PWR_CTL0 to high - enable EINK_VDD (3.15) */
	gpio_set_value(IMX_GPIO_NR(2, 7), 1);
	udelay(1000);

	/* Enable epdc signal pin */
	epdc_enable_pins();

	/* Set PMIC Wakeup to high - enable Display power */
	gpio_set_value(IMX_GPIO_NR(2, 14), 1);

	/* Wait for PWRGOOD == 1 */
	while (1) {
		reg = readl(&gpio_regs->gpio_psr);
		if (!(reg & (1 << 13)))
			break;

		udelay(100);
	}

	/* Enable VCOM */
	gpio_set_value(IMX_GPIO_NR(2, 3), 1);

	udelay(500);
}

void epdc_power_off(void)
{
	/* Set PMIC Wakeup to low - disable Display power */
	gpio_set_value(IMX_GPIO_NR(2, 14), 0);

	/* Disable VCOM */
	gpio_set_value(IMX_GPIO_NR(2, 3), 0);

	epdc_disable_pins();

	/* Set EPD_PWR_CTL0 to low - disable EINK_VDD (3.15) */
	gpio_set_value(IMX_GPIO_NR(2, 7), 0);
}
#endif

#ifdef CONFIG_SYS_I2C_MXC
/* set all switches APS in normal and PFM mode in standby */
static int setup_pmic_mode(int chip)
{
	unsigned char offset, i, switch_num, value;

	if (!chip) {
		/* pfuze100 */
		switch_num = 6;
		offset = 0x31;
	} else {
		/* pfuze200 */
		switch_num = 4;
		offset = 0x38;
	}

	value = 0xc;
	if (i2c_write(0x8, 0x23, 1, &value, 1)) {
		printf("Set SW1AB mode error!\n");
		return -1;
	}

	for (i = 0; i < switch_num - 1; i++) {
		if (i2c_write(0x8, offset + i * 7, 1, &value, 1)) {
			printf("Set switch%x mode error!\n", offset);
			return -1;
		}
	}

	return 0;
}

/* Fox Specific hacks */
static void setup_wifi(void)
{
	/* Turn on Wifi REG_ON */
	gpio_direction_output(IMX_GPIO_NR(2, 18), 1);

	/* Turn on BT REG_ON */
	gpio_direction_output(IMX_GPIO_NR(2, 19), 1);
}
/* End Fox Specific hack */

static int setup_pmic_voltages(void)
{
	unsigned char value, rev_id = 0;

	i2c_set_bus_num(0);
	if (!i2c_probe(0x8)) {
		if (i2c_read(0x8, 0, 1, &value, 1)) {
			printf("Read device ID error!\n");
			return -1;
		}
		if (i2c_read(0x8, 3, 1, &rev_id, 1)) {
			printf("Read Rev ID error!\n");
			return -1;
		}
		printf("Found PFUZE%s deviceid=%x,revid=%x\n",
			((value & 0xf) == 0) ? "100" : "200", value, rev_id);

		if (setup_pmic_mode(value & 0xf)) {
			printf("setup pmic mode error!\n");
			return -1;
		}
		/* set SW1AB staby volatage 0.975V */
		if (i2c_read(0x8, 0x21, 1, &value, 1)) {
			printf("Read SW1ABSTBY error!\n");
			return -1;
		}
		value &= ~0x3f;
		value |= 0x1b;
		if (i2c_write(0x8, 0x21, 1, &value, 1)) {
			printf("Set SW1ABSTBY error!\n");
			return -1;
		}

		/* set SW1AB/VDDARM step ramp up time from 16us to 4us/25mV */
		if (i2c_read(0x8, 0x24, 1, &value, 1)) {
			printf("Read SW1ABCONFIG error!\n");
			return -1;
		}
		value &= ~0xc0;
		value |= 0x40;
		if (i2c_write(0x8, 0x24, 1, &value, 1)) {
			printf("Set SW1ABCONFIG error!\n");
			return -1;
		}

		/* set SW1C staby volatage 0.975V */
		if (i2c_read(0x8, 0x2f, 1, &value, 1)) {
			printf("Read SW1CSTBY error!\n");
			return -1;
		}
		value &= ~0x3f;
		value |= 0x1b;
		if (i2c_write(0x8, 0x2f, 1, &value, 1)) {
			printf("Set SW1CSTBY error!\n");
			return -1;
		}

		/* set SW1C/VDDSOC step ramp up time to from 16us to 4us/25mV */
		if (i2c_read(0x8, 0x32, 1, &value, 1)) {
			printf("Read SW1CCONFIG error!\n");
			return -1;
		}
		value &= ~0xc0;
		value |= 0x40;
		if (i2c_write(0x8, 0x32, 1, &value, 1)) {
			printf("Set SW1CCONFIG error!\n");
			return -1;
		}
	}

	return 0;
}

#ifdef CONFIG_LDO_BYPASS_CHECK
void ldo_mode_set(int ldo_bypass)
{
	unsigned char value;
	int is_400M;

	/* swith to ldo_bypass mode */
	if (ldo_bypass) {
		prep_anatop_bypass();

		/* decrease VDDARM to 1.1V */
		if (i2c_read(0x8, 0x20, 1, &value, 1)) {
			printf("Read SW1AB error!\n");
			return;
		}
		value &= ~0x3f;
		value |= 0x20;
		if (i2c_write(0x8, 0x20, 1, &value, 1)) {
			printf("Set SW1AB error!\n");
			return;
		}
		/* increase VDDSOC to 1.3V */
		if (i2c_read(0x8, 0x2e, 1, &value, 1)) {
			printf("Read SW1C error!\n");
			return;
		}
		value &= ~0x3f;
		value |= 0x28;
		if (i2c_write(0x8, 0x2e, 1, &value, 1)) {
			printf("Set SW1C error!\n");
			return;
		}

		is_400M = set_anatop_bypass(0);

		/*
		 * MX6SL: VDDARM:1.175V@800M; VDDSOC:1.175V@800M
		 *        VDDARM:0.975V@400M; VDDSOC:1.175V@400M
		 */
		if (i2c_read(0x8, 0x20, 1, &value, 1)) {
			printf("Read SW1AB error!\n");
			return;
		}
		value &= ~0x3f;
		if (is_400M)
			value |= 0x1b;
		else
			value |= 0x23;
		if (i2c_write(0x8, 0x20, 1, &value, 1)) {
			printf("Set SW1AB error!\n");
			return;
		}

		/* decrease VDDSOC to 1.175V */
		if (i2c_read(0x8, 0x2e, 1, &value, 1)) {
			printf("Read SW1C error!\n");
			return;
		}
		value &= ~0x3f;
		value |= 0x23;
		if (i2c_write(0x8, 0x2e, 1, &value, 1)) {
			printf("Set SW1C error!\n");
			return;
		}

		finish_anatop_bypass();
		printf("switch to ldo_bypass mode!\n");
	}

}
#endif
#endif

int board_early_init_f(void)
{
	setup_iomux_uart1();

	return 0;
}

/*************************************************************************
 * get_board_serial() - setup to pass kernel serial number information
 *      return: alphanumeric containing the serial number.
 *************************************************************************/
const u8 *get_board_serial(void)
{
	return (u8 *) "0480100000000000";
}


/*************************************************************************
 * get_board_id16() - setup to pass kernel board revision information
 *      16-byte alphanumeric containing the board revision.
 *************************************************************************/
const u8 *get_board_id16(void)
{
	return (u8 *) "019110000000000000";
}

static void disable_watchdog_power_count_down_event(void)
{
    __raw_writew(~0x1, (WDOG1_BASE_ADDR+0x08));
}

static iomux_v3_cfg_t set_unused_pads[] = {
	//3V3
	MX6_PAD_KEY_COL0__GPIO_3_24,
	MX6_PAD_KEY_COL1__GPIO_3_26,
	MX6_PAD_SD1_DAT7__GPIO_5_10,
	MX6_PAD_SD1_DAT6__GPIO_5_7,
	MX6_PAD_EPDC_D10__GPIO_1_17,
	MX6_PAD_EPDC_D11__GPIO_1_18,
	MX6_PAD_EPDC_D12__GPIO_1_19,
	//1V8
	MX6_PAD_ECSPI2_SS0__GPIO_4_15,
	MX6_PAD_EPDC_PWRCOM__GPIO_2_11,
	MX6_PAD_FEC_RXD1__GPIO_4_18
};



static void set_unused_pins(void)
{
	iomux_v3_cfg_t *p = set_unused_pads;
	int i;
	/* Set PADCTRL to 0 for all IOMUX */
	for (i = 0; i < ARRAY_SIZE(set_unused_pads); i++) {
		*p &= ~MUX_PAD_CTRL_MASK;
		*p |= ((u64)0x3000 << MUX_PAD_CTRL_SHIFT); /* enable Pull and PD */
		p++;
	}
	imx_iomux_v3_setup_multiple_pads(set_unused_pads,
			ARRAY_SIZE(set_unused_pads));

	/* PMIC nFID pin */
//      imx_iomux_v3_setup_pad(MX6_PAD_FEC_TX_CLK__GPIO_4_21);

	udelay(100);

	//Set unused GPIOs to input
	gpio_direction_input(IMX_GPIO_NR(3,24));
	gpio_direction_input(IMX_GPIO_NR(3,26));
	gpio_direction_input(IMX_GPIO_NR(5,10));
	gpio_direction_input(IMX_GPIO_NR(5,7));
	gpio_direction_input(IMX_GPIO_NR(1,17));
	gpio_direction_input(IMX_GPIO_NR(1,18));
	gpio_direction_input(IMX_GPIO_NR(1,19));
	gpio_direction_input(IMX_GPIO_NR(4,15));
	gpio_direction_input(IMX_GPIO_NR(2,11));
	gpio_direction_input(IMX_GPIO_NR(4,18));
}

int board_init(void)
{
	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	setup_iomux();
//	set_unused_pins();

	//Keeper circuit soln.
	gpio_direction_output(IMX_GPIO_NR(1,22), 1);
	//GPIO(pin EPDC_SDCE1) for PMIC LPM - disabled by default
	gpio_direction_output(IMX_GPIO_NR(1,28), 0);
	//some delay to stabilise
	udelay(2000);

	disable_watchdog_power_count_down_event();

	return 0;
}

#define I2C1_PAD_CTRL   (PAD_CTL_LVE | PAD_CTL_SPEED_MED | \
	PAD_CTL_DSE_40ohm | PAD_CTL_HYS | \
	PAD_CTL_ODE | PAD_CTL_SRE_FAST)

#define I2C2_PAD_CTRL  (PAD_CTL_LVE | PAD_CTL_PUS_100K_PD | \
	PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | \
	PAD_CTL_ODE | PAD_CTL_SRE_SLOW)
/* I2C: I2C1 */
struct i2c_pads_info i2c1_info = {
	.sda = {
		.i2c_mode = MX6_PAD_I2C1_SDA__I2C1_SDA | MUX_PAD_CTRL(I2C1_PAD_CTRL),
		.gpio_mode = MX6_PAD_I2C1_SDA__GPIO_3_13 | MUX_PAD_CTRL(I2C1_PAD_CTRL),
		.gp = IMX_GPIO_NR(3, 13),
	},
	.scl = {
		.i2c_mode = MX6_PAD_I2C1_SCL__I2C1_SCL | MUX_PAD_CTRL(I2C1_PAD_CTRL),
		.gpio_mode = MX6_PAD_I2C1_SCL__GPIO_3_12 | MUX_PAD_CTRL(I2C1_PAD_CTRL),
		.gp = IMX_GPIO_NR(3, 12),
	},
};
struct i2c_pads_info i2c2_info = {
	.sda = {
		.i2c_mode = MX6_PAD_I2C2_SDA__I2C2_SDA | MUX_PAD_CTRL(I2C2_PAD_CTRL),
		.gpio_mode = MX6_PAD_I2C2_SDA__GPIO | MUX_PAD_CTRL(I2C2_PAD_CTRL),
		.gp = IMX_GPIO_NR(3, 15),
	},
	.scl = {
		.i2c_mode = MX6_PAD_I2C2_SCL__I2C2_SCL | MUX_PAD_CTRL(I2C2_PAD_CTRL),
		.gpio_mode = MX6_PAD_I2C2_SCL__GPIO | MUX_PAD_CTRL(I2C2_PAD_CTRL),
		.gp = IMX_GPIO_NR(3, 14),
	},
};

int board_late_init(void)
{

	/* pmic */
	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x34, &i2c1_info);
	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x35, &i2c1_info);
	setup_i2c(0, CONFIG_SYS_I2C_SPEED, 0x3c, &i2c1_info);

	i2c_set_bus_num(0);
	pmic_init();

	/* Temporary hack to enable wifi on Fox */
	setup_wifi();

	return 0;
}

u32 get_board_rev(void)
{
	return get_cpu_rev();
}

#ifdef CONFIG_FASTBOOT

void board_fastboot_setup(void)
{
	/* explicitly set MMC1 */
	setenv("fastboot_dev", "mmc"__stringify(CONFIG_MMC_DEFAULT_DEV));
}

void board_fastboot_load_partitions(void)
{
	struct fastboot_ptentry		ptable[CONFIG_NUM_PARTITIONS + 1];
	const struct partition_info_t	*part_info = partition_info_default;
	struct mmc			*mmc;
	block_dev_desc_t *dev_desc;
	int count = 0;
	int i;
	int mmc_no = CONFIG_MMC_DEFAULT_DEV;

	printf("flash target is MMC:%d\n", mmc_no);
	mmc = find_mmc_device(mmc_no);
	if (mmc && mmc_init(mmc))
		printf("MMC card init failed!\n");

	dev_desc = get_dev("mmc", mmc_no);
	if (NULL == dev_desc) {
		printf("** Block device MMC %d not supported\n", mmc_no);
		return;
	}

	memset((char *)ptable, 0, sizeof(ptable));
	/* fill gpt partition layout */
	for (i = 0; i < CONFIG_NUM_PARTITIONS; i++) {
		/* partition record */
		strcpy(ptable[count].name, part_info[i].name);
		ptable[count].start = part_info[i].address / dev_desc->blksz;
		if (part_info[i].size != PARTITION_FILL_SPACE)
			ptable[count].length = part_info[i].size / dev_desc->blksz;
		else
			ptable[count].length = dev_desc->lba - 128 - ptable[count].start;
		ptable[count].partition_id = (part_info[i].partition == CONFIG_BOOT_FROM_PARTITION)?1:0;

		/* add to fastboot */
		fastboot_flash_add_ptn(&ptable[count]);
		/* next */
		++count;
	}

	/*check it out */
	fastboot_flash_dump_ptn();

}

#ifdef CONFIG_ANDROID_RECOVERY
int check_recovery_cmd_file(void)
{
    return recovery_check_and_clean_flag();
}

void board_recovery_setup(void)
{
	int bootdev = get_boot_device();

	/*current uboot BSP only supports USDHC2*/
	switch (bootdev) {
#if defined(CONFIG_FASTBOOT_STORAGE_MMC)
	case SD1_BOOT:
	case MMC1_BOOT:
		if (!getenv("bootcmd_android_recovery"))
			setenv("bootcmd_android_recovery",
					"booti mmc0 recovery");
		break;
	case SD2_BOOT:
	case MMC2_BOOT:
		if (!getenv("bootcmd_android_recovery"))
			setenv("bootcmd_android_recovery",
					"booti mmc1 recovery");
		break;
	case SD3_BOOT:
	case MMC3_BOOT:
		if (!getenv("bootcmd_android_recovery"))
			setenv("bootcmd_android_recovery",
					"booti mmc2 recovery");
		break;
#endif /*CONFIG_FASTBOOT_STORAGE_MMC*/
	default:
		printf("Unsupported bootup device for recovery: dev: %d\n",
			bootdev);
		return;
	}

	printf("setup env for recovery..\n");
	setenv("bootcmd", "run bootcmd_android_recovery");
}

#endif /*CONFIG_ANDROID_RECOVERY*/

#endif /*CONFIG_FASTBOOT*/


#ifdef CONFIG_MXC_KPD
#define MX6SL_KEYPAD_CTRL	(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_PUE | \
				PAD_CTL_PUS_100K_UP | PAD_CTL_DSE_120ohm)

iomux_v3_cfg_t const mxc_kpd_pads[] = {
	(MX6_PAD_KEY_COL0__KPP_COL_0 | MUX_PAD_CTRL(NO_PAD_CTRL)),
	(MX6_PAD_KEY_COL1__KPP_COL_1 | MUX_PAD_CTRL(NO_PAD_CTRL)),
	(MX6_PAD_KEY_COL2__KPP_COL_2 | MUX_PAD_CTRL(NO_PAD_CTRL)),
	(MX6_PAD_KEY_COL3__KPP_COL_3 | MUX_PAD_CTRL(NO_PAD_CTRL)),

	(MX6_PAD_KEY_ROW0__KPP_ROW_0 | MUX_PAD_CTRL(MX6SL_KEYPAD_CTRL)),
	(MX6_PAD_KEY_ROW1__KPP_ROW_1 | MUX_PAD_CTRL(MX6SL_KEYPAD_CTRL)),
	(MX6_PAD_KEY_ROW2__KPP_ROW_2 | MUX_PAD_CTRL(MX6SL_KEYPAD_CTRL)),
	(MX6_PAD_KEY_ROW3__KPP_ROW_3 | MUX_PAD_CTRL(MX6SL_KEYPAD_CTRL)),
};
int setup_mxc_kpd(void)
{
	imx_iomux_v3_setup_multiple_pads(mxc_kpd_pads,
			ARRAY_SIZE(mxc_kpd_pads));

	return 0;
}
#endif /*CONFIG_MXC_KPD*/

#ifdef CONFIG_IMX_UDC
iomux_v3_cfg_t const otg_udc_pads[] = {
	(MX6_PAD_EPDC_PWRCOM__ANATOP_USBOTG1_ID | MUX_PAD_CTRL(NO_PAD_CTRL)),
};
void udc_pins_setting(void)
{
	imx_iomux_v3_setup_multiple_pads(otg_udc_pads,
			ARRAY_SIZE(otg_udc_pads));
}
#endif /*CONFIG_IMX_UDC*/

int checkboard(void)
{
	puts("Board: MX6SL_WARIO\n");

	return 0;
}

#ifdef CONFIG_USB_EHCI_MX6
iomux_v3_cfg_t const usb_otg1_pads[] = {
	MX6_PAD_KEY_COL4__USB_USBOTG1_PWR | MUX_PAD_CTRL(NO_PAD_CTRL),
	MX6_PAD_EPDC_PWRCOM__ANATOP_USBOTG1_ID | MUX_PAD_CTRL(NO_PAD_CTRL)
};

iomux_v3_cfg_t const usb_otg2_pads[] = {
	MX6_PAD_KEY_COL5__USB_USBOTG2_PWR | MUX_PAD_CTRL(NO_PAD_CTRL),
};

int board_ehci_hcd_init(int port)
{
	switch (port) {
	case 0:
		imx_iomux_v3_setup_multiple_pads(usb_otg1_pads,
			ARRAY_SIZE(usb_otg1_pads));
		break;
	case 1:
		imx_iomux_v3_setup_multiple_pads(usb_otg2_pads,
			ARRAY_SIZE(usb_otg2_pads));
		break;
	default:
		printf("MXC USB port %d not yet supported\n", port);
		return 1;
	}
	return 0;
}
#endif
