/*
Intel Hexadecimal object file 32 bits format
Change log:
At the beginning
1. Can read in a intel hex format file.
2. Can get the intel hex file's data size.
2011/3/20
1. Fill Hex file with Empty byte value(0x00-0xFF).
2011/3/22
1. Merge two hex file.
2. Convert hex file to hex or bin file.
2011/4/3
1. opt hex file output.


TODOs:
1. Add Operation process and result statistics.
4. Add support to Segment address model data process.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "intel.h"

struct Block
{
	u32 Address;
	u32 Length;
	u8* Data;
	struct Block* Next;
};

enum ModeType
{
	MODE_NONE,
	MODE_SEGMENTED,
	MODE_LINEAR
};

struct IntelHexFormat
{
	u32 TotalLength;
	u32 StartLinearAddress;
	struct Block* BlockHead;
	struct Block* BlockTail;
	bool EndOfFile;
	u32 AddressBase;
	enum ModeType FileMode;
	u32 StartSegmentAddress;
};

u8 LineBuffer[1024];

void usage(void)
{
    fprintf (stderr,
        "\n"
        "usage: "PROGRAM" [-s address] -i hexfile1 [hexfile2] [-o binfile]\n"
        "Options:\n"
        "  -s [address]  Starting address in hex\n"
        "  -i [hexfile1] Intel Hex format file path\n"
        "  -f [value]    Empty byte value in hex (default==0x00), or may output multi bin files\n"
        "  -o [file]     Output Hex or Bin(default) file path\n"
    );
    exit(1);
}


static bool AddBlock(struct IntelHexFormat* hex_file, u32 address, u8* data, u32 data_len)
{
    struct Block* new_block = NULL;
    struct Block* append_block = NULL;
    struct Block* add_block = NULL;
    struct Block* current_block = NULL;
    u8* temp;

    //Append to an exist block
    if (hex_file->BlockTail != NULL)
    {
        if ((hex_file->BlockTail->Address + hex_file->BlockTail->Length) == address)
        {
            append_block = hex_file->BlockTail;
        }
        else if ((hex_file->BlockTail->Address + hex_file->BlockTail->Length) <= address)
        {
            add_block = hex_file->BlockTail;
        }
        else
        {
            current_block = hex_file->BlockHead;
            while (current_block != NULL)
            {
                if (address == (current_block->Address + current_block->Length))
                {
                    append_block = current_block;
                    break;
                }
                else if ((current_block->Address + current_block->Length) < address)
                {
                    add_block = current_block;
                }
                current_block = current_block->Next;
            }
        }
    }
    if (append_block != NULL)
    {
        temp = realloc((void*)(append_block->Data), append_block->Length + data_len);
        if (temp == NULL)
        {
            goto ERROR_MEMORY;
        }
        append_block->Data = temp;
        memcpy(append_block->Data + append_block->Length, data, data_len);
        append_block->Length += data_len;
    }
    else
    {
        if (((add_block == NULL) && (hex_file->BlockHead) && (address > hex_file->BlockHead->Address)) ||
            (add_block != NULL) && ((add_block->Next != NULL) && (address + data_len > add_block->Next->Address)))
        {
            printf("Error: Data Overlap in Hex File.\n");
            return FALSE;
        }
        //Create a new block object;
        new_block = (struct Block*)malloc(sizeof(struct Block));
        if (new_block == NULL)
        {
            goto ERROR_MEMORY;
        }
        new_block->Address = address;
        new_block->Data = (u8*)malloc(data_len);
        if (new_block->Data == NULL)
        {
            goto ERROR_MEMORY;
        }
        memcpy(new_block->Data, data, data_len);
        new_block->Length = data_len;
        new_block->Next = NULL;

        if (hex_file->BlockHead == NULL)
        {
            hex_file->BlockHead = hex_file->BlockTail = new_block;
        }
        else if (add_block == NULL)
        {
            new_block->Next = hex_file->BlockHead;
            hex_file->BlockHead = new_block;
        }
        else
        {
            new_block->Next = add_block->Next;
            add_block->Next = new_block;
            add_block = new_block->Next;
        }
		while ((hex_file->BlockTail) && (hex_file->BlockTail->Next))
		{
			hex_file->BlockTail = hex_file->BlockTail->Next;
		}
    }

    return TRUE;

ERROR_MEMORY:
    printf("Insufficient Memory.\n");
    free(new_block->Data);
    free(new_block);
    return FALSE;
}

/***************************************************************************************************
*\Function      HexToByteArray
*\Description   HEX字符串转化为字节数组
*\Parameter     data        缓冲区指针
*\Parameter     data_len    缓冲区长度
*\Parameter     hex         HEX字符串
*\Parameter     skip_invalid    是否跳过非法字符(非0~9,a~f)
*\Return        u32     所转化的字节数组长度，若skip_invalid为FALSE，在hex字符串中遇到非法字符
                        则立刻返回0.
*\Log           2010.11.10    Ver 1.0    高峰
*               创建函数。
***************************************************************************************************/
u32 HexToByteArray(u8* data, u32 data_len, u8* hex, bool skip_invalid)
{
    u32 str_len = 0;
    bool odd_digit = FALSE;
    u32 i;
    u8 ch;
    u8 tmp;

    str_len = strlen((char*)hex);
    if (str_len > data_len * 2)
    {
        str_len = data_len * 2;
    }

    for (i = 0; i < str_len; ++i)
    {
        ch = hex[i];

        if (ch >= '0' && ch <= '9')
        {
            tmp = ch - '0';
        }
        else if (ch >= 'a' && ch <= 'f')
        {
            tmp = ch - 'a' + 10;
        }
        else if (ch >= 'A' && ch <= 'F')
        {
            tmp = ch - 'A' + 10;
        }
        else
        {
            if (skip_invalid == TRUE)
            {
                continue;
            }
            else
            {
                return 0;
            }
        }
        if (odd_digit) {
            *data |= tmp;
            data++;
            odd_digit = FALSE;
        } else {
            *data = tmp << 4;
            odd_digit = TRUE;
        }
    }
    return (str_len + 1) / 2;
}

const u8 HexList[] = "0123456789ABCDEF";

void BytesMapHexStr(u8* str, u8* bytes, u32 len)
{
    u16 i = 0x00;
    for(i = 0; i < len; i++)
    {
        str[i*2]   = HexList[bytes[i]/16];
        str[i*2+1] = HexList[bytes[i]%16];
    }
    str[len * 2] = 0x00;
}

//return false means stop parsing file.
static bool ParseOneLine(struct IntelHexFormat* hex_file, u32 line_no, u8* str)
{
    u32 index;
    u32 str_len;
    u8 data[256];
    u32 data_len;
    u16 address_field;
    u8 cs;
    u32 split;

    //
    // keep reading lines until something returnable comes along
    //
    index = 0;
    
    //
    // quietly ignore blank lines
    //
    if (str[0] == '\n')
    {
        return TRUE;
    }

    str_len = strlen(str);

    //
    // If it doesn't start with a colon, it's a garbage line.
    // Warn, and then ignore it.
    //
    if ((str_len < 11) || (str[0] != ':'))
    {
        printf("Warning: garbage lines at %d\n", line_no);
        return TRUE;
    }
    str[str_len - 1] = 0x00;
    data_len = HexToByteArray(data, 256, str + 1, TRUE);
    //
    if (data_len != ((str_len - 1) / 2))
    {
        return FALSE;
    }
    //Check Byte count
    if (data[0] != (data_len - 5))
    {
        return FALSE;
    }
    //Checksum
    cs = 0;
    for (index = 0; index < data_len; index++)
    {
        cs += data[index];
    }
    if (cs != 0)
    {
        return FALSE;
    }
    //Big Endian Address field
    address_field = data[1] * 256 + data[2];
    
    switch (data[3])
    {
    case 0:
        //
        // data
        //
        if (hex_file->FileMode == MODE_LINEAR)
        {
            //
            // linear addressing model
            //
            if (((u64)hex_file->AddressBase + address_field < ((u64)1 << 32)) &&
                ((u64)hex_file->AddressBase + address_field + data[0] > ((u64)1 << 32)))
            {
                /*calculate first half data length*/
                split = (u32)(((u64)1 << 32) - hex_file->AddressBase - address_field);
                //first of all, add second half data to address 0.
                if (AddBlock(hex_file, 0, data + 4 + split, data[0] - split) == FALSE)
                {
                    return FALSE;
                }
                hex_file->TotalLength += (data[0] - split);
                data[0] = (u8)split;
            }
            if (AddBlock(hex_file, hex_file->AddressBase + address_field, data + 4, data[0]) == FALSE)
            {
                return FALSE;
            }
            hex_file->TotalLength += data[0];
        }
        else if (hex_file->FileMode == MODE_SEGMENTED)
        {
            //
            // segmented addressing model
            //
            if (address_field + data[0] > (1L << 16))
            {
                //
                // If the record would cross a segment boundary,
                // wrap around to the beginning of the segment.  We
                // do this by using "pushback" to remember additonal
                // data.
                //
                split = (1L << 16) - address_field;
                /*
                pushback =
                    new srecord::record
                    (
                        srecord::record::type_data,
                        address_base + ((address_field + split) & 0xFFFF),
                        buffer + 4 + split,
                        buffer[0] - split
                    );
                buffer[0] = split;
                */
            }
        }
        else
        {
            printf("Error: Invalid Hex file\n");
        }
        break;

    case 1:
        //
        // end-of-file record
        //
        if (data[0] != 0)
        {
            printf("line %d:Length field must be zero.\n", line_no);
            return FALSE;
        }
        if (address_field != 0)
        {
            printf("line %d:Length field must be zero.\n", line_no);
            return FALSE;
        }
        hex_file->EndOfFile = TRUE;
        return FALSE;

    case 2:
        //
        // extended segment address record
        //
        // Set the base address and addressing mode,
        // and then loop for another record, this one
        // isn't visable to the caller.
        //
        if (data[0] != 0)
        {
            printf("line %d:Length field must be zero.\n", line_no);
            return FALSE;
        }
        if (address_field != 0)
        {
            printf("line %d:Length field must be zero.\n", line_no);
            return FALSE;
        }
        hex_file->AddressBase = address_field << 4;
        hex_file->FileMode = MODE_SEGMENTED;
        return TRUE;;

    case 3:
        //
        // start segment address record
        //
        if (data[0] != 4)
        {
            printf("line %d:Length field must be 4.\n", line_no);
            return FALSE;
        }
        if (address_field != 0)
        {
            printf("line %d:Length field must be zero.\n", line_no);
            return FALSE;
        }
        hex_file->StartSegmentAddress = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];

        return TRUE;

    case 4:
        //
        // extended linear address record
        //
        // Set the base address and addressing mode,
        // and then loop for another record, this one
        // isn't visable to the caller.
        //
        if (data[0] != 2)
        {
            printf("line %d:Length field must be 2.\n", line_no);
            return FALSE;
        }
        if (address_field != 0)
        {
            printf("line %d:Length field must be zero.\n", line_no);
            return FALSE;
        }
        hex_file->AddressBase = (data[4] << 24) + (data[5] << 16);
        hex_file->FileMode = MODE_LINEAR;
        return TRUE;

    case 5:
        //
        // start linear address record
        //
        if (data[0] != 4)
        {
            printf("line %d:Length field must be 4.\n", line_no);
            return FALSE;
        }
        if (address_field != 0)
        {
            printf("line %d:Length field must be zero.\n", line_no);
            return FALSE;
        }
        hex_file->StartLinearAddress = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + data[7];
        return TRUE;
    }

    return TRUE;
}
/*
Read in a intel hex format file into internal. return TRUE if success.
*/
IntelHexFormat* IntelHexFileInput(const u8* file_name)
{
    FILE* fp = NULL;
    u32 lineno = 0;
	struct IntelHexFormat* hex_file = NULL;
	
	hex_file = malloc(sizeof(struct IntelHexFormat));
	if (!hex_file)
	{
		printf("Error: Not enough Memory.\n");
		return NULL;
	}
	memset(hex_file, 0x00, sizeof(struct IntelHexFormat));

    fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("Can't open file %s when read hex file.\n", file_name);
        return NULL;
    }
    hex_file->FileMode = MODE_LINEAR;
    lineno = 1;
    fprintf(stdout, "Read in Intel HEX file %s...\n", file_name);
    while (fgets((char*)LineBuffer, sizeof(LineBuffer), fp) != NULL)
    {
        fprintf(stdout, "Processing line %d...\r", lineno);
        if (ParseOneLine(hex_file, lineno, LineBuffer) == FALSE)
        {
            break;
        }
        lineno++;
    }
	fclose(fp);
	fprintf(stdout, "Processing complete.          \n");

    fprintf(stdout, "Data length\t %d\n", hex_file->TotalLength);

    return hex_file;
}

u32 IntelHexFileFillEmptyValue(struct IntelHexFormat* ihf, u8 empty_value)
{
    u32 count = 0;
    struct Block* current;
    struct Block* next;
    struct Block* tmp;
    u32 gap_len = 0;
    u8* gap_data = NULL;
    
    current = ihf->BlockHead;

    while ((current != NULL) && (current->Next != NULL))
    {
        next = current->Next;
        gap_len = next->Address - current->Address - current->Length;
        if (gap_len == 0)
        {
            break;
        }
        count += gap_len;
 
        gap_data = malloc(gap_len);
        tmp = malloc(sizeof(struct Block));
        if ((tmp == NULL) || (gap_data == NULL))
        {
            printf("Error: not enough memory!\n");
            return 0;
        }
        memset(gap_data, empty_value, gap_len);
        tmp->Address = current->Address + current->Length;
        tmp->Data = gap_data;
        tmp->Length = gap_len;
        tmp->Next = next;
        current->Next = tmp;
        current = tmp;
    }
    return count;
}
/*
Merge ifh1 and ifh2 to ifh1.
*/
bool IntelHexFileMerge(struct IntelHexFormat* ihf1, struct IntelHexFormat* ihf2)
{
    struct IntelHexFormat* temp = NULL;
    if ((ihf1->EndOfFile == FALSE) || (ihf2->EndOfFile == FALSE))
    {
        return FALSE;
    }
    if (ihf1->FileMode != ihf2->FileMode)
    {
        fprintf(stdout, "Error: Different file mode!\n");
        return FALSE;
    }
    if (ihf2->BlockHead == NULL)
    {
        /*Need NOT to merge*/
        return TRUE;
    }
    if (ihf1->BlockHead == NULL)
    {
        /*Need NOT to merge*/
        ihf1 = ihf2;
        return TRUE;
    }
    if (ihf1->BlockHead->Address > (ihf2->BlockTail->Address + ihf2->BlockTail->Length))
    {
        temp = ihf1;
        ihf1 = ihf2;
        ihf2 = temp;
    }

    if ((ihf1->BlockTail->Address + ihf1->BlockTail->Length) > ihf2->BlockHead->Address)
    {
        fprintf(stdout, "Error: Hex file Overlapped!\n");
        return FALSE;
    }
    
    /*Start merge*/
    ihf1->BlockTail->Next = ihf2->BlockHead;
    ihf1->BlockTail = ihf2->BlockTail;
    ihf1->TotalLength += ihf2->TotalLength;

    ihf1->StartLinearAddress = 0xFFFFFFFF;

    return TRUE;
}

static u8 GetCheckSum(u8* buf, u32 len)
{
    u32 i;
    u8 cs = 0;
    for (i = 0; i < len; i++)
    {
        cs = cs - buf[i];
    }
    return cs;
}
/*Output data to hex_path file following Intel HEX format*/
bool IntelHexFileOutput(struct IntelHexFormat* ihf, u8* hex_path)
{
    //max line length is 42
    u8 str[80];
    u8 buf[37];
    u8 buf_len = 0;
    u32 ulba = 0xFFFF;
    u32 addr = 0;
    u32 data_len = 0;
    u32 rest_len = 0;
    u32 max_len = 0;
    u32 offset = 0;
    struct Block* current;
    FILE* fp;
    if ((ihf->EndOfFile == FALSE) || (ihf->BlockHead == NULL))
    {
        return FALSE;
    }

    if ((fp = fopen(hex_path, "w")) == NULL)
    {
        fprintf(stdout, "Error: Can't create file %s", hex_path);
        return FALSE;
    }

    fprintf(stdout, "Output Intel HEX file %s\n", hex_path);

    /*determine each data record's max length*/
    if (ihf->TotalLength > 0xFFFF)
    {
        max_len = 32;
    }
    else
    {
        max_len = 16;
    }

    current = ihf->BlockHead;
    addr = current->Address;
    rest_len = current->Length;
    offset = 0;

    while (current != NULL)
    {
        /*Extended Linear Address*/
        if ((addr >> 16) != ulba)
        {
            ulba = addr >> 16;
            memset(buf, 0x00, sizeof(buf));
            buf[0] = 2;
            buf[3] = 0x04;
            buf[4] = (u8)(ulba >> 8);
            buf[5] = (u8)(ulba);
            buf[6] = GetCheckSum(buf, 6);
            buf_len = 7;
            BytesMapHexStr(str, buf, buf_len);
            fprintf(fp, ":%s\n", str);
        }

        if (rest_len > max_len)
        {
            data_len = max_len;
        }
        else
        {
            data_len = rest_len;
        }
        if ((addr + data_len + 1) >> 16 != ulba)
        {
            data_len = ((ulba + 1) << 16) - addr;
        }

        memset(buf, 0x00, sizeof(buf));
        buf[0] = (u8)data_len;
        buf[1] = (u8)(addr >> 8);
        buf[2] = (u8)addr;
        buf[3] = 0x00;

        memcpy(buf + 4, current->Data + offset, data_len);
        buf_len = (u8)(4 + data_len);
        buf[buf_len] = GetCheckSum(buf, 4 + data_len);
        buf_len++;
        BytesMapHexStr(str, buf, buf_len);
        fprintf(fp, ":%s\n", str);
        
        offset += data_len;
        rest_len -= data_len;
        addr += data_len;

        if (rest_len == 0)
        {
            current = current->Next;
            if (current != NULL)
            {
                addr = current->Address;
                rest_len = current->Length;
                offset = 0;
            }
        }
    }
    
    //Start Linear Address Record
    if (ihf->StartLinearAddress != 0xFFFFFFFF)
    {
        memset(buf, 0x00, sizeof(buf));
        buf[0] = 4;

        buf[3] = 0x05;
        buf[4] = (u8)(ihf->StartLinearAddress >> 24);
        buf[5] = (u8)(ihf->StartLinearAddress >> 16);
        buf[6] = (u8)(ihf->StartLinearAddress >> 8);
        buf[7] = (u8)(ihf->StartLinearAddress);

        buf[8] = GetCheckSum(buf, 8);
        buf_len = 9;
        BytesMapHexStr(str, buf, buf_len);
        fprintf(fp, ":%s\n", str);
    }

    //End Of File record
    memset(buf, 0x00, sizeof(buf));
    buf[0] = 0;

    buf[3] = 0x01;
    buf[4] = GetCheckSum(buf, 4);
    buf_len = 5;
    BytesMapHexStr(str, buf, buf_len);
    fprintf(fp, ":%s\n", str);

    fprintf(stdout, "Data length\t %d\n", ihf->TotalLength);

    return TRUE;
}

/*
If hex file has multi data section, then will output several bin file, and from the second file, it's
name will append start address.
*/
bool IntelHexFileToBin(struct IntelHexFormat* ihf, u8* file_path)
{
    FILE* fp = NULL;
    u8 addr_str[9];
    struct Block* current = NULL;
    u32 start_addr;
    u32 current_addr;
    bool first_block = TRUE;
    u8* period = NULL;
	u8* bin_file_path = NULL;
    
	bin_file_path = malloc(strlen(file_path) + 8 + 1);
	if (bin_file_path == NULL)
	{
		fprintf(stdout, "Error: Not enough memory.\n");
		return FALSE;
	}
	strcpy(bin_file_path, file_path);
    fp = fopen(bin_file_path, "wb");
    if (fp == NULL)
    {
        fprintf(stdout, "Can't create file %s", bin_file_path);
		free(bin_file_path);
        return FALSE;
    }
	fprintf(stdout, "Output file %s created.\n", bin_file_path);

    current = ihf->BlockHead;

    while (current != NULL)
    {
        if (first_block == TRUE)
        {
            start_addr = current->Address;
            current_addr = current->Address;
            first_block = FALSE;
			fprintf(stdout, "Start Address: 0x%0*X\n", 8, start_addr);
		}
        /*Start a new data section*/
        if (current_addr != current->Address)
        {
            current_addr = current->Address;
            fclose(fp);
            if ((period = strrchr(bin_file_path,'.')) != NULL)
            {
                *(period) = '\0';
            }
            sprintf(addr_str, "%0*X", 8, current_addr);
            strcat(bin_file_path, addr_str);
            strcat(bin_file_path, ".bin");
            if ((fp = fopen(bin_file_path, "wb")) == NULL)
            {
                fprintf(stdout, "Can't create file %s\n", bin_file_path);
				free(bin_file_path);
                return FALSE;
            }
			fprintf(stdout, "Output file %s created.\n", bin_file_path);
			first_block = FALSE;
        }
        fwrite(current->Data, 1, current->Length, fp);
        current_addr = current->Address + current->Length;
        current = current->Next;
    }
    fclose(fp);
	free(bin_file_path);
    return TRUE;
}

