/***************************************************************************************************
*                    (c) Copyright 1992-2009 Embedded Products Research Center
*                                       All Rights Reserved
*
*\File          usdl_crc.h
*\Description   提供计算CRC校验值的各种算法      
*\Log           2008.09.19    Ver 1.0    gaofeng
*               创建文件。
***************************************************************************************************/

#ifndef _USDL_CRC_H
#define _USDL_CRC_H

#include "type.h"

/*CRC默认种子*/
#define CRC_DEFAULT_SEED    0xFFFFFFFF

/***************************************************************************************************
*\Function      GetCRC16
*\Description   计算CRC16校验值,默认采用CRC-CCITT多项式.
*\Parameter     seed    种子值
*\Parameter     src     缓冲区起始指针
*\Parameter     length  缓冲区长度
*\Return        u16     指定缓冲区的校验值.
*\Log           2010.01.11    Ver 1.0    高峰
*               创建函数。
***************************************************************************************************/
u16 GetCRC16(u16 seed, u8* src, u32 length);

/***************************************************************************************************
*\Function      GetCRC16_IBM
*\Description   根据以src起始长len的串生成16位CRC-IBM校验值
*\Parameter     seed    种子值
*\Parameter     src     源数据
*\Parameter     len     数据长度
*\Return        u16
*\Note          CRC16-IBM = X16 + X15 + X2 + 1
*\Log           2008.09.19    Ver 1.0    gaofeng
*               创建函数。
***************************************************************************************************/
u16 GetCRC16_IBM(u16 seed, u8* src, u32 len);

/***************************************************************************************************
*\Function      GetCRC16_CCITT
*\Description   根据以src起始长len的串生成16位CRC_CCITT交验数据
*\Parameter     seed    种子值
*\Parameter     src     源数据
*\Parameter     len     数据长度
*\Return        u16
*\Log           2008.9.19   Ver 1.0    高峰
*               创建函数。
***************************************************************************************************/
u16 GetCRC16_CCITT(u16 seed, u8* src, u32 len);

/***************************************************************************************************
*\Function      GetCRC32
*\Description   对src所指向的长度为length的缓冲区计算CRC32校验值
*\Parameter     seed    种子值
*\Parameter     src     数据缓冲区指针
*\Parameter     len     数据长度
*\Return        u16
*\Note          多项式为0xEDB88320。
*\Log           2009.8.24   Ver 1.0    高峰
*               创建函数。
***************************************************************************************************/
u32 GetCRC32(u32 seed, u8* src,u32 len);

/***************************************************************************************************
*\Function      GetCRC32_IEEE
*\Description   按通用标准方法计算从src开始的长度为len缓冲区的CRC32校验值。
*\Parameter     src     缓冲区起始指针
*\Parameter     len     缓冲区长度
*\Return        u32     校验值
*\Note          (1) 种子为0xFFFFFFFF，多项式为0xEDB88320，最终校验结果会取做一次位取反操作。
*\Note          (2) 应用于Ethernet,MPEG-2,PNG,POSIX cksum等。
*\Log           2010.01.11    Ver 1.0    高峰
*               创建函数。
***************************************************************************************************/
u32 GetCRC32_IEEE(u8* src, u32 len);

#endif /*_USDL_CRC_H*/
