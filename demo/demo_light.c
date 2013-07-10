/*
 * =====================================================================================
 *        COPYRIGHT NOTICE
 *        Copyright (c) 2012  HUST-Renesas Lab
 *        ALL rights reserved.
 *//**        
 *        @file     demo_light.c
 *
 *        @brief    light ntshell cmd
 *
 *        @version  0.1
 *        @date     2012/3/22 21:37:42
 *
 *        @author   Hu Chunxu , hcx196@gmail.com
 *//* ==================================================================================
 *  @0.1    Hu Chunxu   2012/3/22   create orignal file
 *  @0.2    Hu Chunxu   2012/9/6    移植到asp
 * =====================================================================================
 */
#include <unistd.h>
#include <stdlib.h>

#include <t_stddef.h>
#include <stdio.h>

#include "demo_light.h"
#include "task_ntshell.h"
#include "light.h"

static void light(int argc, char **argv);
static void usage(void);

/**
 *    @brief   light命令注册
 *
 *    @param   exinf  扩展信息
 *
 *    @return  无
 *
 *    @note    若扩展信息无，传入(intptr_t)(NULL)
 */
void demo_light_initialize(intptr_t exinf)
{
    ntshell_register("light", light);
}

/**
 *    @brief   light命令
 *
 *    @param   argc  参数数量
 *    @param   argv  参数指针
 *    
 *    @note    若扩展信息无，传入(intptr_t)(NULL)
 */
static void light(int argc, char **argv)
{
    int option = 0;

    /* 再次调用 getopt() 时的下一个argv指针的索引，如果不清零，下一次使用相同命令无响应 */
    optind = 0;
    /* 不希望getopt()印出错信息 */
    opterr = 0;

    while ((option = getopt(argc, argv, "?o:c:")) != -1)
    {
        switch (option)
        {
            case 'o':
                switch (*optarg)
                {
                    case '0':
                            light_open(LIGHT0);
                            puts("light0 open!");
                        break;
                    case '1':
                            light_open(LIGHT1);
                            puts("light1 open!");
                        break;
                    case '2':
                    	    light_open(LIGHT2);
                    	    puts("light2 open!");
                        break;
                    case '3':
                    	    light_open(LIGHT3);
                    	    puts("light3 open!");
                        break;
                    case '4':
                    	    light_open(LIGHT4);
                    	    puts("light4 open!");
                        break;
                    case '5':
                    	    light_open(LIGHT5);
                    	    puts("light5 open!");
                        break;
                    case '6':
                    	    light_open(LIGHT6);
                    	    puts("light6 open!");
                        break;
                    case '7':
                    	    light_open(LIGHT7);
                    	    puts("light7 open!");
                        break;
                    default:
                        usage();
                        break;
                }
                break;

            case 'c':
                switch (*optarg)
                {
                    case '0':
                            light_close(LIGHT0);
                            puts("light0 close!");
                        break;
                    case '1':
                            light_close(LIGHT1);
                            puts("light1 close!");
                        break;
                    case '2':
                            light_close(LIGHT2);
                            puts("light2 close!");
                        break;
                    case '3':
                            light_close(LIGHT3);
                            puts("light3 close!");
                        break;
                    case '4':
                            light_close(LIGHT4);
                            puts("light4 close!");
                        break;
                    case '5':
                            light_close(LIGHT5);
                            puts("light5 close!");
                        break;
                    case '6':
                            light_close(LIGHT6);
                            puts("light6 close!");
                        break;
                    case '7':
                            light_close(LIGHT7);
                            puts("light7 close!");
                        break;
                    default:
                        usage();
                        break;
                }
                break;

            case 'h':
            case '?':
            default:
                usage();
                break;
        }
    }
}

/**
 *    @brief   lightset命令的帮助信息
 */
static void usage(void)
{
    printf("\nUsage: light [-o <open>] [-c <close>] [-h <help>]"
        "\n  -o  open the light e.g. -o 1"
        "\n  -c  close the light e.g. -c 2\n");
}

