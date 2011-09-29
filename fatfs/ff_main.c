#include <stdio.h>
#include "ff.h"

void main(void)
{
    FATFS fs[2];         /* Work area (file system object) for logical drives */
    FIL fsrc, fdst;      /* file objects */
    BYTE buffer[1540];   /* file copy buffer */
    BYTE buffer2[100];
    FRESULT res;         /* FatFs function common result code */
    UINT br;             /* File read count */
    UINT bw;             /* File write count */

    /* Register work area for each volume (Always succeeds regardless of disk status) */
    f_mount(0, &fs[0]);
    f_mkfs(0, 0, 1024);

    /* Open source file on the drive 1 */
    res = f_open(&fsrc, "0:srcfile.dat", FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

    memset(buffer, 0xEE, sizeof(buffer));
    res = f_write(&fsrc, buffer, sizeof(buffer), &bw);    /* Read a chunk of src file */
    res = f_lseek(&fsrc, 1536);
    res = f_lseek(&fsrc, 1024);
    res = f_read(&fsrc, buffer2, 10, &br);

    /* Close open files */
    f_close(&fsrc);

    /* Unregister work area prior to discard it */
    f_mount(0, NULL);
}

