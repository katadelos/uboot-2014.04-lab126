/*
 * Copyright (C) 2014 Amazon
 *
 * Pinmux settings
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
#include <i2c.h>
#include <asm/imx-common/mxc_i2c.h>

//#include "wario.h"


#define PAD_PUS0_CTRL        (PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS)
#define PAD_PUS0_LVE_CTRL    (PAD_PUS0_CTRL | PAD_CTL_LVE)

#define PAD_100DOWN_CTRL (PAD_PUS0_LVE_CTRL | PAD_CTL_PUS_100K_DOWN)
#define PAD_100PD_CTRL   (PAD_PUS0_LVE_CTRL | PAD_CTL_PUS_100K_PD  )
#define PAD_100UP_CTRL   (PAD_PUS0_LVE_CTRL | PAD_CTL_PUS_100K_UP  )
#define PAD_22UP_CTRL    (PAD_PUS0_LVE_CTRL | PAD_CTL_PUS_22K_UP  )


/*
 * UART
 */
#define UART1_RXD_CTRL    (PAD_100UP_CTRL)
#define UART1_TXD_CTRL    (PAD_100DOWN_CTRL)


static iomux_v3_cfg_t const uart1_pads[] = {
	MX6_PAD_UART1_TXD__UART1_TXD | MUX_PAD_CTRL(UART1_TXD_CTRL),
	MX6_PAD_UART1_RXD__UART1_RXD | MUX_PAD_CTRL(UART1_RXD_CTRL),
};

static iomux_v3_cfg_t const uart2_pads[] = {
	MX6_PAD_LCD_ENABLE__UART2_RXD | MUX_PAD_CTRL(UART1_RXD_CTRL),
};

static iomux_v3_cfg_t const uart3_pads[] = {
	MX6_PAD_EPDC_BDR1__UART3_CTS | MUX_PAD_CTRL(UART1_RXD_CTRL),
	MX6_PAD_EPDC_BDR0__UART3_RTS | MUX_PAD_CTRL(UART1_RXD_CTRL),
	MX6_PAD_EPDC_VCOM0__UART3_RXD | MUX_PAD_CTRL(UART1_RXD_CTRL),
	MX6_PAD_EPDC_VCOM1__UART3_TXD | MUX_PAD_CTRL(UART1_TXD_CTRL),
};



void setup_iomux_uart1(void)
{
	imx_iomux_v3_setup_multiple_pads(uart1_pads, ARRAY_SIZE(uart1_pads));
}

void setup_iomux_uart2(void)
{
	imx_iomux_v3_setup_multiple_pads(uart2_pads, ARRAY_SIZE(uart2_pads));
}

void setup_iomux_uart3(void)
{
	imx_iomux_v3_setup_multiple_pads(uart3_pads, ARRAY_SIZE(uart3_pads));
}


/*
 * USDHC
 */
#define USDHC_PAD_CTRL   (PAD_PUS0_LVE_CTRL | PAD_CTL_PUS_22K_UP | PAD_CTL_SRE_FAST)

static iomux_v3_cfg_t const usdhc1_pads[] = {
	/* 4 bit */
	MX6_PAD_SD1_CLK__USDHC1_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_CMD__USDHC1_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DAT0__USDHC1_DAT0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DAT1__USDHC1_DAT1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DAT2__USDHC1_DAT2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD1_DAT3__USDHC1_DAT3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc2_pads[] = {
	/* 8 bit eMMC */
	MX6_PAD_SD2_CLK__USDHC2_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_CMD__USDHC2_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT0__USDHC2_DAT0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT1__USDHC2_DAT1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT2__USDHC2_DAT2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT3__USDHC2_DAT3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT4__USDHC2_DAT4 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT5__USDHC2_DAT5 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT6__USDHC2_DAT6 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_DAT7__USDHC2_DAT7 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD2_RST__USDHC2_RST | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

static iomux_v3_cfg_t const usdhc3_pads[] = {
	/* 4 bit */
	MX6_PAD_SD3_CLK__USDHC3_CLK | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_CMD__USDHC3_CMD | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT0__USDHC3_DAT0 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT1__USDHC3_DAT1 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT2__USDHC3_DAT2 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
	MX6_PAD_SD3_DAT3__USDHC3_DAT3 | MUX_PAD_CTRL(USDHC_PAD_CTRL),
};

void setup_iomux_usdhc1(void)
{
	imx_iomux_v3_setup_multiple_pads(usdhc1_pads, ARRAY_SIZE(usdhc1_pads));
}

void setup_iomux_usdhc2(void)
{
	imx_iomux_v3_setup_multiple_pads(usdhc2_pads, ARRAY_SIZE(usdhc2_pads));
}

void setup_iomux_usdhc3(void)
{
	imx_iomux_v3_setup_multiple_pads(usdhc3_pads, ARRAY_SIZE(usdhc3_pads));
}


void setup_iomux_usdhc(void)
{
	setup_iomux_usdhc2();
}


/*
 * I2C
 */
#define I2C1_PAD_CTRL    (PAD_PUS0_LVE_CTRL  | PAD_CTL_ODE | PAD_CTL_SRE_FAST)
#define I2C2_PAD_CTRL    (0x404108a8)


static iomux_v3_cfg_t const i2c1_pads[] = {
	MX6_PAD_I2C1_SDA__I2C1_SDA | MUX_PAD_CTRL(I2C1_PAD_CTRL),
	MX6_PAD_I2C1_SCL__I2C1_SCL | MUX_PAD_CTRL(I2C1_PAD_CTRL),
};

static iomux_v3_cfg_t const i2c2_pads[] = {
	MX6_PAD_I2C2_SDA__I2C2_SDA | MUX_PAD_CTRL(I2C2_PAD_CTRL),
	MX6_PAD_I2C2_SCL__I2C2_SCL | MUX_PAD_CTRL(I2C2_PAD_CTRL),
};

static iomux_v3_cfg_t const i2c3_pads[] = {
	MX6_PAD_EPDC_SDCE2__I2C3_SCL | MUX_PAD_CTRL(I2C2_PAD_CTRL),
	MX6_PAD_EPDC_SDCE3__I2C3_SDA | MUX_PAD_CTRL(I2C2_PAD_CTRL),
};


void setup_iomux_i2c1(void)
{
	imx_iomux_v3_setup_multiple_pads(i2c1_pads, ARRAY_SIZE(i2c1_pads));
}

void setup_iomux_i2c2(void)
{
	imx_iomux_v3_setup_multiple_pads(i2c2_pads, ARRAY_SIZE(i2c2_pads));
}

void setup_iomux_i2c3(void)
{
	imx_iomux_v3_setup_multiple_pads(i2c3_pads, ARRAY_SIZE(i2c3_pads));
}


/*
 * GPIO

 PAD_CTL_PKE
 */
#define GPIO1_PD_PAD_CTRL        (PAD_100PD_CTRL)
#define GPIO1_PD_PKE_PAD_CTRL    (PAD_100PD_CTRL | PAD_CTL_PKE)
#define GPIO1_DOWN_PAD_CTRL      (PAD_100DOWN_CTRL)
#define GPIO1_DOWN_ODE_PAD_CTRL  (PAD_100DOWN_CTRL | PAD_CTL_ODE)
#define GPIO1_UP_PAD_CTRL        (PAD_100UP_CTRL)
#define GPIO1_UP_ODE_PAD_CTRL    (PAD_100UP_CTRL | PAD_CTL_ODE)

static iomux_v3_cfg_t const gpio1_pads[] = {
	MX6_PAD_EPDC_SDCE1__GPIO_1_28 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
};


#define GPIO_47UP_PAD_CTRL (PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_120ohm | PAD_CTL_SRE_SLOW | PAD_CTL_ODE)
#define GPIO_LCD_CLK_CTRL  (PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW | PAD_CTL_ODE)
#define GPIO_EPDC_PWRCTRL1 (PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN |PAD_CTL_DSE_60ohm | PAD_CTL_SRE_SLOW)

static iomux_v3_cfg_t const gpio2_pads[] = {
	MX6_PAD_EPDC_PWRCTRL0__GPIO_2_7 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_EPDC_PWRCTRL1__GPIO_2_8 | MUX_PAD_CTRL(GPIO_EPDC_PWRCTRL1),
	MX6_PAD_EPDC_PWRSTAT__GPIO_2_13 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_LCD_CLK__GPIO_2_15 | MUX_PAD_CTRL(GPIO_LCD_CLK_CTRL),
	MX6_PAD_LCD_ENABLE__GPIO_2_16 | MUX_PAD_CTRL(GPIO1_UP_PAD_CTRL),
	MX6_PAD_LCD_HSYNC__GPIO_2_17 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_LCD_VSYNC__GPIO_2_18 | MUX_PAD_CTRL(GPIO1_UP_PAD_CTRL),
	MX6_PAD_LCD_RESET__GPIO_2_19 | MUX_PAD_CTRL(GPIO1_UP_PAD_CTRL),
	MX6_PAD_LCD_DAT0__GPIO_2_20 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT1__GPIO_2_21 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT2__GPIO_2_22 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT3__GPIO_2_23 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT4__GPIO_2_24 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT5__GPIO_2_25 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT6__GPIO_2_26 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT7__GPIO_2_27 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT8__GPIO_2_28 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT9__GPIO_2_29 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT10__GPIO_2_30 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT11__GPIO_2_31 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
};


#define GPIO_HSIC_DAT_CTRL    (PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_40ohm)
#define GPIO_KEY_COL0_CTRL    (PAD_CTL_PUS_100K_PD | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_SRE_SLOW | PAD_CTL_ODE)
#define GPIO_KEY_ROW2_CTRL    (PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW)

static iomux_v3_cfg_t const gpio3_pads[] = {
	MX6_PAD_LCD_DAT12__GPIO_3_0 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT13__GPIO_3_1 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT14__GPIO_3_2 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT15__GPIO_3_3 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT20__GPIO_3_8 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT21__GPIO_3_9 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT22__GPIO_3_10 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_HSIC_DAT__GPIO_3_19 | MUX_PAD_CTRL(GPIO_HSIC_DAT_CTRL),
	MX6_PAD_HSIC_STROBE__GPIO_3_20 | MUX_PAD_CTRL(GPIO_HSIC_DAT_CTRL),
	MX6_PAD_LCD_DAT23__GPIO_3_11 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_KEY_ROW0__GPIO_3_25 | MUX_PAD_CTRL(PAD_22UP_CTRL),
	MX6_PAD_KEY_ROW1__GPIO_3_27 | MUX_PAD_CTRL(PAD_22UP_CTRL),
	MX6_PAD_KEY_ROW2__GPIO_3_29 | MUX_PAD_CTRL(PAD_22UP_CTRL),
	MX6_PAD_KEY_ROW3__GPIO_3_31 | MUX_PAD_CTRL(PAD_22UP_CTRL),
};


#define GPIO_KEY_COL4_CTRL    (PAD_CTL_LVE | PAD_CTL_PUS_100K_PD | PAD_CTL_PUE | PAD_CTL_DSE_60ohm | PAD_CTL_SRE_SLOW)
#define GPIO_FEC_RXD0_CTRL    (PAD_CTL_LVE | PAD_CTL_PUS_100K_PD | PAD_CTL_SPEED_MED | PAD_CTL_DSE_34ohm | PAD_CTL_HYS | PAD_CTL_SRE_SLOW)
#define GPIO_FEC_TXD0_CTRL    (PAD_CTL_PUS_100K_PD | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | PAD_CTL_SRE_SLOW)
#define GPIO_ECSPI2_MISO_CTRL (PAD_PUS0_LVE_CTRL | PAD_CTL_PUS_22K_UP)

static iomux_v3_cfg_t const gpio4_pads[] = {
	MX6_PAD_KEY_ROW4__GPIO_4_1 | MUX_PAD_CTRL(PAD_22UP_CTRL),
	MX6_PAD_KEY_ROW5__GPIO_4_3 | MUX_PAD_CTRL(PAD_22UP_CTRL),
	MX6_PAD_KEY_ROW6__GPIO_4_5 | MUX_PAD_CTRL(PAD_22UP_CTRL),
	MX6_PAD_FEC_RXD1__GPIO_4_18 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_FEC_RX_ER__GPIO_4_19 | MUX_PAD_CTRL(GPIO1_UP_PAD_CTRL),
	MX6_PAD_FEC_MDIO__GPIO_4_20 | MUX_PAD_CTRL(GPIO1_UP_PAD_CTRL),
	MX6_PAD_FEC_TX_EN__GPIO_4_22 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_FEC_MDC__GPIO_4_23 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_FEC_CRS_DV__GPIO_4_25 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_FEC_REF_CLK__GPIO_4_26 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
};

static iomux_v3_cfg_t const gpio5_pads[] = {
	MX6_PAD_SD1_DAT7__GPIO_5_10 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_SD1_DAT6__GPIO_5_7 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
};


void setup_iomux_gpio1(void)
{
	imx_iomux_v3_setup_multiple_pads(gpio1_pads, ARRAY_SIZE(gpio1_pads));
}

void setup_iomux_gpio2(void)
{
	imx_iomux_v3_setup_multiple_pads(gpio2_pads, ARRAY_SIZE(gpio2_pads));
}

void setup_iomux_gpio3(void)
{
	imx_iomux_v3_setup_multiple_pads(gpio3_pads, ARRAY_SIZE(gpio3_pads));
}

void setup_iomux_gpio4(void)
{
	imx_iomux_v3_setup_multiple_pads(gpio4_pads, ARRAY_SIZE(gpio4_pads));
}

void setup_iomux_gpio5(void)
{
	imx_iomux_v3_setup_multiple_pads(gpio5_pads, ARRAY_SIZE(gpio5_pads));
}


/*
 * WDOG
 */
#define WDOG_PAD_CTRL    (PAD_100PD_CTRL | PAD_CTL_ODE)

static iomux_v3_cfg_t const wdog_pads[] = {
	MX6_PAD_WDOG_B__WDOG_B | MUX_PAD_CTRL(WDOG_PAD_CTRL),
};

void setup_iomux_wdog(void)
{
	imx_iomux_v3_setup_multiple_pads(wdog_pads, ARRAY_SIZE(wdog_pads));
}




/* setup iomux */
void setup_iomux(void)
{
	setup_iomux_gpio1();
	setup_iomux_gpio2();
	setup_iomux_gpio3();
	setup_iomux_gpio4();

	setup_iomux_uart2();
	setup_iomux_uart3();

	setup_iomux_i2c1();
	setup_iomux_i2c2();

	setup_iomux_wdog();
}
/* unused
gpio1 bank
	MX6_PAD_EPDC_D8__GPIO_1_15 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_EPDC_D9__GPIO_1_16 | MUX_PAD_CTRL(GPIO1_UP_ODE_PAD_CTRL),
	MX6_PAD_EPDC_D10__GPIO_1_17 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_EPDC_D11__GPIO_1_18 | MUX_PAD_CTRL(GPIO1_DOWN_ODE_PAD_CTRL),
	MX6_PAD_EPDC_D12__GPIO_1_19 | MUX_PAD_CTRL(GPIO1_UP_ODE_PAD_CTRL),
	MX6_PAD_EPDC_D13__GPIO_1_20 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_EPDC_D14__GPIO_1_21 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_EPDC_D15__GPIO_1_22 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),

gpio2 bank
-	MX6_PAD_EPDC_GDRL__GPIO_2_1 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
-	MX6_PAD_EPDC_PWRCTRL2__GPIO_2_9 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
-	MX6_PAD_EPDC_PWRCTRL3__GPIO_2_10 | MUX_PAD_CTRL(GPIO_47UP_PAD_CTRL),
-	MX6_PAD_EPDC_PWRINT__GPIO_2_12 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
-	MX6_PAD_EPDC_PWRWAKEUP__GPIO_2_14 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),

gpio3 bank
	MX6_PAD_LCD_DAT16__GPIO_3_4 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT17__GPIO_3_5 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT18__GPIO_3_6 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_LCD_DAT19__GPIO_3_7 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_REF_CLK_24M__GPIO_3_21 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_REF_CLK_32K__GPIO_3_22 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_KEY_COL0__GPIO_3_24 | MUX_PAD_CTRL(GPIO_KEY_COL0_CTRL),
	MX6_PAD_KEY_COL1__GPIO_3_26 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_KEY_COL2__GPIO_3_28 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_KEY_COL3__GPIO_3_30 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),

gpio4 bank
	MX6_PAD_KEY_COL4__GPIO_4_0 | MUX_PAD_CTRL(GPIO_KEY_COL4_CTRL),
	MX6_PAD_KEY_COL5__GPIO_4_2 | MUX_PAD_CTRL(GPIO1_PD_PKE_PAD_CTRL),
	MX6_PAD_KEY_COL6__GPIO_4_4 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_KEY_COL7__GPIO_4_6 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_KEY_ROW7__GPIO_4_7 | MUX_PAD_CTRL(GPIO1_PD_PAD_CTRL),
	MX6_PAD_ECSPI2_SCLK__GPIO_4_12 | MUX_PAD_CTRL(GPIO1_UP_PAD_CTRL),
	MX6_PAD_ECSPI2_MOSI__GPIO_4_13 | MUX_PAD_CTRL(GPIO1_UP_PAD_CTRL),
	MX6_PAD_ECSPI2_MISO__GPIO_4_14 | MUX_PAD_CTRL(GPIO_ECSPI2_MISO_CTRL),
	MX6_PAD_ECSPI2_SS0__GPIO_4_15 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_FEC_TXD1__GPIO_4_16 | MUX_PAD_CTRL(GPIO1_DOWN_PAD_CTRL),
	MX6_PAD_FEC_RXD0__GPIO_4_17 | MUX_PAD_CTRL(GPIO_FEC_RXD0_CTRL),
	MX6_PAD_FEC_TXD0__GPIO_4_24 | MUX_PAD_CTRL(GPIO1_PD_PAD_CTRL),

*/

