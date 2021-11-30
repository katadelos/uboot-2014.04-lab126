/*
 * Copyright (C) 2010-2012 Freescale Semiconductor, Inc.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

// DRAM Configuration

#include "common.h"
#include "registers.h"
#include "dram_api.h"

// ---------------------------------------------------------------------
// Registers
// ---------------------------------------------------------------------
//DDR LOGIC
#define MMDC_0_MDCTL           (MMDC_P0_BASE_ADDR + MDCTL_OFFSET)
#define MMDC_0_MDPDC           (MMDC_P0_BASE_ADDR + MDPDC_OFFSET)
#define MMDC_0_MDOTC           (MMDC_P0_BASE_ADDR + MDOTC_OFFSET)
#define MMDC_0_MDCFG0          (MMDC_P0_BASE_ADDR + MDCFG0_OFFSET)
#define MMDC_0_MDCFG1          (MMDC_P0_BASE_ADDR + MDCFG1_OFFSET)
#define MMDC_0_MDCFG2          (MMDC_P0_BASE_ADDR + MDCFG2_OFFSET)
#define MMDC_0_MDMISC          (MMDC_P0_BASE_ADDR + MDMISC_OFFSET)
#define MMDC_0_MDSCR           (MMDC_P0_BASE_ADDR + MDSCR_OFFSET)
#define MMDC_0_MDREF           (MMDC_P0_BASE_ADDR + MDREF_OFFSET)
#define MMDC_0_MDWCC           (MMDC_P0_BASE_ADDR + MDWCC_OFFSET)
#define MMDC_0_MDRCC           (MMDC_P0_BASE_ADDR + MDRCC_OFFSET)
#define MMDC_0_MDRWD           (MMDC_P0_BASE_ADDR + MDRWD_OFFSET)
#define MMDC_0_MDOR            (MMDC_P0_BASE_ADDR + MDOR_OFFSET)
#define MMDC_0_MDMRR           (MMDC_P0_BASE_ADDR + MDMRR_OFFSET)
#define MMDC_0_MDCFG3LP        (MMDC_P0_BASE_ADDR + MDCFG3LP_OFFSET)
#define MMDC_0_MDMR4           (MMDC_P0_BASE_ADDR + MDMR4_OFFSET)
#define MMDC_0_MDASP           (MMDC_P0_BASE_ADDR + MDASP_OFFSET)

#define MMDC_1_MDCTL           (MMDC_P1_BASE_ADDR + MDCTL_OFFSET)
#define MMDC_1_MDPDC           (MMDC_P1_BASE_ADDR + MDPDC_OFFSET)
#define MMDC_1_MDOTC           (MMDC_P1_BASE_ADDR + MDOTC_OFFSET)
#define MMDC_1_MDCFG0          (MMDC_P1_BASE_ADDR + MDCFG0_OFFSET)
#define MMDC_1_MDCFG1          (MMDC_P1_BASE_ADDR + MDCFG1_OFFSET)
#define MMDC_1_MDCFG2          (MMDC_P1_BASE_ADDR + MDCFG2_OFFSET)
#define MMDC_1_MDMISC          (MMDC_P1_BASE_ADDR + MDMISC_OFFSET)
#define MMDC_1_MDSCR           (MMDC_P1_BASE_ADDR + MDSCR_OFFSET)
#define MMDC_1_MDREF           (MMDC_P1_BASE_ADDR + MDREF_OFFSET)
#define MMDC_1_MDWCC           (MMDC_P1_BASE_ADDR + MDWCC_OFFSET)
#define MMDC_1_MDRCC           (MMDC_P1_BASE_ADDR + MDRCC_OFFSET)
#define MMDC_1_MDRWD           (MMDC_P1_BASE_ADDR + MDRWD_OFFSET)
#define MMDC_1_MDOR            (MMDC_P1_BASE_ADDR + MDOR_OFFSET)
#define MMDC_1_MDMRR           (MMDC_P1_BASE_ADDR + MDMRR_OFFSET)
#define MMDC_1_MDCFG3LP        (MMDC_P1_BASE_ADDR + MDCFG3LP_OFFSET)
#define MMDC_1_MDMR4           (MMDC_P1_BASE_ADDR + MDMR4_OFFSET)
#define MMDC_1_MDASP           (MMDC_P1_BASE_ADDR + MDASP_OFFSET)

//ADOPT
#define MMDC_0_MAARCR          (MMDC_P0_BASE_ADDR + MAARCR_OFFSET) // ARCR
#define MMDC_0_MAPSR           (MMDC_P0_BASE_ADDR + MAPSR_OFFSET) // PSR (former MCR0 and PSM0)
#define MMDC_0_MAEXIDR0        (MMDC_P0_BASE_ADDR + MAEXIDR0_OFFSET) // Exclusive ID Monitor register0
#define MMDC_0_MAEXIDR1        (MMDC_P0_BASE_ADDR + MAEXIDR1_OFFSET) // Exclusive ID Monitor register1
#define MMDC_0_MADPCR0         (MMDC_P0_BASE_ADDR + MADPCR0_OFFSET) // Debug and Profiling Control register0
#define MMDC_0_MADPCR1         (MMDC_P0_BASE_ADDR + MADPCR1_OFFSET) // Debug and Profiling Control register1
#define MMDC_0_MADPSR0         (MMDC_P0_BASE_ADDR + MADPSR0_OFFSET) // Debug and Profiling Control register0
#define MMDC_0_MADPSR1         (MMDC_P0_BASE_ADDR + MADPSR1_OFFSET) // Debug and Profiling Control register1
#define MMDC_0_MADPSR2         (MMDC_P0_BASE_ADDR + MADPSR2_OFFSET) // Debug and Profiling Control register2
#define MMDC_0_MADPSR3         (MMDC_P0_BASE_ADDR + MADPSR3_OFFSET) // Debug and Profiling Control register3
#define MMDC_0_MADPSR4         (MMDC_P0_BASE_ADDR + MADPSR4_OFFSET) // Debug and Profiling Control register2
#define MMDC_0_MADPSR5         (MMDC_P0_BASE_ADDR + MADPSR5_OFFSET) // Debug and Profiling Control register2
#define MMDC_0_MASBS0          (MMDC_P0_BASE_ADDR + MASBS0_OFFSET) // Step by Step Address
#define MMDC_0_MASBS1          (MMDC_P0_BASE_ADDR + MASBS1_OFFSET) // Step by Step Controls
#define MMDC_0_MAGENP          (MMDC_P0_BASE_ADDR + MAGENP_OFFSET) // General purpose register

#define MMDC_1_MAARCR          (MMDC_P1_BASE_ADDR + MAARCR_OFFSET) // ARCR
#define MMDC_1_MAPSR           (MMDC_P1_BASE_ADDR + MAPSR_OFFSET) // PSR (former MCR0 and PSM0)
#define MMDC_1_MAEXIDR0        (MMDC_P1_BASE_ADDR + MAEXIDR0_OFFSET) // Exclusive ID Monitor register0
#define MMDC_1_MAEXIDR1        (MMDC_P1_BASE_ADDR + MAEXIDR1_OFFSET) // Exclusive ID Monitor register1
#define MMDC_1_MADPCR0         (MMDC_P1_BASE_ADDR + MADPCR0_OFFSET) // Debug and Profiling Control register0
#define MMDC_1_MADPCR1         (MMDC_P1_BASE_ADDR + MADPCR1_OFFSET) // Debug and Profiling Control register1
#define MMDC_1_MADPSR0         (MMDC_P1_BASE_ADDR + MADPSR0_OFFSET) // Debug and Profiling Control register0
#define MMDC_1_MADPSR1         (MMDC_P1_BASE_ADDR + MADPSR1_OFFSET) // Debug and Profiling Control register1
#define MMDC_1_MADPSR2         (MMDC_P1_BASE_ADDR + MADPSR2_OFFSET) // Debug and Profiling Control register2
#define MMDC_1_MADPSR3         (MMDC_P1_BASE_ADDR + MADPSR3_OFFSET) // Debug and Profiling Control register3
#define MMDC_1_MADPSR4         (MMDC_P1_BASE_ADDR + MADPSR4_OFFSET) // Debug and Profiling Control register2
#define MMDC_1_MADPSR5         (MMDC_P1_BASE_ADDR + MADPSR5_OFFSET) // Debug and Profiling Control register2
#define MMDC_1_MASBS0          (MMDC_P1_BASE_ADDR + MASBS0_OFFSET) // Step by Step Address
#define MMDC_1_MASBS1          (MMDC_P1_BASE_ADDR + MASBS1_OFFSET) // Step by Step Controls
#define MMDC_1_MAGENP          (MMDC_P1_BASE_ADDR + MAGENP_OFFSET) // General purpose register

//DDR_PHY
#define MMDC_0_MPZQHWCTRL      (MMDC_P0_BASE_ADDR + MPZQHWCTRL_OFFSET)
#define MMDC_0_MPZQSWCTRL      (MMDC_P0_BASE_ADDR + MPZQSWCTRL_OFFSET)
#define MMDC_0_MPWLGCR         (MMDC_P0_BASE_ADDR + MPWLGCR_OFFSET)
#define MMDC_0_MPWLDECTRL0     (MMDC_P0_BASE_ADDR + MPWLDECTRL0_OFFSET)
#define MMDC_0_MPWLDECTRL1     (MMDC_P0_BASE_ADDR + MPWLDECTRL1_OFFSET)
#define MMDC_0_MPWLDLST        (MMDC_P0_BASE_ADDR + MPWLDLST_OFFSET)
#define MMDC_0_MPODTCTRL       (MMDC_P0_BASE_ADDR + MPODTCTRL_OFFSET)
#define MMDC_0_MPREDQBY0DL     (MMDC_P0_BASE_ADDR + MPREDQBY0DL_OFFSET)
#define MMDC_0_MPREDQBY1DL     (MMDC_P0_BASE_ADDR + MPREDQBY1DL_OFFSET)
#define MMDC_0_MPREDQBY2DL     (MMDC_P0_BASE_ADDR + MPREDQBY2DL_OFFSET)
#define MMDC_0_MPREDQBY3DL     (MMDC_P0_BASE_ADDR + MPREDQBY3DL_OFFSET)
#define MMDC_0_MPWRDQBY0DL     (MMDC_P0_BASE_ADDR + MPWRDQBY0DL_OFFSET)
#define MMDC_0_MPWRDQBY1DL     (MMDC_P0_BASE_ADDR + MPWRDQBY1DL_OFFSET)
#define MMDC_0_MPWRDQBY2DL     (MMDC_P0_BASE_ADDR + MPWRDQBY2DL_OFFSET)
#define MMDC_0_MPWRDQBY3DL     (MMDC_P0_BASE_ADDR + MPWRDQBY3DL_OFFSET)
#define MMDC_0_MPDGCTRL0       (MMDC_P0_BASE_ADDR + MPDGCTRL0_OFFSET)
#define MMDC_0_MPDGCTRL1       (MMDC_P0_BASE_ADDR + MPDGCTRL1_OFFSET)
#define MMDC_0_MPDGDLST        (MMDC_P0_BASE_ADDR + MPDGDLST_OFFSET)
#define MMDC_0_MPRDDLCTL       (MMDC_P0_BASE_ADDR + MPRDDLCTL_OFFSET)
#define MMDC_0_MPRDDLST        (MMDC_P0_BASE_ADDR + MPRDDLST_OFFSET)
#define MMDC_0_MPWRDLCTL       (MMDC_P0_BASE_ADDR + MPWRDLCTL_OFFSET)
#define MMDC_0_MPWRDLST        (MMDC_P0_BASE_ADDR + MPWRDLST_OFFSET)
#define MMDC_0_MPSDCTRL        (MMDC_P0_BASE_ADDR + MPSDCTRL_OFFSET)
#define MMDC_0_MPZQLP2CTL      (MMDC_P0_BASE_ADDR + MPZQLP2CTL_OFFSET )
#define MMDC_0_MPRDDLHWCTL     (MMDC_P0_BASE_ADDR + MPRDDLHWCTL_OFFSET)
#define MMDC_0_MPWRDLHWCTL     (MMDC_P0_BASE_ADDR + MPWRDLHWCTL_OFFSET)
#define MMDC_0_MPRDDLHWST0     (MMDC_P0_BASE_ADDR + MPRDDLHWST0_OFFSET)
#define MMDC_0_MPRDDLHWST1     (MMDC_P0_BASE_ADDR + MPRDDLHWST1_OFFSET)
#define MMDC_0_MPWRDLHWST0     (MMDC_P0_BASE_ADDR + MPWRDLHWST0_OFFSET)
#define MMDC_0_MPWRDLHWST1     (MMDC_P0_BASE_ADDR + MPWRDLHWST1_OFFSET)
#define MMDC_0_MPWLHWERR       (MMDC_P0_BASE_ADDR + MPWLHWERR_OFFSET)
#define MMDC_0_MPDGHWST0       (MMDC_P0_BASE_ADDR + MPDGHWST0_OFFSET)
#define MMDC_0_MPDGHWST1       (MMDC_P0_BASE_ADDR + MPDGHWST1_OFFSET)
#define MMDC_0_MPDGHWST2       (MMDC_P0_BASE_ADDR + MPDGHWST2_OFFSET)
#define MMDC_0_MPDGHWST3       (MMDC_P0_BASE_ADDR + MPDGHWST3_OFFSET)
#define MMDC_0_MPPDCMPR1       (MMDC_P0_BASE_ADDR + MPPDCMPR1_OFFSET)
#define MMDC_0_MPPDCMPR2       (MMDC_P0_BASE_ADDR + MPPDCMPR2_OFFSET)
#define MMDC_0_MPSWDAR         (MMDC_P0_BASE_ADDR + MPSWDAR_OFFSET)
#define MMDC_0_MPSWDRDR0       (MMDC_P0_BASE_ADDR + MPSWDRDR0_OFFSET)
#define MMDC_0_MPSWDRDR1       (MMDC_P0_BASE_ADDR + MPSWDRDR1_OFFSET)
#define MMDC_0_MPSWDRDR2       (MMDC_P0_BASE_ADDR + MPSWDRDR2_OFFSET)
#define MMDC_0_MPSWDRDR3       (MMDC_P0_BASE_ADDR + MPSWDRDR3_OFFSET)
#define MMDC_0_MPSWDRDR4       (MMDC_P0_BASE_ADDR + MPSWDRDR4_OFFSET)
#define MMDC_0_MPSWDRDR5       (MMDC_P0_BASE_ADDR + MPSWDRDR5_OFFSET)
#define MMDC_0_MPSWDRDR6       (MMDC_P0_BASE_ADDR + MPSWDRDR6_OFFSET)
#define MMDC_0_MPSWDRDR7       (MMDC_P0_BASE_ADDR + MPSWDRDR7_OFFSET)
#define MMDC_0_MPMUR           (MMDC_P0_BASE_ADDR + MPMUR_OFFSET)
#define MMDC_0_MPWRCADL        (MMDC_P0_BASE_ADDR + MPWRCADL_OFFSET)
#define MMDC_0_MPDCCR          (MMDC_P0_BASE_ADDR + MPDCCR_OFFSET)
#define MMDC_0_MPBC            (MMDC_P0_BASE_ADDR + MPBC_OFFSET)

#define MMDC_1_MPZQHWCTRL      (MMDC_P1_BASE_ADDR + MPZQHWCTRL_OFFSET)
#define MMDC_1_MPZQSWCTRL      (MMDC_P1_BASE_ADDR + MPZQSWCTRL_OFFSET)
#define MMDC_1_MPWLGCR         (MMDC_P1_BASE_ADDR + MPWLGCR_OFFSET)
#define MMDC_1_MPWLDECTRL0     (MMDC_P1_BASE_ADDR + MPWLDECTRL0_OFFSET)
#define MMDC_1_MPWLDECTRL1     (MMDC_P1_BASE_ADDR + MPWLDECTRL1_OFFSET)
#define MMDC_1_MPWLDLST        (MMDC_P1_BASE_ADDR + MPWLDLST_OFFSET)
#define MMDC_1_MPODTCTRL       (MMDC_P1_BASE_ADDR + MPODTCTRL_OFFSET)
#define MMDC_1_MPREDQBY0DL     (MMDC_P1_BASE_ADDR + MPREDQBY0DL_OFFSET)
#define MMDC_1_MPREDQBY1DL     (MMDC_P1_BASE_ADDR + MPREDQBY1DL_OFFSET)
#define MMDC_1_MPREDQBY2DL     (MMDC_P1_BASE_ADDR + MPREDQBY2DL_OFFSET)
#define MMDC_1_MPREDQBY3DL     (MMDC_P1_BASE_ADDR + MPREDQBY3DL_OFFSET)
#define MMDC_1_MPWRDQBY0DL     (MMDC_P1_BASE_ADDR + MPWRDQBY0DL_OFFSET)
#define MMDC_1_MPWRDQBY1DL     (MMDC_P1_BASE_ADDR + MPWRDQBY1DL_OFFSET)
#define MMDC_1_MPWRDQBY2DL     (MMDC_P1_BASE_ADDR + MPWRDQBY2DL_OFFSET)
#define MMDC_1_MPWRDQBY3DL     (MMDC_P1_BASE_ADDR + MPWRDQBY3DL_OFFSET)
#define MMDC_1_MPDGCTRL0       (MMDC_P1_BASE_ADDR + MPDGCTRL0_OFFSET)
#define MMDC_1_MPDGCTRL1       (MMDC_P1_BASE_ADDR + MPDGCTRL1_OFFSET)
#define MMDC_1_MPDGDLST        (MMDC_P1_BASE_ADDR + MPDGDLST_OFFSET)
#define MMDC_1_MPRDDLCTL       (MMDC_P1_BASE_ADDR + MPRDDLCTL_OFFSET)
#define MMDC_1_MPRDDLST        (MMDC_P1_BASE_ADDR + MPRDDLST_OFFSET)
#define MMDC_1_MPWRDLCTL       (MMDC_P1_BASE_ADDR + MPWRDLCTL_OFFSET)
#define MMDC_1_MPWRDLST        (MMDC_P1_BASE_ADDR + MPWRDLST_OFFSET)
#define MMDC_1_MPSDCTRL        (MMDC_P1_BASE_ADDR + MPSDCTRL_OFFSET)
#define MMDC_1_MPZQLP2CTL      (MMDC_P1_BASE_ADDR + MPZQLP2CTL_OFFSET)
#define MMDC_1_MPRDDLHWCTL     (MMDC_P1_BASE_ADDR + MPRDDLHWCTL_OFFSET)
#define MMDC_1_MPWRDLHWCTL     (MMDC_P1_BASE_ADDR + MPWRDLHWCTL_OFFSET)
#define MMDC_1_MPRDDLHWST0     (MMDC_P1_BASE_ADDR + MPRDDLHWST0_OFFSET)
#define MMDC_1_MPRDDLHWST1     (MMDC_P1_BASE_ADDR + MPRDDLHWST1_OFFSET)
#define MMDC_1_MPWRDLHWST0     (MMDC_P1_BASE_ADDR + MPWRDLHWST0_OFFSET)
#define MMDC_1_MPWRDLHWST1     (MMDC_P1_BASE_ADDR + MPWRDLHWST1_OFFSET)
#define MMDC_1_MPWLHWERR       (MMDC_P1_BASE_ADDR + MPWLHWERR_OFFSET)
#define MMDC_1_MPDGHWST0       (MMDC_P1_BASE_ADDR + MPDGHWST0_OFFSET)
#define MMDC_1_MPDGHWST1       (MMDC_P1_BASE_ADDR + MPDGHWST1_OFFSET)
#define MMDC_1_MPDGHWST2       (MMDC_P1_BASE_ADDR + MPDGHWST2_OFFSET)
#define MMDC_1_MPDGHWST3       (MMDC_P1_BASE_ADDR + MPDGHWST3_OFFSET)
#define MMDC_1_MPPDCMPR1       (MMDC_P1_BASE_ADDR + MPPDCMPR1_OFFSET)
#define MMDC_1_MPPDCMPR2       (MMDC_P1_BASE_ADDR + MPPDCMPR2_OFFSET)
#define MMDC_1_MPSWDAR         (MMDC_P1_BASE_ADDR + MPSWDAR_OFFSET)
#define MMDC_1_MPSWDRDR0       (MMDC_P1_BASE_ADDR + MPSWDRDR0_OFFSET)
#define MMDC_1_MPSWDRDR1       (MMDC_P1_BASE_ADDR + MPSWDRDR1_OFFSET)
#define MMDC_1_MPSWDRDR2       (MMDC_P1_BASE_ADDR + MPSWDRDR2_OFFSET)
#define MMDC_1_MPSWDRDR3       (MMDC_P1_BASE_ADDR + MPSWDRDR3_OFFSET)
#define MMDC_1_MPSWDRDR4       (MMDC_P1_BASE_ADDR + MPSWDRDR4_OFFSET)
#define MMDC_1_MPSWDRDR5       (MMDC_P1_BASE_ADDR + MPSWDRDR5_OFFSET)
#define MMDC_1_MPSWDRDR6       (MMDC_P1_BASE_ADDR + MPSWDRDR6_OFFSET)
#define MMDC_1_MPSWDRDR7       (MMDC_P1_BASE_ADDR + MPSWDRDR7_OFFSET)
#define MMDC_1_MPMUR           (MMDC_P1_BASE_ADDR + MPMUR_OFFSET)
#define MMDC_1_MPWRCADL        (MMDC_P1_BASE_ADDR + MPWRCADL_OFFSET)
#define MMDC_1_MPDCCR          (MMDC_P1_BASE_ADDR + MPDCCR_OFFSET)
#define MMDC_1_MPBC            (MMDC_P1_BASE_ADDR + MPBC_OFFSET)

// ---------------------------------------------------------------------
// functions
// ---------------------------------------------------------------------
void dram_data_init(WORD, WORD)
	__attribute__ ((section (".low_power_code")));
void dram_data_init(WORD addr, WORD length)
{
	WORD i;

	for(i = 0; i < length; i++)
	{
		*((WORD *)(addr) + i) = i;
	}
}

int dram_data_check(WORD , WORD)
	__attribute__ ((section (".low_power_code")));
int dram_data_check(WORD addr, WORD length)
{
	WORD i;
	WORD data32;

	for(i = 0; i < length; i++) {
		data32 = *((WORD *)(addr) + i);
		if(data32 != i)
		{
			return 1;
		}
	}

	return 0;
}

void lpddr2_enter_lpm (void)
	__attribute__ ((section (".low_power_code")));
void lpddr2_enter_lpm (void)
{
	return;

	//while ((reg32_read(0x021b0404) & 0x10) == 0);//Notes from 2015.12.2: the following code are not copied to the OCRAM to put the DDR to self-refresh mode. so it will hang on the while loop
                                                       // here, since the suspend_resume.S is copied to the OCRAM, will do the following setting in the suspend_resume.S file.

	/* Put DDR explicitly into self-refresh. */
	 /* Disable Automatic power savings. */
	reg32_write(0x21b0404, reg32_read(0x021b0404) | 0x1);
	/* Set the DVFS bit. */
	reg32_write(0x21b0404, reg32_read(0x021b0404) | 0x200000);
	/* Wait for the DVFS-ACK */
	while ((reg32_read(0x021b0404) & 0x2000000) == 0);

	reg32_write(IOMUXC_SW_PAD_CTL_GRP_DDRPK, 0x00000000); // IOMUXC_SW_PAD_CTL_GRP_DDRPKE
	reg32_write(IOMUXC_SW_PAD_CTL_GRP_DDRPKE, 0x00001000); // IOMUXC_SW_PAD_CTL_GRP_DDRPKE

	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM0 - DSE=110, DDR_INPUT=1, HYS=0
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM1 - DSE=110, DDR_INPUT=1, HYS=0
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM2, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM2 - DSE=110, DDR_INPUT=1, HYS=0
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM3, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_DQM3 - DSE=110, DDR_INPUT=1, HYS=0

	reg32_write(IOMUXC_SW_PAD_CTL_GRP_B0DS, 0x00000000);  // IOMUXC_SW_PAD_CTL_GRP_B0DS - DSE=110
	reg32_write(IOMUXC_SW_PAD_CTL_GRP_B1DS, 0x00000000);  // IOMUXC_SW_PAD_CTL_GRP_B1DS - DSE=110
	reg32_write(IOMUXC_SW_PAD_CTL_GRP_B2DS, 0x00000000);  // IOMUXC_SW_PAD_CTL_GRP_B2DS - DSE=110
	reg32_write(IOMUXC_SW_PAD_CTL_GRP_B3DS, 0x00000000);  // IOMUXC_SW_PAD_CTL_GRP_B3DS - DSE=110

	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_CAS, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_CAS - DSE=110, DDR_INPUT=1, HYS=0
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_RAS, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_RAS - DSE=110, DDR_INPUT=1, HYS=0
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK_0, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCLK_0 - DSE=101, DDR_INPUT=0, HYS=0
	reg32_write(IOMUXC_SW_PAD_CTL_GRP_ADDDS, 0x00000000);  // IOMUXC_SW_PAD_CTL_GRP_ADDS - DSE=110

	reg32_write(IOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL  , 0x00000000);  // IOMUXC_SW_PAD_CTL_GRP_DDRMODE_CTL - DDR_INPUT=1
	reg32_write(IOMUXC_SW_PAD_CTL_GRP_DDRMODE, 0x00000000);  // IOMUXC_SW_PAD_CTL_GRP_DDRMODE- DDR_INPUT 1,diff
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_SDODT0, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDODT0
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_SDODT1, 0x00000000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDODT1

	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_RESET, 0x00080000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_RESET
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCKE0, 0x00001000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCKE0
	reg32_write(IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCKE1, 0x00001000);  // IOMUXC_SW_PAD_CTL_PAD_DRAM_SDCKE1
}