#include <stdio.h>
#include <time.h>

#include "integer.h"
#include "diskio.h"
#include "ff.h"

FILE *pDisk0 = 0;
TCHAR* DiskFileName = "Disk0.vdisk";

#define DISK_SIZE (1024 * 1024 * 5)
#define SECTOR_SIZE 512

#define SECTOR_COUNT (DISK_SIZE / SECTOR_SIZE)

DSTATUS disk_initialize(BYTE Drive)
{
    BYTE buf[1024];
    int i;

    memset(buf, 0x00, sizeof(buf));
    /*尝试打开一个一个已存在的文件*/
    pDisk0 = fopen(DiskFileName, "rb+");
    if (pDisk0 == 0)
    {
        /*创建新文件，将文件内容初始化*/
        pDisk0 = fopen(DiskFileName, "wb+");
        if (pDisk0 == 0)
        {
            return STA_NOINIT;
        }
        for (i = 0; i < DISK_SIZE; i++)
        {
            if (fwrite(buf, 1, 1, pDisk0) == 0)
            {
                return STA_NOINIT;
            }
        }
    }

    //fclose(pDisk0);
    return 0;
}

DSTATUS disk_status(BYTE Drive)
{
    return 0;
}

DRESULT disk_read(BYTE Drive, BYTE* Buffer, DWORD SectorNumber, BYTE SectorCount)
{
    if (SectorNumber + SectorCount > SECTOR_COUNT)
    {
        return RES_ERROR;
    }
    fseek(pDisk0, SectorNumber * SECTOR_SIZE, SEEK_SET);
    if (fread(Buffer, 1, SECTOR_SIZE * SectorCount, pDisk0) != (SECTOR_SIZE * SectorCount))
    {
        return RES_ERROR;
    }
    fflush(pDisk0);
    return RES_OK;
}

DRESULT disk_write(BYTE Drive, const BYTE* Buffer, DWORD SectorNumber, BYTE SectorCount)
{
    if (SectorNumber + SectorCount > SECTOR_COUNT)
    {
        return RES_ERROR;
    }
    fseek(pDisk0, SectorNumber * SECTOR_SIZE, SEEK_SET);
    if (fwrite(Buffer, 1, SECTOR_SIZE * SectorCount, pDisk0) != (SECTOR_SIZE * SectorCount))
    {
        return RES_ERROR;
    }
    fflush(pDisk0);
    return RES_OK;
}

DRESULT disk_ioctl(BYTE Drive, BYTE Command, void* Buffer)
{
    DRESULT res = RES_PARERR;
    switch (Command)
    {
    case GET_SECTOR_COUNT:
        ST_DWORD(Buffer, SECTOR_COUNT);
        res = RES_OK;
        break;
    case CTRL_SYNC:
        res = RES_OK;
        break;
    default:
        break;
    }
    return res;
}

DWORD get_fattime (void)
{
    time_t rt;
    struct tm* t;
    DWORD fat_t;

    time(&rt);
    t = localtime(&rt);

    t->tm_year = t->tm_year + 1900 - 1980;
    if (t->tm_year > 127)
    {
        t->tm_year = 0;
    }
    t->tm_mon += 1;
    if (t->tm_mon > 12)
    {
        t->tm_mon = 12;
    }
    if (t->tm_mday > 31)
    {
        t->tm_mday = 31;
    }
    if (t->tm_hour > 23)
    {
        t->tm_hour = 23;
    }
    if (t->tm_min > 59)
    {
        t->tm_min = 59;
    }
    t->tm_sec /= 2;
    if (t->tm_sec > 29)
    {
        t->tm_sec = 29;
    }
    fat_t = t->tm_sec | (t->tm_min << 5) | (t->tm_hour << 11) |
        (t->tm_mday << 16) | (t->tm_mon << 21) | (t->tm_year << 25);
    return fat_t;
}