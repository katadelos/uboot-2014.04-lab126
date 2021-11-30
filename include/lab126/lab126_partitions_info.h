/*
 * lab126_partitions_info.h
 *
 * Copyright 2010-2015 Amazon Technologies, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef __MX60_EANAB_BOARD_H__
#define __MX60_EANAB_BOARD_H__

#define PARTITION_FILL_SPACE	-1


typedef struct partition_info_t {
    const char *name;
    unsigned int address;
    unsigned int size;
    unsigned int partition;
} partition_info_t;


const struct partition_info_t partition_info_default[CONFIG_NUM_PARTITIONS] = {
	{
		.name = "bootloader",
		.address = 0,
		.size = CONFIG_MMC_USERDATA_ADDR, 
		.partition = CONFIG_BOOT_FROM_PARTITION
	},
	{
		.name = "prod",
		.address = 0x0, /* overlap with bootloader */
		.size = CONFIG_MMC_BIST_ADDR, 
		.partition = CONFIG_BOOT_FROM_PARTITION
	},
	{
		.name = "bist",
		.address = CONFIG_MMC_BIST_ADDR, /* overlap with bootloader */
		.size = CONFIG_MMC_BIST_SIZE, /* 256 KiB */
		.partition = CONFIG_BOOT_FROM_PARTITION
	},
	{
		.name = "idme_data",
		.address = CONFIG_MMC_USERDATA_ADDR,
		.size = CONFIG_MMC_USERDATA_SIZE,  /* 5 KiB */
		.partition = CONFIG_BOOT_FROM_PARTITION
	},
	{
		.name = "mbr",
		.address = 0,
		.size = 1024,  /* 1 KiB */
		.partition = 0
	},
	{
		.name = "kernel",
		.address = CONFIG_MMC_BOOTFLASH_ADDR,
		.size = CONFIG_MMC_BOOTFLASH_SIZE,  /* 14 MiB */
		.partition = 0
	},
	{
		.name = "diags_kernel",
		.address = CONFIG_MMC_BOOTDIAGS_ADDR,
		.size = CONFIG_MMC_BOOTFLASH_SIZE,  /* 14 MiB */
		.partition = 0
	},
	{
		.name = "dtb",
		.address = 0x01FC0000,
		.size = (256*1024),  /* 256 KB */
		.partition = 0
	},
	{
		.name = "system",
		.address = 0x02000000,
		.size = (450*1024*1024),  /* 450 MiB */
		.partition = 0
	},
	{
		.name = "diags",
		.address = 0x1E200000,
		.size = (64*1024*1024),  /* 64 MiB */
		.partition = 0
	},
	{
		.name = "recovery",
		.address = 0x22200000,
		.size = (64*1024*1024),  /* 64 MiB */
		.partition = 0
	},
	{
		.name = "cache",
		.address = 0x26200000,
		.size = (450*1024*1024),  /* 450 MiB */
		.partition = 0
	},
	{
		.name = "userdata",
		.address = 0x42400000,
		.size = PARTITION_FILL_SPACE,  /* based on MMC size */
		.partition = 0
	}
};

#endif /* __MX60_EANAB_BOARD_H__ */
