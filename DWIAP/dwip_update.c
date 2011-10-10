#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dwip_update.h"
#include "usdl_crc.h"

typedef struct FileHeader_st
{
	u32 header_crc; /*�ļ�ͷУ��*/
	u32 update_flag; /*���ն�������ֵ*/
	u32 attribute;  /*�ļ����ͣ������ļ�Ϊ1*/
	u32 sn;			/*�ļ����к�*/
	u8 version[20]; /*�汾�ַ�������0x00����*/
	u8 product_name[20]; /*��Ʒ���ƣ���0x00����*/
	u32 update_file_length; /*�����ļ��ܳ���*/
	u32 update_file_crc;	/*�����ļ���У��*/
	u32 time;	/*ʱ�䣬�ļ�ʱ���*/
} FileHeaderType;

#define CHUNK_SIZE  (0x400 * 5)

int GenerateUpdateFile(const u8* bin_file_path, const u8* dat_file_path, const u8* product_name, const u8* version)
{
	FileHeaderType* fht = NULL;
	FILE* bin_file = NULL;
	FILE* dat_file = NULL;
	u32 file_len = 0;
	u32 file_crc = 0;
	u8* file_buf = NULL;
	u32 file_buf_size = 0;
	u32 read_len = 0xFFFFFFFF;
	u32 i = 0;

	if ((strlen(bin_file_path) < 4) || (memcmp(bin_file_path + strlen(bin_file_path) - 4, ".bin", 4) != 0))
	{
		printf("Error: Input file's extension must be .bin!\n");
		return 0;
	}
	if ((strlen(dat_file_path) < 4) || (memcmp(dat_file_path + strlen(dat_file_path) - 4, ".dat", 4) != 0))
	{
		printf("Error: Output file's extension must be .dat!\n");
		return 0;
	}

	bin_file = fopen(bin_file_path, "rb");
	if (bin_file == NULL)
	{
		printf("Error: Can't open file %s.\n", bin_file_path);
		return 0;
	}
	dat_file = fopen(dat_file_path, "wb");
	if (dat_file == NULL)
	{
		printf("Error: Can't open or create file %s.\n", dat_file_path);
		fclose(bin_file);
		return 0;
	}

	fht = malloc(sizeof(FileHeaderType));
	if (fht == NULL)
	{
		printf("Error: Not enough memory.\n");
		return 0;
	}

	memset(fht, 0x00, sizeof(FileHeaderType));
	fht->attribute = 1;
	fht->sn = 0;
	strncpy(fht->version, version, 20);
	strncpy(fht->product_name, product_name, 20);

	/*��ȡ�����Ƴ����ļ�*/
	file_buf_size = 0;
	file_len = 0;
	do
	{
		file_buf_size += CHUNK_SIZE;
		file_buf = realloc(file_buf, file_buf_size);
		if (file_buf == NULL)
		{
			printf("Error: Not enough memory.\n");
			return 0;
		}
		read_len = fread(file_buf + file_len, 1, CHUNK_SIZE, bin_file);
		file_len += read_len;

	} while (read_len == CHUNK_SIZE);
	fclose(bin_file);

	fht->update_file_length = file_len;
	fht->update_file_crc = GetCRC32(CRC_DEFAULT_SEED, file_buf, file_len);
	fht->header_crc = GetCRC32(CRC_DEFAULT_SEED, (u8*)fht + 4, sizeof(FileHeaderType) - 4);

	fwrite(fht, 1, sizeof(FileHeaderType), dat_file);
	for (i = 0; i < 188; i++)
	{
		fwrite("\xFF", 1, 1, dat_file);
	}
	fwrite(file_buf, 1, file_len, dat_file);
	fclose(dat_file);
	
	free(file_buf);
	free(fht);

	return 1;
}