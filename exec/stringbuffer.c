#include <stdlib.h>
#include <string.h>

#include "stringbuffer.h"

//��ʼ���ַ���ָ��
char* StringInit()
{
	char* str = (char*)malloc(1 * sizeof(char));
	memset(str, 0x00, 1*sizeof(char));
	return str;
}

//�����ַ���Ϊָ���ַ���
char* StringSet(char* old,const char* new)
{
	int len = strlen(new) + 1;
	old = (char*)realloc(old, len * sizeof(char));
	strcpy(old, new);
	return old;
}

//���ַ���ĩβ���ָ���ַ���
char* StringAppent(char* first,const char* last)
{
	int len=strlen(first) + strlen(last) + 1;
	first = (char*)realloc(first, len * sizeof(char));
	strcat(first, last);
	return first;
}

//���ַ���ָ��λ�����ָ���ַ���
char* StringInsert(char* str,const char* insert,int start)
{
	int pos = 0;
	int len = 0;
	int i = 0;
	//�Կ�ʼ��������ֽ�����֤
	if (start > (int)strlen(str))
	{
		start = strlen(str);
	}
	else if (start < 0)
	{
		start = 0;
	}
	//���·�������
	pos = strlen(str);
	len = strlen(str)+strlen(insert)+1;
	str = (char*)realloc(str,len*sizeof(char));

	//ʹ��ָ��ķ�ʽ���б���
	//ͨ������ʽ��׼���������ݺ�λ�õ����ݸ��Ƶ����
	for(i=1;i<=pos-start;i++)
	{
		str[len-i-1]=str[len-i-strlen(insert) -1];
	}
	//��׼����������ݲ��뵽��ʼ����ĵط�
	for(i = 1; i <= (int)strlen(insert); i++)
	{
		str[i + start - 1] = insert[i - 1];
	}
	//ĩβ��0
	str[len-1] = '\0';
	return str;
}