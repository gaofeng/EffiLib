
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "intel.h"

#define OUTPUT_BIN_PATH "bootloader.bin"

#define COND(x)	 (*(x) == '-')

int main(int argc, char *argv[])
{
    IntelHexFormat* ihf1 = NULL;
    IntelHexFormat* ihf2 = NULL;
    u32 bin_size = 0;
    u32 fill_size = 0;
    int para_index;
    u8* hexfile1 = NULL;
    u8* hexfile2 = NULL;
    u8* outfile = NULL;
    /* cmd-line parameter # */
    char *c,*p;
    char* period;
    bool out_bin = TRUE;
    bool merge = FALSE;
    u8* para_out = NULL;

    fprintf (stdout,PROGRAM" v"VERSION", \nCopyright (C) 2011 gaofeng <gaofengdiskonline@gmail.com>\n");

    if (argc == 1)
    {
        usage();
    }


    for (para_index = 1; para_index < argc; para_index++)
    {
        c = p = argv[para_index];

        if (COND(c)) {
            p = c + 2;
            /* Parameter may follow immediately after option */
            /* Skips after parameter to next option */
            if (*p == '\0')
            {
                p = argv[para_index + 1];
            }

            switch(tolower(*(++c)))
            {

            case 's':
//                 sscanf(p,"%x",&Starting_Address);
//                 Starting_Address_Setted = TRUE;
                break;
            case 'h':
                usage();
                break;
                /* New Checksum parameters */
            case 'i':
                /* Input hex file */
                if (!COND(p) && ((para_index + 1) < argc))
                {
                    p = hexfile1 = argv[++para_index];
                }
                
                if (((para_index + 1) < argc) && !COND(argv[para_index + 1]))
                {
                    hexfile2 = argv[++para_index];
                }
                break;
            case 'o':
                /*Output file*/
                if (!COND(p) && ((para_index + 1) < argc))
                {
                    para_index++;
                    para_out = argv[para_index];
                }
                break;
            default:
                usage();
            } /* switch */
        } else
            break;
        /* if option */

    } /* for Param */

    if (para_out == NULL)
    {
        para_out = hexfile1;        
    }

    outfile = (u8*)malloc(strlen(para_out) + 10);
    if (outfile == NULL)
    {
        fprintf(stderr, "Error: Not enough memory.\n");
        return -1;
    }
    strcpy(outfile, para_out);

    if ((period = strrchr(outfile,'.')) != NULL)
    {
        *(period) = '\0';
        if (memcmp(period + 1, "hex", 3) == 0)
        {
            out_bin = FALSE;
        }
    }

    if (hexfile1 != NULL)
    {
		ihf1 = IntelHexFileInput(hexfile1);
        if (!ihf1)
        {
            return -2;
        }
        if (hexfile2 != NULL)
        {
            merge = TRUE;
			ihf2 = IntelHexFileInput(hexfile2);
            if (!ihf2)
            {
                return -2;
            }
            if (IntelHexFileMerge(ihf1, ihf2) == FALSE)
            {
                fprintf(stderr, "Merge Failed\n");
                return -3;
            }
        }
    }



    if (out_bin == TRUE)
    {
        IntelHexFileFillEmptyValue(ihf1, 0xFF);
        strcat(outfile, ".bin");
        IntelHexFileToBin(ihf1, outfile);
    }
    else
    {
        strcat(outfile, ".hex");
        IntelHexFileOutput(ihf1, outfile);
    }
    return 0;
}