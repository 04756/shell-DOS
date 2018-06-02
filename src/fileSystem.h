#pragma once
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAXNAME 25//文件名字最大长度

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
	int signedId;//系统标识符
	int type;//被支持不同类型文件系统使用
	int iden;//文件0，目录1
	char adr[30];//文件位置
	unsigned long fileSize;//文件大小
	char* fileContent;
	char permissions[3];//访问权限
	int fMode;//操作权限，0可读，1可写，2可读可写
	DATA setup;
	DATA recentChange;//上次修改时间
	FCBList next;//同级兄弟
	FCBList childs;//子文件，子目录
}FCB;

 typedef struct
{
	int dev_id;//所在设备设备号
	unsigned long s_blocksize;//文件按系统的块大小，以字节为单位
	struct FCB* childs;
}super_block;
 
super_block *root;

void creatFile(FCBList a);
void creatItem(FCBList a);//根目录下创建
void creatItemFile(FCBList item, FCBList a);//非根目录下创建
void creatItemItem(FCBList item, FCBList a);
void ls(FCBList curChannel);


