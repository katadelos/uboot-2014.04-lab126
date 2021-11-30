# (C) Copyright 2013 Freescale Semiconductor, Inc.
#
# SPDX-License-Identifier:	GPL-2.0+
#

obj-y  := mx6sl_wario.o
obj-y  += pinmux.o
obj-$(CONFIG_PMIC_MAX77696) += pmic.o

extra-$(CONFIG_USE_PLUGIN) :=  plugin.bin
$(obj)/plugin.bin: $(obj)/plugin.o
	$(OBJCOPY) -O binary --gap-fill 0xff $< $@
