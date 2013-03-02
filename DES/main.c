// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "DES.h"

int main(int argc, char* argv[])
{
	unsigned char plain_text[9] = "abcdefgh";
	unsigned char key[9] = "abcdefgh";
	unsigned char result[8] = {0};
	unsigned char* result2 = NULL;
	unsigned char decrypt_data[8] = {0};
	KeySchedule key_s;
	key_s = DESKeySchedule(key);
	//result2 = DESEncrypt((Block)plain_text, key_s);
	DESEncrypt((Block)plain_text, key_s, result);

	DESDecrypt(result, key_s, decrypt_data);

	free(key_s);
	return 0;
}

