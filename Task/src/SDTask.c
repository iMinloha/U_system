#include "../inc/SDTask.h"
#include "fatfs.h"
#include "u_stdio.h"


FRESULT MyFile_Res;

void FatFs_Check(void){
    BYTE work[_MAX_SS];
    FATFS_LinkDriver(&SD_Driver, SDPath);
    MyFile_Res = f_mount(&SDFatFS,SDPath,1);

    if (MyFile_Res == FR_OK)
        u_print("\r\nSD File system been mount\r\n");
    else {
        u_print("SD card have not file system, waiting init......\r\n");

        MyFile_Res = f_mkfs(SDPath, FM_FAT32, 0, work, sizeof work);		//格式化SD卡，FAT32，簇默认大小16K

        u_print("Init Result: %d\r\n", MyFile_Res);

        if (MyFile_Res == FR_OK) u_print("SD card init succeed\r\n");
        else u_print("Init Faild, please replace SD card\r\n");
    }
}