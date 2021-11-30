make ARRCH=arm CROSS_COMPILE=arm-linux-gnueabi- mx6sleanab_config
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
rm -f u-boot.prod.bin
dd if=u-boot.imx of=u-boot.prod.bin bs=1024 seek=1

make ARRCH=arm CROSS_COMPILE=arm-linux-gnueabi- mx6sleanab_mfgtool_config
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
rm -f u-boot.mfgtool.bin
dd if=u-boot.imx of=u-boot.mfgtool.bin


make ARRCH=arm CROSS_COMPILE=arm-linux-gnueabi- mx6sleanab_bist_config
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-
rm -r u-boot.bist.bin
dd if=u-boot.bin of=u-boot.bist.bin

dd if=u-boot.prod.bin of=u-boot.bin bs=1024
dd if=u-boot.bist.bin of=u-boot.bin bs=1024 seek=192




