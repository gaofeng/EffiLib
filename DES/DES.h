#ifndef DES_H_INCLUDED
#define DES_H_INCLUDED

#include <stdlib.h>

#ifdef __cplusplus 
extern "C" { 
#endif 

typedef unsigned char uChar;
typedef unsigned char * Block, * Table;
typedef unsigned char * KeySchedule;

KeySchedule DESKeySchedule(Block key);

void DESEncrypt(Block message, KeySchedule k, Block result);

void DESDecrypt(Block cipher, KeySchedule k, Block result);

#ifdef __cplusplus 
} 
#endif

#endif // DES_H_INCLUDED
