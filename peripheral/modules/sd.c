/*
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *//**        
 *        @file     sd.c
 *
 *        @brief    SD卡抽象层函数
 *
 *        @version  0.1
 *        @date     2012/4/25  10:31
 *
 *        @author   Hu Chunxu , hcx196@gmail.com
 *//* ==================================================================================
 *  @0.1    Hu Chunxu   2012/4/25    create orignal file
 *  @0.2    Hu Chunxu   2012/5/6     SD卡测试成功
 * =====================================================================================
 */
#include "sd.h"
#include "light.h"
#include "diskio.h"

/* 全局变量 */
FATFS Fatfs;                /* File system object for each logical drive */

/**
 *    @brief   在SD卡中创建一个新的文件
 * 
 *    @param   fp   文件对象
 *    @param   fln  文件名
 */
void sd_create_file(FIL *fp, char *fln) 
{
    /* if file exist create a new one */
    while (f_open(fp, fln, FA_CREATE_NEW|FA_WRITE) == FR_EXIST)
    {
        fln[0] = fln[0] + 1;
    }
}

/**
 *    @brief   初始化SD卡
 *
 *    @param   fs   文件系统
 */
void sd_init(FATFS *fs) 
{
    /* SD卡和SPI初始化 */
    while (disk_initialize(0))
    {
       //light_open(LIGHT7);
    }
    //light_open(LIGHT6);
    
//    /* 创建文件系统 */
//    if (!f_mount(0, fs))          /* mount a file system */
//    {          
//        light_close(LIGHT6); 
//        light_close(LIGHT7); 
//    }
}

/**
 *    @brief   关闭SD卡文件
 *
 *    @param   fp   文件对象
 */
void sd_close_file(FIL *fp) 
{
    if (fp->fs!=0)
    {
        f_close(fp);
    }
}

/**
 *    @brief   write external message to sd card files
 * 
 *    @param   fp   文件对象
 *    @param   msg  消息
 */
void sd_write_externmsg(FIL *fp, char *msg) 
{
    if ((fp->fs!=0) && (msg!=NULL))
    {
        f_printf(fp, &msg[0]);
    }
}


