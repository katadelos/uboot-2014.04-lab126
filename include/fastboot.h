/*
 * (C) Copyright 2008 - 2009
 * Windriver, <www.windriver.com>
 * Tom Rix <Tom.Rix@windriver.com>
 *
 * Copyright (C) 2010-2013 Freescale Semiconductor, Inc.
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
 *
 * The logical naming of flash comes from the Android project
 * Thse structures and functions that look like fastboot_flash_*
 * They come from bootloader/legacy/include/boot/flash.h
 *
 * The boot_img_hdr structure and associated magic numbers also
 * come from the Android project.  They are from
 * system/core/mkbootimg/bootimg.h
 *
 * Here are their copyrights
 *
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

#ifndef FASTBOOT_H
#define FASTBOOT_H

#include <common.h>
#include <command.h>

/* This is the interface file between the common cmd_fastboot.c and
   the board specific support.

   To use this interface, define CONFIG_FASTBOOT in your board config file.
   An example is include/configs/mx6slevkandroid.h
   ...
   #define CONFIG_FASTBOOT	        1    / * Using fastboot interface * /
   ...

*/

/* From fastboot client.. */
#define FASTBOOT_INTERFACE_CLASS     0xff
#define FASTBOOT_INTERFACE_SUB_CLASS 0x42
#define FASTBOOT_INTERFACE_PROTOCOL  0x03

#define FASTBOOT_VERSION "0.5"

/* Max size of responses from us to host */
#define FASTBOOT_RESPONSE_SIZE 65

/* The fastboot client uses a value of 2048 for the
   page size of it boot.img file format.
   Reset this in your board config file as needed. */
#ifndef CFG_FASTBOOT_MKBOOTIMAGE_PAGE_SIZE
#define CFG_FASTBOOT_MKBOOTIMAGE_PAGE_SIZE 2048
#endif

/* Lower byte shows if the read/write/erase operation in
   repeated.  The base address is incremented.
   Either 0 or 1 is ok for a default */

#define FASTBOOT_PTENTRY_FLAGS_REPEAT(n)              (n & 0x0f)
#define FASTBOOT_PTENTRY_FLAGS_REPEAT_MASK            0x0000000F

/* Writes happen a block at a time.
   If the write fails, go to next block
   NEXT_GOOD_BLOCK and CONTIGOUS_BLOCK can not both be set */
#define FASTBOOT_PTENTRY_FLAGS_WRITE_NEXT_GOOD_BLOCK  0x00000010

/* Find a contiguous block big enough for a the whole file
   NEXT_GOOD_BLOCK and CONTIGOUS_BLOCK can not both be set */
#define FASTBOOT_PTENTRY_FLAGS_WRITE_CONTIGUOUS_BLOCK 0x00000020

/* Write the file with write.i */
#define FASTBOOT_PTENTRY_FLAGS_WRITE_I                0x00000100

/* Write the file with write.trimffs */
#define FASTBOOT_PTENTRY_FLAGS_WRITE_TRIMFFS          0x00000200

/* Write the file as a series of variable/value pairs
   using the setenv and saveenv commands */
#define FASTBOOT_PTENTRY_FLAGS_WRITE_ENV              0x00000400

/* Status values */
#define FASTBOOT_OK			0
#define FASTBOOT_ERROR			-1
#define FASTBOOT_DISCONNECT		1
#define FASTBOOT_INACTIVE		2

/* Android bootimage file format */
#define FASTBOOT_BOOT_MAGIC "ANDROID!"
#define FASTBOOT_BOOT_MAGIC_SIZE 8
#define FASTBOOT_BOOT_NAME_SIZE 16
#define FASTBOOT_BOOT_ARGS_SIZE 512

#define FASTBOOT_MMC_BOOT_PARTITION_ID  1
#define FASTBOOT_MMC_USER_PARTITION_ID  0
#define FASTBOOT_MMC_NONE_PARTITION_ID -1

enum {
    DEV_SATA,
    DEV_MMC,
    DEV_NAND
};

struct cmd_fastboot_interface {
	/* This function is called when a buffer has been
	   recieved from the client app.
	   The buffer is a supplied by the board layer and must be unmodified.
	   The buffer_size is how much data is passed in.
	   Returns 0 on success
	   Returns 1 on failure

	   Set by cmd_fastboot	*/
	int (*rx_handler)(const unsigned char *buffer,
			  unsigned int buffer_size);

	/* This function is called when an exception has
	   occurred in the device code and the state
	   off fastboot needs to be reset

	   Set by cmd_fastboot */
	void (*reset_handler)(void);

	/* A getvar string for the product name
	   It can have a maximum of 60 characters

	   Set by board	*/
	char *product_name;

	/* A getvar string for the serial number
	   It can have a maximum of 60 characters

	   Set by board */
	char *serial_no;

	/* Nand block size
	   Supports the write option WRITE_NEXT_GOOD_BLOCK

	   Set by board */
	unsigned int nand_block_size;

	/* Nand oob size
	   Set by board */
	unsigned int nand_oob_size;

	/* Transfer buffer, for handling flash updates
	   Should be multiple of the nand_block_size
	   Care should be take so it does not overrun bootloader memory
	   Controlled by the configure variable CFG_FASTBOOT_TRANSFER_BUFFER

	   Set by board */
	unsigned char *transfer_buffer;

	/* How big is the transfer buffer
	   Controlled by the configure variable
	   CFG_FASTBOOT_TRANSFER_BUFFER_SIZE

	   Set by board	*/
	unsigned int transfer_buffer_size;

};

/* flash partitions are defined in terms of blocks
** (flash erase units)
*/
struct fastboot_ptentry {
	/* The logical name for this partition, null terminated */
	char name[16];
	/* The start wrt the nand part, must be multiple of nand block size */
	unsigned int start;
	/* The length of the partition, must be multiple of nand block size */
	unsigned int length;
	/* Controls the details of how operations are done on the partition
	   See the FASTBOOT_PTENTRY_FLAGS_*'s defined below */
	unsigned int flags;
	/* partition id: 0 - normal partition; 1 - boot partition */
	unsigned int partition_id;
};

struct fastboot_device_info {
	unsigned char type;
	unsigned char dev_id;
};

/* Boot img hdr structure comes from the Android project
  * See it in: system/core/mkbootimg/bootimg.h
  */
struct fastboot_boot_img_hdr {
	unsigned char magic[FASTBOOT_BOOT_MAGIC_SIZE];

	unsigned kernel_size;  /* size in bytes */
	unsigned kernel_addr;  /* physical load addr */

	unsigned ramdisk_size; /* size in bytes */
	unsigned ramdisk_addr; /* physical load addr */

	unsigned second_size;  /* size in bytes */
	unsigned second_addr;  /* physical load addr */

	unsigned tags_addr;    /* physical addr for kernel tags */
	unsigned page_size;    /* flash page size we assume */
	unsigned unused[2];    /* future expansion: should be 0 */

	unsigned char name[FASTBOOT_BOOT_NAME_SIZE]; /* asciiz product name */

	unsigned char cmdline[FASTBOOT_BOOT_ARGS_SIZE];

	unsigned id[8]; /* timestamp / checksum / sha1 / etc */
};

#ifdef CONFIG_FASTBOOT

extern struct fastboot_device_info fastboot_devinfo;

/* Prepare the fastboot environments,
  * should be executed before "fastboot" cmd
  */
void fastboot_setup(void);

/* Initizes the board specific fastboot
  * Returns 0 on success
  * Returns 1 on failure
  */
int fastboot_init(struct cmd_fastboot_interface *interface);

/* Cleans up the board specific fastboot */
void fastboot_shutdown(void);

/*
 * Handles board specific usb protocol exchanges
 * Returns 0 on success
 * Returns 1 on disconnects, break out of loop
 * Returns 2 if no USB activity detected
 * Returns -1 on failure, unhandled usb requests and other error conditions
*/
int fastboot_poll(void);

/* Is this high speed (2.0) or full speed (1.1) ?
  * Returns 0 on full speed
  * Returns 1 on high speed
  */
int fastboot_is_highspeed(void);

/* Return the size of the fifo */
int fastboot_fifo_size(void);

/* Send a status reply to the client app
  * buffer does not have to be null terminated.
  * buffer_size must be not be larger than what is returned by
  * fastboot_fifo_size
  * Returns 0 on success
  * Returns 1 on failure
  */
int fastboot_tx_status(const char *buffer, unsigned int buffer_size);

/*
 * Send some data to the client app
 * buffer does not have to be null terminated.
 * buffer_size can be larger than what is returned by
 * fastboot_fifo_size
 * Returns number of bytes written
 */
int fastboot_tx(unsigned char *buffer, unsigned int buffer_size);

/* A board specific variable handler.
  * The size of the buffers is governed by the fastboot spec.
  * rx_buffer is at most 57 bytes
  * tx_buffer is at most 60 bytes
  * Returns 0 on success
  * Returns 1 on failure
  */
int fastboot_getvar(const char *rx_buffer, char *tx_buffer);

/* The Android-style flash handling */

/* tools to populate and query the partition table */
void fastboot_flash_add_ptn(struct fastboot_ptentry *ptn);
struct fastboot_ptentry *fastboot_flash_find_ptn(const char *name);
struct fastboot_ptentry *fastboot_flash_get_ptn(unsigned n);
unsigned int fastboot_flash_get_ptn_count(void);
void fastboot_flash_dump_ptn(void);
	/* !!!! Initialize partition layout on storage !!!! */
unsigned int fastboot_flash_init_partitions(void);

/* Check the board special boot mode reboot to fastboot mode. */
int fastboot_check_and_clean_flag(void);

/* Set the board special boot mode reboot to fastboot mode. */
int fastboot_set_flag_bootloader(void);

/*fastboot command handling function*/
int do_fastboot(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

/*check if fastboot mode is requested by user*/
void check_fastboot(void);

/*Setup board-relative fastboot environment */
void board_fastboot_setup(void);

#ifdef CONFIG_FASTBOOT_STORAGE_VENDOR_PTABLE
/*Setup board-relative fastboot partition table */
void board_fastboot_load_partitions(void);
#endif

#ifdef CONFIG_FASTBOOT_STORAGE_NAND
/*Save parameters for NAND storage partitions */
void save_parts_values(struct fastboot_ptentry *ptn,
	unsigned int offset, unsigned int size);

/* Checks parameters for NAND storage partitions
  * Return 1 if the parameter is not set
  * Return 0 if the parameter has been set
  */
int check_parts_values(struct fastboot_ptentry *ptn);
#endif /*CONFIG_FASTBOOT_STORAGE_NAND*/

#else /*! CONFIG_FASTBOOT*/

/* Stubs for when CONFIG_FASTBOOT is not defined */
#define fastboot_setup() 0
#define fastboot_init(a) 1
#define fastboot_shutdown()
#define fastboot_poll() 1
#define fastboot_is_highspeed() 0
#define fastboot_fifo_size() 0
#define fastboot_tx_status(a, b) 1
#define fastboot_getvar(a, b) 1
#define fastboot_tx(a, b) 1

#define fastboot_flash_add_ptn(a)
#define fastboot_flash_find_ptn(a) NULL
#define fastboot_flash_get_ptn(a) NULL
#define fastboot_flash_get_ptn_count() 0
#define fastboot_flash_dump_ptn()
#define do_fastboot(a, b, c, d) 0

#define fastboot_quick(a) 0
#define fastboot_get_ep_num(a, b)  0
#define fastboot_dump_memory(a, b) 0
#define DBG_ALWS(x...)
#define DBG_ERR(x...)
#define DBG_DEBUG(x...)
#define DBG_INFO(x...)


#endif /*! CONFIG_FASTBOOT*/
#endif /* FASTBOOT_H */
