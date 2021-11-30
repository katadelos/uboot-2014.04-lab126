/*
 * Copyright (C) 2014-2015 Amazon.com, Inc.
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

#include <boardid.h>

extern const unsigned char *get_board_id16(void);

/*
 * UART
 */
void setup_iomux_uart1(void)
{
	imx_iomux_v3_setup_pad( MX6_PAD_UART1_TXD__UART1_TXD | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_PUS_100K_UP) );
	imx_iomux_v3_setup_pad( MX6_PAD_UART1_RXD__UART1_RXD | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_PUS_100K_UP) );
}

void setup_iomux_uart2(void)
{
	//imx_iomux_v3_setup_pad(MX6_PAD_LCD_ENABLE__UART2_RXD | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_UP) );
}

void setup_iomux_uart3(void)
{
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_BDR1__UART3_CTS | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_UP) );
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_BDR0__UART3_RTS | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_UP) );
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_VCOM0__UART3_RXD | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_UP) );
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_VCOM1__UART3_TXD | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN) );
}

void setup_iomux_uart3_lpm(void)
{
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_BDR1__GPIO_2_6 | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN) );
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_BDR0__GPIO_2_5 | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN) );
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_VCOM0__GPIO_2_3 | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN) );
	imx_iomux_v3_setup_pad( MX6_PAD_EPDC_VCOM1__GPIO_2_4 | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN) );
}

void setup_iomux_uart4(void)
{
	imx_iomux_v3_setup_pad( MX6_PAD_SD1_DAT4__UART4_RXD | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_PUS_100K_UP) );
	imx_iomux_v3_setup_pad( MX6_PAD_SD1_DAT5__UART4_TXD | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_PUS_100K_UP) );
}
void setup_iomux_uart4_lpm(void)
{
        imx_iomux_v3_setup_pad( MX6_PAD_SD1_DAT4__GPIO_5_12 | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_PUS_100K_DOWN) );
        imx_iomux_v3_setup_pad( MX6_PAD_SD1_DAT5__GPIO_5_9 | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_PUS_100K_DOWN) );
        gpio_direction_input( IMX_GPIO_NR(5,12));
        gpio_direction_input( IMX_GPIO_NR(5,9));
}
/*
 * USDHC
 */
void setup_iomux_usdhc1(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_CLK__USDHC1_CLK | MUX_PAD_CTRL(PAD_CTL_LVE |  PAD_CTL_SPEED_HIGH | PAD_CTL_DSE_34ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_CMD__USDHC1_CMD | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT0__USDHC1_DAT0 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT1__USDHC1_DAT1 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT2__USDHC1_DAT2 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT3__USDHC1_DAT3 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));	
}

void setup_iomux_usdhc1_lpm(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_CLK__GPIO_5_15 | MUX_PAD_CTRL(PAD_CTL_LVE |  PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_34ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_CMD__GPIO_5_14 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT0__GPIO_5_11 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT1__GPIO_5_8 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT2__GPIO_5_13 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT3__GPIO_5_6 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_100K_DOWN | PAD_CTL_DSE_48ohm | PAD_CTL_SRE_FAST));	
}

void setup_iomux_usdhc2(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_CLK__USDHC2_CLK | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_CMD__USDHC2_CMD | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT0__USDHC2_DAT0 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT1__USDHC2_DAT1 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT2__USDHC2_DAT2 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT3__USDHC2_DAT3 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT4__USDHC2_DAT4 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT5__USDHC2_DAT5 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT6__USDHC2_DAT6 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT7__USDHC2_DAT7 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_RST__USDHC2_RST | MUX_PAD_CTRL(PAD_CTL_LVE |  PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
}

void setup_iomux_usdhc2_lpm(void)
{

	imx_iomux_v3_setup_pad(MX6_PAD_SD2_CLK__GPIO_5_5   | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_CMD__GPIO_5_4   | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT0__GPIO_5_1  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT1__GPIO_4_30 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT2__GPIO_5_3  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT3__GPIO_4_28 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT4__GPIO_5_2  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT5__GPIO_4_31 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT6__GPIO_4_29 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_DAT7__GPIO_5_0  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD2_RST__GPIO_4_27  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_47K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_80ohm | PAD_CTL_SRE_FAST));

	gpio_direction_input( IMX_GPIO_NR(5,0));
	gpio_direction_input( IMX_GPIO_NR(5,1));
	gpio_direction_input( IMX_GPIO_NR(5,2));
	gpio_direction_input( IMX_GPIO_NR(5,3));
	gpio_direction_input( IMX_GPIO_NR(5,4));
	gpio_direction_input( IMX_GPIO_NR(5,5));
	gpio_direction_input( IMX_GPIO_NR(4,27));
	gpio_direction_input( IMX_GPIO_NR(4,28));
	gpio_direction_input( IMX_GPIO_NR(4,29));
	gpio_direction_input( IMX_GPIO_NR(4,30));
	gpio_direction_input( IMX_GPIO_NR(4,31));
}

void setup_iomux_usdhc3(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_SD3_CLK__USDHC3_CLK | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD3_CMD__USDHC3_CMD | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD3_DAT0__USDHC3_DAT0 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD3_DAT1__USDHC3_DAT1 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD3_DAT2__USDHC3_DAT2 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_SD3_DAT3__USDHC3_DAT3 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_PUS_22K_UP | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_HYS | PAD_CTL_SRE_FAST));
}



/*
 * I2C
 */
void setup_iomux_i2c1(void)
{
	unsigned int reg;
    	struct mxc_ccm_reg *ccm_regs = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	imx_iomux_v3_setup_pad(MX6_PAD_I2C1_SDA__I2C1_SDA | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_ODE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_FAST));
	imx_iomux_v3_setup_pad(MX6_PAD_I2C1_SCL__I2C1_SCL | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_ODE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_FAST));

    	/* Enable i2c1 clock */
	reg = readl(&ccm_regs->CCGR2);
    	reg |= 0xC0;
	writel(reg, &ccm_regs->CCGR2);

}

void setup_iomux_i2c2(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_I2C2_SDA__I2C2_SDA | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_ODE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_48ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_I2C2_SCL__I2C2_SCL | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_ODE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_48ohm));
}

void setup_iomux_i2c3(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_SDCE2__I2C3_SCL | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_ODE ));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_SDCE3__I2C3_SDA | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_ODE ));
}


/*
 * GPIO
 */
void setup_iomux_gpio1(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_AUD_RXFS__GPIO_1_0 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_AUD_RXC__GPIO_1_1 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_AUD_RXD__GPIO_1_2 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_AUD_TXFS__GPIO_1_4 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_AUD_TXD__GPIO_1_5 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_AUD_MCLK__GPIO_1_6 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D8__GPIO_1_15 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D9__GPIO_1_16 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D10__GPIO_1_17 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D11__GPIO_1_18 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D12__GPIO_1_19 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D13__GPIO_1_20 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D14__GPIO_1_21 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_D15__GPIO_1_22 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_SDSHR__GPIO_1_26 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_SDCE1__GPIO_1_28 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_AUD_TXC__GPIO_1_3 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
																					   
}

void setup_iomux_gpio2(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_GDRL__GPIO_2_1 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRCTRL0__GPIO_2_7 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRCTRL1__GPIO_2_8 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRCTRL2__GPIO_2_9 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRCTRL3__GPIO_2_10 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRINT__GPIO_2_12 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRSTAT__GPIO_2_13 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRWAKEUP__GPIO_2_14 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_EPDC_PWRCOM__GPIO_2_11 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_CLK__GPIO_2_15 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); // WIFI ENABLE
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_ENABLE__GPIO_2_16 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_HSYNC__GPIO_2_17 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_VSYNC__GPIO_2_18 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_RESET__GPIO_2_19 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //BT ENABLE
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT0__GPIO_2_20 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT1__GPIO_2_21 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT2__GPIO_2_22 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT7__GPIO_2_27 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT8__GPIO_2_28 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT9__GPIO_2_29 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT10__GPIO_2_30 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT11__GPIO_2_31 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
}

void setup_iomux_gpio3(void)
{
        imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT12__GPIO_3_0 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT13__GPIO_3_1 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT14__GPIO_3_2 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT15__GPIO_3_3 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT20__GPIO_3_8 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT21__GPIO_3_9 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT22__GPIO_3_10 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT23__GPIO_3_11 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_HSIC_DAT__GPIO_3_19 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_HSIC_STROBE__GPIO_3_20 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW0__GPIO_3_25 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //Fitipower Enable, PM_EPD_EN
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW1__GPIO_3_27 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //fitipower enable, PM_EPD_ENOP
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW2__GPIO_3_29 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_PUS_100K_DOWN | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //BT_HOST_WAKE
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW3__GPIO_3_31 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_PUS_100K_DOWN | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //WIFI_WAKE_ON_LAN_B
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_COL0__GPIO_3_24 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //Zforce bootloader tirgger
	
}

void setup_iomux_gpio4(void)
{
        imx_iomux_v3_setup_pad(MX6_PAD_KEY_COL4__GPIO_4_0 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));//EPDC_XON
        imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW4__GPIO_4_1 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //MUX_INT_N
        imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW5__GPIO_4_3 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));//TOUCH_DATA_READY
        
	imx_iomux_v3_setup_pad(MX6_PAD_KEY_ROW6__GPIO_4_5 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_ODE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //TOUCH_RST_N
	
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_RXD1__GPIO_4_18 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_TXD1__GPIO_4_16 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));//HALL_INT_N
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_RXD0__GPIO_4_17 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_PUS_100K_DOWN | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //USB1_CONN
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_RX_ER__GPIO_4_19 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_MDIO__GPIO_4_20 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //PMIC_INT_N
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_TX_EN__GPIO_4_22 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_MDC__GPIO_4_23 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_CRS_DV__GPIO_4_25 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
        imx_iomux_v3_setup_pad(MX6_PAD_FEC_REF_CLK__GPIO_4_26 | MUX_PAD_CTRL(PAD_CTL_HYS | PAD_CTL_PKE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
}
void setup_iomux_gpio4_lpm(void)
{
        imx_iomux_v3_setup_pad( MX6_PAD_KEY_ROW6__GPIO_4_5 | MUX_PAD_CTRL(PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm | PAD_CTL_SRE_SLOW  | PAD_CTL_HYS | PAD_CTL_PUS_100K_DOWN) );
        gpio_direction_input( IMX_GPIO_NR(4,5));
}

void setup_iomux_gpio5(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT7__GPIO_5_10 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm)); //BT_DEV_WAKE
	imx_iomux_v3_setup_pad(MX6_PAD_SD1_DAT6__GPIO_5_7 | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));  //WIFI_GPIO_1
						    
}


/*
 * WDOG
 */
void setup_iomux_wdog(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_WDOG_B__WDOG_B | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS | PAD_CTL_ODE | PAD_CTL_SPEED_MED | PAD_CTL_DSE_40ohm));
}

/*
 *  *  bluetooth audio
 *   */
#define AUD4_PAD_CTRL   ((PAD_CTL_LVE | PAD_CTL_HYS))
void setup_iomux_aud(void)
{
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT3__AUD4_RXD  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT4__AUD4_TXC  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT5__AUD4_TXFS | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS));
	imx_iomux_v3_setup_pad(MX6_PAD_LCD_DAT6__AUD4_TXD  | MUX_PAD_CTRL(PAD_CTL_LVE | PAD_CTL_HYS));
};



/* setup iomux */
void setup_iomux(void)
{

	setup_iomux_gpio1();
	setup_iomux_gpio2();
	setup_iomux_gpio3();
	setup_iomux_gpio4();
	setup_iomux_gpio5();

	setup_iomux_uart2();
	setup_iomux_uart3();
	setup_iomux_uart4();
	setup_iomux_aud();
	setup_iomux_wdog();
}

void setup_iomux_lpm(void)
{
	setup_iomux_usdhc1_lpm();
	setup_iomux_uart3_lpm();
	setup_iomux_uart4_lpm();
	setup_iomux_gpio4_lpm();
	setup_iomux_usdhc2_lpm();
}
void setup_iomux_idle(void)
{
	setup_iomux_uart3_lpm();
	setup_iomux_uart4_lpm();
	setup_iomux_gpio4_lpm();
	setup_iomux_usdhc2_lpm();
}
