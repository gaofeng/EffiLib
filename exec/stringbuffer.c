#include <stdlib.h>
#include <string.h>

#include "stringbuffer.h"

//初始化字符串指针
char* StringInit()
{
	char* str = (char*)malloc(1 * sizeof(char));
	memset(str, 0x00, 1*sizeof(char));
	return str;
}

//设置字符串为指定字符串
char* StringSet(char* old,const char* new)
{
	int len = strlen(new) + 1;
	old = (char*)realloc(old, len * sizeof(char));
	strcpy(old, new);
	return old;
}

//在字符串末尾添加指定字符串
char* StringAppent(char* first,const char* last)
{
	int len=strlen(first) + strlen(last) + 1;
	first = (char*)realloc(first, len * sizeof(char));
	strcat(first, last);
	return first;
}

//在字符串指定位置添加指定字符串
char* StringInsert(char* str,const char* insert,int start)
{
	int pos = 0;
	int len = 0;
	int i = 0;
	//对开始插入的数字进行验证
	if (start > (int)strlen(str))
	{
		start = strlen(str);
	}
	else if (start < 0)
	{
		start = 0;
	}
	//重新分配内容
	pos = strlen(str);
	len = strlen(str)+strlen(insert)+1;
	str = (char*)realloc(str,len*sizeof(char));

	//使用指针的方式进行遍历
	//通过倒序方式将准备插入数据后位置的数据复制到最后
	for(i=1;i<=pos-start;i++)
	{
		str[len-i-1]=str[len-i-strlen(insert) -1];
	}
	//将准备插入的数据插入到开始插入的地方
	for(i = 1; i <= (int)strlen(insert); i++)
	{
		str[i + start - 1] = insert[i - 1];
	}
	//末尾置0
	str[len-1] = '\0';
	return str;
}