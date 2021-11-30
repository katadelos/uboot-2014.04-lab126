/*
 * led_control.c
 *
 * Copyright 2013 - 2015 Amazon Technologies, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <common.h>
#include <command.h>

#include <malloc.h>
#include <lp5523.h>
#include "boot_anim_data.h"

#include <asm/arch/mx6-pins.h>
#include <asm/arch/mx6sl_pins.h>

/// Animation storage
#define BOOT_ANIMATION_FILE		"bootanim.data"

/// boardrev placeholder
#define BOARD_LED_LP5523		1

// Controller I2C Addresses
#define LP5523_DRIVER_1_ADDR		0x32
#define LP5523_DRIVER_2_ADDR		0x33

// Number of controllers on board
#define NUM_CONTROLLERS		CONFIG_LP5523_NUM_DRIVERS

/* lp5523 driver addresses */
static u8 lp5523_driver_address[] = {
	LP5523_DRIVER_1_ADDR,
	0x0
};

static struct led_control board_led_control = {
	.board_rev = BOARD_LED_LP5523,
	.enable_power_led = NULL,
	.driver_info.gpio_info = {IMX_GPIO_NR(2, 7), 0},
	.driver_info.i2c_info = {CONFIG_LP5523_I2C_BUS, lp5523_driver_address},
	.power_led_info.gpio_info = {0, 0},
	.power_led_info.i2c_info = {0, 0},
};

static struct led_control* driver_handle_list[] = {
	&board_led_control,
	NULL
};

// Master Sequence
u8 lp5523_master[] = {
		0x00, 0x2A, 0x00, 0x20, 0x00, 0x02, 0x00, 0x08, 0x00, 0x10, 0x00, 0x01, 0x00, 0x04, 0x9C, 0x04,
		0x9C, 0x86, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0x00, 0x9D, 0x80, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0x00,
		0x9D, 0x80, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0x00, 0x9D, 0x80, 0x00, 0x00, 0x9C, 0x00, 0x9C, 0x83,
		0x40, 0xFF, 0x5A, 0x00, 0x40, 0xFF, 0x9D, 0x80, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0xFF, 0x9D, 0x80,
		0x40, 0xFF, 0x5A, 0x00, 0x40, 0xFF, 0x9D, 0x80, 0x00, 0x00, 0x90, 0x00, 0x94, 0x03, 0xE1, 0x00,
		0xE0, 0x80, 0xF0, 0x00, 0x91, 0x01, 0x8E, 0x11, 0xA0, 0x04, 0xE0, 0x02, 0xE0, 0x04, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Slave 1 Sequence
u8 lp5523_slave_1[] = {
		0x00, 0x2A, 0x00, 0x20, 0x00, 0x02, 0x00, 0x08, 0x00, 0x10, 0x00, 0x01, 0x00, 0x04, 0x9C, 0x04,
		0x9C, 0x86, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0x00, 0x9D, 0x80, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0x00,
		0x9D, 0x80, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0x00, 0x9D, 0x80, 0x00, 0x00, 0x9C, 0x00, 0x9C, 0x83,
		0x40, 0xFF, 0x5A, 0x00, 0x40, 0xFF, 0x9D, 0x80, 0x40, 0xFF, 0x5A, 0x00, 0x40, 0xFF, 0x9D, 0x80,
		0x40, 0xFF, 0x5A, 0x00, 0x40, 0xFF, 0x9D, 0x80, 0x00, 0x00, 0x90, 0x00, 0x94, 0x03, 0xE1, 0x00,
		0xE0, 0x80, 0xF0, 0x00, 0x91, 0x01, 0x8E, 0x11, 0xA0, 0x04, 0xE0, 0x02, 0xE0, 0x04, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


// Default patterns
static struct pattern_info pattern_boot_master = {
		.data = lp5523_master,
		.size = sizeof(lp5523_master) / sizeof(lp5523_master[0]),
		.engine1_start = 0x07,
		.engine2_start = 0x16,
		.engine3_start = 0x25,
		.address_list = { LP5523_DRIVER_1_ADDR, 0x0 }
};

static struct pattern_info pattern_boot_slave_1 = {
		.data = lp5523_slave_1,
		.size = sizeof(lp5523_slave_1) / sizeof(lp5523_slave_1[0]),
		.engine1_start = 0x07,
		.engine2_start = 0x16,
		.engine3_start = 0x25,
		.address_list = { LP5523_DRIVER_2_ADDR, 0x0 }
};

/*
 * get_driver_handle - Return the handle / index for the controller
 */
static struct led_control* get_driver_handle(int board_rev)
{
	int i = 0;
	u32 search_rev = BOARD_LED_LP5523; // Use board rev

	while (driver_handle_list[i] != NULL) {
		if (driver_handle_list[i]->board_rev == search_rev)
			return driver_handle_list[i];
		i++;
	}

	return (struct led_control*) NULL;
}

static int get_pattern_from_storage(struct pattern_info * p1, struct pattern_info * p2)
{
	int ret = -1;

	if (!p1 || !p2)
		return ret;

	/* Fox TBD - Animation files will be loaded from root filesystem in /boot/*/

	return ret;
}

static int load_default_boot_patterns(void)
{
	printf("loading default boot pattern\n");
	// load master pattern
	if (lp5523_download_animation(&pattern_boot_master))
		return -1;

	if (NUM_CONTROLLERS > 1) {
		// load slave 1 pattern
		if (lp5523_download_animation(&pattern_boot_slave_1))
			return -1;
	}
	return 0;
}

/*
 * lp5523_enable_boot_pattern - Enable the boot pattern
 */
static int lp5523_enable_boot_pattern(struct led_control* const handle)
{
	struct pattern_info pattern_program_1; //master
	struct pattern_info pattern_program_2; //slave

	memcpy(&pattern_program_1, &pattern_boot_master,
			sizeof(struct pattern_info));
	memcpy(&pattern_program_2, &pattern_boot_slave_1,
			sizeof(struct pattern_info));

	if (get_pattern_from_storage(&pattern_program_1, &pattern_program_2) != 0)
		// load the default patterns
		load_default_boot_patterns();
	else {
		// load master pattern
		if (lp5523_download_animation(&pattern_program_1))
			return -1;

		if (NUM_CONTROLLERS > 1) {
			// load slave 1 pattern
			if (lp5523_download_animation(&pattern_program_2))
				return -1;
		}
	}

	// Run the pattern
	return lp5523_enable_animation();
}

/*
 * initialize_leds - Perform any leds related initialization
 */
void initialize_leds() {
	int board_rev = BOARD_LED_LP5523;
	struct led_control* handle = get_driver_handle(board_rev);

	if (NULL == handle) {
		printf("initialize_leds - Invalid control handle\n");
		return;
	}

	if (lp5523_register_control(handle)) {
		printf("initialize_leds - Failed to register handle\n");
		return;
	}

	if (lp5523_pattern_active() == 0)
		// Download the boot animation
		lp5523_enable_boot_pattern(handle);
}

/* ------------------------------------------------------------------
 *                           COMMAND CONTROL
 * ------------------------------------------------------------------
 */

/*
 * trigger_leds - Trigger necessary sequence to enable LED color
 */
static int trigger_leds(LED_PATTERN_T color)
{
	struct led_control* handle = get_driver_handle(BOARD_LED_LP5523);

	if (NULL == handle) {
		printf("initialize_leds - Invalid control handle\n");
		return -1;
	}

	if (lp5523_register_control(handle)) {
		printf("initialize_leds - Failed to register handle\n");
		return -1;
	}

	return lp5523_show_pattern(color);
}

/*
 * get_led_cmd - Return the led command
 */
static enum led_cmd get_led_cmd(char *var)
{
	if (!var)
		return -1;

	if (strcmp(var, "off") == 0)
		return LED_OFF;

	if (strcmp(var, "on") == 0)
		return LED_ON;

	return -1;
}

/*
 * get_color - Return the led color
 */
static LED_PATTERN_T get_color(char* color)
{
	if (color == NULL)
		return LED_PATTERN_INVALID;

	switch (color[0]) {
	case 'r':
	case 'R':
		return LED_PATTERN_SOLID_RED;
	case 'g':
	case 'G':
		return LED_PATTERN_SOLID_GREEN;
	case 'b':
	case 'B':
		return LED_PATTERN_SOLID_BLUE;
	case 'a':
	case 'A':
		return LED_PATTERN_SOLID_WHITE;
	default:
		return LED_PATTERN_INVALID;
	}
}

/*
 * do_led - Command line run routine
 */
int do_led (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	enum led_cmd cmd;
	LED_PATTERN_T color;

	/* Validate arguments */
	if (argc != 3) {
		printf("invalid argument(s)\n");
		return -1;
	}
	cmd = get_led_cmd(argv[2]);
	color = get_color(argv[1]);
	if (cmd < 0) {
		printf("invalid command\n");
		return -1;
	}
	if (color == LED_PATTERN_INVALID) {
		printf("invalid color\n");
		return -1;
	}

	/* set leds */
	if (cmd == LED_OFF) {
		color = LED_PATTERN_NONE;
	}
	if (trigger_leds(color) != 0) {
		printf("failed to set LEDs\n");
		return -1;
	}

	return 0;
}


U_BOOT_CMD(
	led, 3, 1, do_led,
	"Turn on/off leds",
	"[red|green|blue|all] [on|off]\n"
);
