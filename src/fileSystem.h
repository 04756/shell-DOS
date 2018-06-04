#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>

#define MAXNAME 25//文件名字最大长度
#define childSize 10//包含子目录子文件最大个数

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
	char fileId[MAXNAME];//文件名字
	//int signedId;//系统标识符
	//int type;//被支持不同类型文件系统使用
	int iden;//文件0，目录1
	//char adr[30];//文件位置
	unsigned long fileSize;//文件大小
	//char* fileContent;
	char permissions[3];//访问权限
	int fMode;//操作权限，0可读，1可写，2可读可写,3无文件
	//DATA setup;
	struct tm* setUp;
	//DATA recentChange;//上次修改时间
	struct tm* chang;

	int childNum;//子文件子目录的数量
	int childs[childSize];//子目录，子文件的下标
	int father;//父目录的下标
	int adr;

			   //FCBList next;//同级兄弟
			   //FCBList childs;//子文件，子目录
			   //FCBList father;//父目录
}FCB;

typedef struct
{
	int dev_id;//所在设备设备号
	unsigned long s_blocksize;//文件按系统的块大小，以字节为单位
	struct FCB* childs;
	//int FAT[512];//文件的下标，文件快表
}super_block;

typedef struct {
	char contain[1024];
}Block;//数据块，每块1kb

super_block SUPER;
FCBList content[400];
Block FAT[512];//数据块，用于存放文件信息。
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