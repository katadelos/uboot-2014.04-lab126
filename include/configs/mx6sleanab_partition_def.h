#define CONFIG_MMC_BOOTFLASH            1
#define CONFIG_MMC_BOOTFLASH_ADDR       0x41000
#define CONFIG_MMC_BOOTFLASH_SIZE       (14*1024*1024) /* 14 MiB */
#define CONFIG_MMC_BOOTDIAGS_ADDR       0xE41000
#define CONFIG_MMC_USERDATA_ADDR        0x80000
#define CONFIG_MMC_USERDATA_SIZE        (5*1024)
#define CONFIG_MMC_BIST_ADDR            (192*1024)
#define CONFIG_MMC_BIST_SIZE            (256*1024)
#define CONFIG_MMC_MAX_TRANSFER_SIZE    (0xFFFF * 512)

#define CONFIG_BOOT_PARTITION_ACCESS
#define CONFIG_BOOT_FROM_PARTITION      1
#define CONFIG_NUM_PARTITIONS 13

#define CONFIG_IDME_UPDATE_ADDR         0x3F000

