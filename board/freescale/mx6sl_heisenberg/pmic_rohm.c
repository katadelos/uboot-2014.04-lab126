/*
 * pmic.c 
 *
 * Copyright 2012 Amazon Technologies, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @file pmic.c
 * @brief This file contains mx6 board-specific info to talk to the MAXIM 77697 PMIC.
 *
 */

#define __DEBUG_PMIC__ 1
#ifdef __DEBUG_PMIC__
#define DBG(fmt,args...)\
		serial_printf("[%s] %s:%d: "fmt,\
				__FILE__,__FUNCTION__,__LINE__, ##args)
#else
#define DBG(fmt,args...)
#endif


#include <common.h>

#include <lab126/pmic.h>
#include <i2c.h>
#include "regs-anadig.h"

#ifdef CONFIG_PMIC_MAX77696
#include <lab126/pmic_max77696.h>
#endif

#ifdef CONFIG_PMIC_ROHM
#include <lab126/pmic_rohm.h>
#endif

#ifdef CONFIG_PMIC_FITIPOWER
#include <lab126/pmic_fitipower.h>
#endif
#include <asm/arch/sys_proto.h>


int board_pmic_init(void) 
{
    return 1;
}

int board_pmic_write_reg(unsigned char saddr, unsigned int reg, const unsigned int val) 
{
    int ret;

    i2c_set_bus_num(0);
    switch(saddr) {
#ifdef CONFIG_PMIC_MAX77696
    case FG_SADDR: 
      ret = i2c_write(saddr, reg, 1, (unsigned char *) &val, 2);
    break;
#endif
    default:
      ret = i2c_write(saddr, reg, 1, (unsigned char *) &val, 1);
    break;
    }

    if (ret)
    {
        DBG("\n%s: failed! for sddr %x, regaddr: %x \n", __func__, saddr, reg);
	return 0;
    }

    return 1;
}

int board_pmic_read_reg(unsigned char saddr, unsigned int reg, unsigned int *val) 
{
    int ret;

    *val = 0;

    i2c_set_bus_num(0);
    switch(saddr) {
#ifdef CONFIG_PMIC_MAX77696
    case FG_SADDR:
      ret = i2c_read(saddr, reg, 1, (unsigned char *) val, 2);
    break;
#endif
    default:
      ret = i2c_read(saddr, reg, 1, (unsigned char *) val, 1);
    break;
    }
    if (ret)
    {
        DBG("\n%s: failed! for sddr %x, regaddr: %x \n", __func__, saddr, reg);
	return 0;
    }

    return 1;
}

int board_pmic_write_verify_reg(unsigned char saddr, unsigned int reg, unsigned int val)
{
   unsigned int rdval; 

   board_pmic_write_reg(saddr, reg, val);
   udelay(100); //some delay before reading back
   board_pmic_read_reg(saddr, reg, &rdval); 

   if(rdval != val) {
        DBG("\n%s: WriteVerify failed! for sddr %x, regaddr: %x		\
		Value written %x, value read-back %x\n", __func__, saddr, reg, val, rdval);
   	return 1;
   }
   return 0;
}

int board_enable_green_led(int enable) 
{
#if 0	/* BEN TODO */
    int pad_val = PAD_CTL_DRV_NORMAL | PAD_CTL_SRE_SLOW | PAD_CTL_DRV_1_8V |
	    PAD_CTL_HYS_CMOS | PAD_CTL_PKE_NONE | PAD_CTL_100K_PU;

    mx35_gpio_direction(IOMUX_TO_GPIO(MX35_PIN_CSI_D12), MX35_GPIO_DIRECTION_OUT);

    /* shasta/luigi controls led w/ gpio */
    if (enable) {
	mx35_gpio_set(IOMUX_TO_GPIO(MX35_PIN_CSI_D12), 1);
    } else {
	mx35_gpio_set(IOMUX_TO_GPIO(MX35_PIN_CSI_D12), 0);
    }

    mxc_iomux_set_pad(MX35_PIN_CSI_D12, pad_val);
#endif
    return 1;
}

#ifdef CONFIG_MX6_INTER_LDO_BYPASS
#define REG_RD(base, reg) \
    (*(volatile unsigned int *)((base) + (reg)))
#define REG_WR(base, reg, value) \
    ((*(volatile unsigned int *)((base) + (reg))) = (value))


void bypass_internal_pmic_ldo(void)
{
    unsigned int val = 0;

    /* Bypass the VDDSOC from Anatop */
    val = REG_RD(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE);
    val &= ~BM_ANADIG_REG_CORE_REG2_TRG;
    val |= BF_ANADIG_REG_CORE_REG2_TRG(0x1f);
    REG_WR(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE, val);

    /* Bypass the VDDCORE from Anatop */
    val = REG_RD(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE);
    val &= ~BM_ANADIG_REG_CORE_REG0_TRG;
    val |= BF_ANADIG_REG_CORE_REG0_TRG(0x1f);
    REG_WR(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE, val);

    /* Powergate the VDDPU from Anatop - unused on Wario */
    val = REG_RD(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE);
    val &= ~BM_ANADIG_REG_CORE_REG1_TRG;
    val |= BF_ANADIG_REG_CORE_REG1_TRG(0x00);
    REG_WR(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE, val);
}
#endif

int board_pmic_reset(void)
{
    return 1;
}

static u32 aboot_mode = 0xFFFF;
#define PMIC_BOOT_REGISTER 0xB7
int board_pmic_get_aboot_mode(void)
{
	u32 reg;
	int ret = 1;
	int retry = 10;

	if (aboot_mode != 0xFFFF)
		return aboot_mode;

	do {
		ret = board_pmic_read_reg(0x4B, PMIC_BOOT_REGISTER, &reg);
	} while ((ret == 0) && (retry-- > 0));
	
	aboot_mode = reg & ANDROID_BOOT_MASK;

	/*
	 * Donot clear 'regular reboot' reason, this is set by
	 * kernel and is used to distinguish between watchdog
	 * reset and a normal reboot.
	 */
	if (aboot_mode == ANDROID_BOOT_REGULAR_REBOOT)
		return aboot_mode;

	reg &= ~ANDROID_BOOT_MASK;
#ifndef CONFIG_BIST
	if (aboot_mode == ANDROID_BOOT_FASTBOOT)
		reg |= ANDROID_BOOT_FASTBOOT;
#endif
	ret = board_pmic_write_reg(0x4B, PMIC_BOOT_REGISTER, reg);

	return aboot_mode;
}

void board_pmic_set_aboot_mode(u32 mode)
{
	u32 reg;
	int ret;

	ret = board_pmic_read_reg(0x4B, PMIC_BOOT_REGISTER, &reg);
	reg &= ~ANDROID_BOOT_MASK;
	reg |= (mode & ANDROID_BOOT_MASK);
	ret = board_pmic_write_reg(0x4B, PMIC_BOOT_REGISTER, reg);
}

