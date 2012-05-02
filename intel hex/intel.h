#ifndef _INTEL_H
#define _INTEL_H

#include "type.h"

#define PROGRAM "hexproc"
#define VERSION "0.0.1"

typedef struct IntelHexFormat IntelHexFormat;

void usage(void);

IntelHexFormat* IntelHexFileInput(const char* file_name);

u32 IntelHexFileGetSize(IntelHexFormat* ihf);

u32 IntelHexFileFillEmptyValue(IntelHexFormat* ihf, u8 empty_value);

bool IntelHexFileOutput(IntelHexFormat* ihf, const u8* hex_path);

/*
Merge ifh1 and ifh2 to ifh1.
*/
bool IntelHexFileMerge(IntelHexFormat* ihf1, IntelHexFormat* ihf2);

/*
If hex file has multi data section, then will output several bin file, and from the second file, it's
name will append start address.
*/
bool IntelHexFileToBin(IntelHexFormat* ihf, u8* bin_file_path);

void IntelHexFree(IntelHexFormat* ihf);

#endif // _INTEL_H
