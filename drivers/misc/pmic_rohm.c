/*
 * pmic_max77696.c 
 *
 * Copyright 2013 Amazon Technologies, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

/*!
 * @file pmic_max77696.c
 * @brief This file contains MAXIM 77696 specific PMIC code. This implementaion
 * may differ for each PMIC chip.
 *
 */

#include <common.h>
#include <lab126/pmic.h>
#include <lab126/pmic_rohm.h>
#include <linux/ctype.h>
#include <boardid.h>
#include <lab126/linux/frontlight.h>

#ifdef CONFIG_PMIC_FITIPOWER
#include <lab126/pmic_fitipower.h>
#endif

extern const u8 *get_board_id16(void);

#ifdef __DEBUG_PMIC__
#define DBG(fmt,args...)\
		serial_printf("[%s] %s:%d: "fmt,\
				__FILE__,__FUNCTION__,__LINE__, ##args)
#else
#define DBG(fmt,args...)
#endif


#define MAX_BRIGHTNESS                      0xFFF /* 12bits resolution */
#define MAX_SCALED_BRIGHTNESS               255
#define MIN_BRIGHTNESS                      0
#define CHG_SET1_WDT_AUTO_ON		0xAF
#define CHG_SET1_WDT_AUTO_OFF		0x2F
#define BD7181X_PRECHG_TIME_30MIN	0x1D
#define BD7181X_CHG_TIME_600MIN		0xAB

#define MID_VCOMR  -2499     //-2499mV
#define MAX_VCOMR  0         //0mV
#define MIN_VCOMR  -5000     //-5000mV
#define EPDVCOM_STEP 98

#define __s16_to_intval(val) \
	(((val) & 0x8000)? -((int)((0x7fff & ~(val)) + 1)) : ((int)(val)))

#define PMIC_CHECK_POWER_GOOD_NUM_RETRY 	10

#ifdef CONFIG_ROHM_FG_INIT

static int pmic_fg_init(void);

#endif

#ifdef CONFIG_PMIC_FITIPOWER
int pmic_enable_epdc(int enable);
int pmic_check_power_good(void);
static int setup_epd_pmic(void);
#endif

static int ocv_table[] = {
        4200,
        4176,
        4126,
        4082,
        4041,
        3997,
        3968,
        3938,
        3911,
        3877,
        3839,
        3817,
        3801,
        3790,
        3781,
        3775,
        3767,
        3750,
        3731,
        3703,
        3686,
        3614
};      /* unit 1 micro V */

static int soc_table[] = {
        100,
        100,
        95,
        90,
        85,
        80,
        75,
        70,
        65,
        60,
        55,
        50,
        45,
        40,
        35,
        30,
        25,
        20,
        15,
        10,
        5,
        0
        /* unit 0.1% */
};




static int setup_regulators(void);

int pmic_wor_reg(unsigned char saddr, unsigned int reg, const unsigned int value, const unsigned int mask)
{
    int ret;
    unsigned tmp;

    ret = pmic_read_reg(saddr, reg, &tmp);
    if (!ret) 
	return ret;
 
    tmp &= ~mask;
    tmp |= (value & mask);

    return pmic_write_reg(saddr, reg, tmp);
}

static unsigned short bd7181x_reg_read16(unsigned char saddr,unsigned int reg) {
        
	int ret;
	unsigned int v1,v2;

        ret = pmic_read_reg(saddr, reg,&v1);
	if (!ret)
	   return ret;
        ret = pmic_read_reg(saddr, reg + 1,&v2);
	if (!ret)
	   return ret;

        return (unsigned short)(v1<<8 | v2);
}


int pmic_check_factory_mode(void)
{
    printf("%s: begin\n", __func__);
    return 0;
}

void low_battery_check_loop(void)
{
	unsigned short batt_vol;
	int retry;
	if(PMIC_CMD_PASS == pmic_fg_read_voltage(&batt_vol)) {
		
		if(batt_vol < LAB126_LOWBATT_VOLTAGE) {
			printf("Low battery voltage=%dmV, please plug in charger and wait..\n", batt_vol);
			udelay(3 * 1000 *1000);
		}

		for(retry=10; 
			(batt_vol < LAB126_LOWBATT_VOLTAGE) && 
			(retry > 0); 
			/* no update */) {
			
			if( ! pmic_charging()) {
				retry--;
				printf("Not charging.. please connect to power source. time out in %d seconds\n", retry * 3);
			}
			
			if( PMIC_CMD_PASS == pmic_fg_read_voltage(&batt_vol) ) {
				// Loop until the battery charge to 
				printf("battery voltage=%dmV \n", batt_vol);				 
			}
			else{
				printf("%s:Battery voltage check failed ..\n", __func__);
				retry = 0;
				break;
			}
			udelay(3 * 1000 *1000);
			
		}//end for
		
		if(retry <= 0) {
			printf("Low battery and not charging.. system is going to halt .. \n");
			pmic_power_off();
		}
	}
	else {
		printf("%s:Battery voltage check failed ..\n", __func__);
		pmic_power_off();
	}
	return;
}

static int setup_regulators(void)
{
	return PMIC_CMD_PASS;
}

int pmic_enable_display_power_rail(unsigned int enable)

{
	
	fitipower_epdc_power(enable);
	return PMIC_CMD_PASS;
}

int pmic_charger_enable(int enable)
{
	pmic_wor_reg(ROHM_I2C_ADDR, BD7181X_REG_CHG_SET1, enable, 0x1);
	return PMIC_CMD_PASS;
}

int pmic_registers_setting(void)
{
	pmic_write_reg(ROHM_I2C_ADDR, BD7181X_REG_CHG_SET1, CHG_SET1_WDT_AUTO_ON);			/* Set Pre-charge Watchdog auto ON 0x47 <= 0xAF */
	pmic_write_reg(ROHM_I2C_ADDR, BD7181X_REG_CHG_WDT_PRE, BD7181X_PRECHG_TIME_30MIN); 	/* Set maximun pre-charge time to 30 minutes 0x49 <= 0x1D */
	pmic_write_reg(ROHM_I2C_ADDR, BD7181X_REG_CHG_WDT_FST, BD7181X_CHG_TIME_600MIN); 	/* Set maximun charging time to 600 minutes  0x4A <= 0xAB */
	pmic_write_reg(ROHM_I2C_ADDR, BD7181X_REG_CHG_SET1, CHG_SET1_WDT_AUTO_OFF);			/* Set Pre-charge Watchdog auto ON 0x47 <= 0x2F */

	return PMIC_CMD_PASS;
}

int pmic_init(void)
{
    int ret;
    //const char * rev = (const char *) get_board_id16();
    ret = board_pmic_init();

    if (!ret)
	return PMIC_CMD_FAIL;

    ret = setup_regulators();
    if (!ret)
        return PMIC_CMD_FAIL;  
 
#ifdef CONFIG_ROHM_FG_INIT
    ret = pmic_fg_init();
    if (!ret)
        return PMIC_CMD_FAIL;
#endif

#ifdef CONFIG_PMIC_FITIPOWER
    ret = setup_epd_pmic();
#endif
	pmic_registers_setting();
	pmic_charger_enable(1);
#ifndef CONFIG_MFGTOOL_MODE
    // loop until the battery charge to certain level or halt.
    low_battery_check_loop();
#endif //  CONFIG_MFGTOOL_MODE

    return ret;
}

static int setup_epd_pmic(void)
{
    return PMIC_CMD_PASS;
}

void pmic_zforce2_pwrenable(int ena)
{

}

int pmic_set_chg_current(pmic_charge_type chg_type)
{
    printf("%s: not implemented and return 1\n", __func__);
    return 1;
}
#define GLBL_CNFG0_FSENT	(1 << 5)
#define GLBL_CNFG0_SFTPDRR	(1 << 0)

int pmic_power_off(void)
{
	int ret = 1;
	
	return ret;
}

int pmic_reset(void)
{

    board_pmic_reset();

    return 1;
}

int pmic_charging(void)
{
    return 0;	
}

int pmic_charge_restart(void)
{
    printf("%s: not implemented and return 1\n", __func__);
    return 1;
}

int pmic_set_autochg(int autochg)
{
    printf("%s: not implemented and return 1\n", __func__);
    return 1;
}

int pmic_start_charging(pmic_charge_type chg_type, int autochg)
{
    printf("%s: not implemented and return 1\n", __func__);
    return 1;
}

static unsigned short saved_result = BATTERY_INVALID_VOLTAGE;

int pmic_adc_read_last_voltage(unsigned short *result) {

	/* just return the last saved voltage result */
	if (saved_result == BATTERY_INVALID_VOLTAGE) {
		*result = 0;
		return 0;
	}

	*result = saved_result;
	return 1;
}

int pmic_adc_read_voltage(unsigned short *result)
{
	if( PMIC_CMD_PASS != pmic_fg_read_voltage(result) ) {
		printf("Cannot read battery voltage! \n");
		return PMIC_CMD_FAIL;
	}
	saved_result = *result;
	return PMIC_CMD_PASS;
}

int pmic_enable_green_led(int enable)
{
    printf("%s: begin\n", __func__);
    return board_enable_green_led(enable);
}

int pmic_set_alarm(unsigned int secs_from_now) 
{
    printf("%s: not implemented and return 1\n", __func__);
    return 1;
}

int pmic_rd_por(unsigned *porv)
{
    printf("%s: not implemented and return 1\n", __func__);
    *porv = 0; 

    return 0;
}

int pmic_wr_por(void)
{
  printf("%s: not implemented and return 1\n", __func__);

  return 1;
}

int pmic_charger_mode_set(int mode)
{
	return PMIC_CMD_PASS;
}

int pmic_charger_lock_config(int lock)
{

	return PMIC_CMD_PASS;
}

int pmic_charger_set_current(int current)
{
	pmic_write_reg(ROHM_I2C_ADDR,BD7181X_REG_CHG_SET1,0x0);
	pmic_write_reg(ROHM_I2C_ADDR,BD7181X_REG_CHG_IFST,current);	
	pmic_write_reg(ROHM_I2C_ADDR,BD7181X_REG_CHG_SET1,0xB9);
	return PMIC_CMD_PASS;
}

int pmic_fg_read_voltage(unsigned short *voltage)
{
	unsigned short tmp_vcell;
        tmp_vcell = bd7181x_reg_read16(ROHM_I2C_ADDR, BD7181X_REG_VM_SMA_VBAT_U);

        *voltage = tmp_vcell;
        
	return PMIC_CMD_PASS;
}

int pmic_fg_read_avg_current(int *curr)
{
	int tmp_curr;
	unsigned int ibat_dir;
	tmp_curr = bd7181x_reg_read16(ROHM_I2C_ADDR, BD7181X_REG_VM_SMA_IBAT_U);
	ibat_dir = tmp_curr & 0x8000;
	tmp_curr = tmp_curr & 0xfff;
        if (ibat_dir != 0 ) {
                tmp_curr = -tmp_curr;
        }
	*curr = (tmp_curr * 33)/100;
	return PMIC_CMD_PASS;
}


int pmic_fg_read_capacity(unsigned short *capacity)
{
        int i = 0;
        int soc;
	unsigned short ocv;
	int ret;

        ret = pmic_fg_read_voltage(&ocv);
        if(PMIC_CMD_PASS != ret){
	   printf("PMIC read voltage failed\n");
	   return PMIC_CMD_FAIL;
	}
	if (ocv > ocv_table[0]) {
                soc = soc_table[0];
        } else {
                i = 0;
                while (soc_table[i] != 0) {
                        if ((ocv <= ocv_table[i]) && (ocv > ocv_table[i+1])) {
                                soc = (soc_table[i] - soc_table[i+1]) * (ocv - ocv_table[i+1]) / (ocv_table[i] - ocv_table[i+1]);
                                soc += soc_table[i+1];
                                break;
                        }
                        i++;
                }
                if (soc_table[i] == 0)
                        soc = soc_table[i];
        }

        *capacity = soc;


    return PMIC_CMD_PASS;
}

int pmic_fg_read_temperature(int *temperature)
{
    unsigned int t;
    int ret;
    ret = pmic_read_reg(ROHM_I2C_ADDR, BD7181X_REG_VM_BTMP,&t);
    if (!ret)
	return PMIC_CMD_FAIL;

    t = 200 - t;
    t = (t > 200)? 200: t;
    *temperature = t;

    return PMIC_CMD_PASS;
}

int pmic_fl_enable ( int enable)
{
    return PMIC_CMD_PASS;
}

int pmic_fl_set (int brightness)
{

    return PMIC_CMD_PASS;
}
 
int pmic_fg_otp_check(void)
{
    return PMIC_CMD_PASS;
}

int is_epdc_on(void)
{
     return PMIC_CMD_PASS;
}

int set_vcom_value(char* value)
{
	 
	int vcom_val = 0;
        int epdvcom = 0;
	
	printf(" set Vcom value:%smV\n",value);	
	
	vcom_val = simple_strtol(value,NULL,10);
	if(vcom_val < 604 || vcom_val > 5002)
	    return PMIC_CMD_FAIL;

	epdvcom=0x1D + (vcom_val-604)/22;	
	printf("REG set to 0x%x\n",epdvcom);	

	return pmic_write_reg(FITIPOWER_I2C_ADDR, VCOM_SETTING_REG, epdvcom);
}

int pmic_vcom_set(char* vcom_val)
{
      /*
       sets VCOM value
      */
     set_vcom_value(vcom_val); 
     return pmic_enable_vcom(1); 

}

int pmic_read_epdc_temperature(int *temperature)
{
    unsigned int t;
    int ret;
    ret = pmic_read_reg(FITIPOWER_I2C_ADDR, TMST_VALUE_REG, &t);
    if (!ret)
		return PMIC_CMD_FAIL;

    *temperature = (int)(t & 0xff);

    return PMIC_CMD_PASS;
}

int pmic_check_power_good(void)
{
     return PMIC_CMD_PASS;
}

int pmic_enable_vcom(int enable)
{
    fitipower_epdc_power(enable);
    return PMIC_CMD_PASS;
}

int pmic_enable_epdc(int enable)
{
   fitipower_epdc_power(enable);
   return PMIC_CMD_PASS;
}

void pmic_enable_usb_phy(int enable) {

}

#ifdef CONFIG_ROHM_FG_INIT
/* Fuel gauge initial settings */
static int pmic_fg_init(void)
{
   return 0;
}

#endif
