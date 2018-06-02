#pragma once
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAXNAME 25//�ļ�������󳤶�

typedef struct
{
	int year;
	int month;
	int day;
	int hour;
	int minutes;
	int seconds;
}DATA;

typedef struct FileControlBlock* FCBList;

typedef struct FileControlBlock
{
	char fileId[MAXNAME];//�ļ�����
	int signedId;//ϵͳ��ʶ��
	int type;//��֧�ֲ�ͬ�����ļ�ϵͳʹ��
	int iden;//�ļ�0��Ŀ¼1
	char adr[30];//�ļ�λ��
	unsigned long fileSize;//�ļ���С
	char* fileContent;
	char permissions[3];//����Ȩ��
	int fMode;//����Ȩ�ޣ�0�ɶ���1��д��2�ɶ���д
	DATA setup;
	DATA recentChange;//�ϴ��޸�ʱ��
	FCBList next;//ͬ���ֵ�
	FCBList childs;//���ļ�����Ŀ¼
}FCB;

 typedef struct
{
	int dev_id;//�����豸�豸��
	unsigned long s_blocksize;//�ļ���ϵͳ�Ŀ��С�����ֽ�Ϊ��λ
	struct FCB* childs;
}super_block;
 
super_block *root;

void creatFile(FCBList a);
void creatItem(FCBList a);//��Ŀ¼�´���
void creatItemFile(FCBList item, FCBList a);//�Ǹ�Ŀ¼�´���
void creatItemItem(FCBList item, FCBList a);
void ls(FCBList curChannel);


