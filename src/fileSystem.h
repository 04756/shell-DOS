#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

#define MAXNAME 25//�ļ�������󳤶�
#define childSize 10//������Ŀ¼���ļ�������

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
	//int signedId;//ϵͳ��ʶ��
	//int type;//��֧�ֲ�ͬ�����ļ�ϵͳʹ��
	int iden;//�ļ�0��Ŀ¼1
	//char adr[30];//�ļ�λ��
	unsigned long fileSize;//�ļ���С
	//char* fileContent;
	char permissions[3];//����Ȩ��
	int fMode;//����Ȩ�ޣ�0�ɶ���1��д��2�ɶ���д,3���ļ�
	//DATA setup;
	struct tm* setUp;
	//DATA recentChange;//�ϴ��޸�ʱ��
	struct tm* chang;

	int childNum;//���ļ���Ŀ¼������
	int childs[childSize];//��Ŀ¼�����ļ����±�
	int father;//��Ŀ¼���±�
	int adr;

			   //FCBList next;//ͬ���ֵ�
			   //FCBList childs;//���ļ�����Ŀ¼
			   //FCBList father;//��Ŀ¼
}FCB;

typedef struct
{
	int dev_id;//�����豸�豸��
	unsigned long s_blocksize;//�ļ���ϵͳ�Ŀ��С�����ֽ�Ϊ��λ
	struct FCB* childs;
	//int FAT[512];//�ļ����±꣬�ļ����
}super_block;

typedef struct {
	char contain[1024];
}Block;//���ݿ飬ÿ��1kb

super_block SUPER;
FCBList content[400];
Block FAT[512];//���ݿ飬���ڴ���ļ���Ϣ��
char result[10][10];

void md(FCBList cur,char*);
void mkdir(FCBList cur, char*);
FCBList cd(FCBList cur, char *);
void dir(FCBList cur);
void edit(char*, FCBList cur);
void type(char*, FCBList cur);
void find(char*);
void attrib(char*, FCBList cur);


char* split(char *str, char *s);
char* splitCommand(char *str);
FCBList findPath(char *a, FCBList start);
char* subString(char * des, char* source, int start, int count);