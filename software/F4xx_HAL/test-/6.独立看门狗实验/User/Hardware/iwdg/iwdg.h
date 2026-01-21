/**
 ****************************************************************************************************
 * @file        wdg.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-14
 * @brief       看门狗 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台：正点原子 F407电机开发板
 * 在线视频：www.yuanzige.com
 * 技术论坛：http://www.openedv.com/forum.php
 * 公司网址：www.alientek.com
 * 购买地址：zhengdianyuanzi.tmall.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 *
 ****************************************************************************************************
 */

#ifndef __IWDG_H
#define __IWDG_H

#include "sys.h"


void iwdg_init(uint32_t prer, uint16_t rlr);        /* 初始化IWDG，并使能IWDG */
void iwdg_feed(void);                               /* 喂狗 */

#endif
