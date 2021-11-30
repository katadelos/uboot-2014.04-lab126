/*
 * Copyright (c) 2013 - 2015 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * PROPRIETARY/CONFIDENTIAL
 *
 * Use is subject to license terms.
 */

#ifndef __BOOTANIMDATA_H__
#define __BOOTANIMDATA_H__


#define BOOT_ANIM_HEADER_SEQUENCE 0xFACEF00D
#define BOOT_ANIM_NAND_PAGE_OFFSET  0;
#define ENGINE_DATA_SIZE (96 * 2) // 96 * 16-bit Instructions


typedef struct engine_data_s {
    u32 size;
    u8 data[ENGINE_DATA_SIZE];
    u8 engine1_offset;
    u8 engine2_offset;
    u8 engine3_offset;
}EngineDataT;

typedef struct boot_anim_data_s {
    u32 header;
    u32 version;
    EngineDataT program1_data;
    EngineDataT program2_data;
    EngineDataT program3_data;
    EngineDataT program4_data;
}BootAnimationT;

#endif //__BOOTANIMDATA_H__
