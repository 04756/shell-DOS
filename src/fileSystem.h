#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <io.h>
#include <time.h>
#include <Windows.h>
#include <winbase.h>
#include <VersionHelpers.h>
#include <sys\types.h>
#include <ShlObj.h>
#include <fileapi.h>
#include <tchar.h>

#define MAXNAME 25//�ļ�������󳤶�
#define childSize 50//������Ŀ¼���ļ�������
#define DIRECTORYNUM 480//Ŀ¼�����
#define FILEBLOCKNUM 900//�ļ��̿����
#define BUFSIZE 1024*1024//1M

typedef struct FileControlBlock* FCBList;

typedef struct FileControlBlock
{
	char fileId[MAXNAME];//�ļ�����
	int iden;//�ļ�0��Ŀ¼1
	int fMode;//����Ȩ�ޣ�0�ɶ���1��д��2�ɶ���д,3���ļ�
	struct tm* setUp;
	struct tm* chang;

	int childNum;//���ļ���Ŀ¼������
	int childs[childSize];//��Ŀ¼�����ļ����±�
	int father;//��Ŀ¼���±�
	int adr;//�Լ����±�
}FCB;

typedef struct
{
	int dev_id;//�����豸�豸��
	unsigned long s_blocksize;//�ļ���ϵͳ�Ŀ��С�����ֽ�Ϊ��λ
	unsigned long remainSize;//ʣ���С
}super_block;//������

typedef struct {
	char contain[1024];
}Block;//���ݿ飬ÿ��1kb

super_block SUPER;
FCBList content[DIRECTORYNUM];//��Ŀ¼��
Block FAT[FILEBLOCKNUM];//���ݿ飬���ڴ���ļ���Ϣ��
char result[childSize][50];//���ڴ洢�ָ����������
int out;//�����˳��ݹ�

//��ϵͳ����
FCBList md(FCBList cur,char*);
FCBList mkdir(FCBList cur, char*);
void more(char*,FCBList cur);
void move(char*, FCBList cur);
FCBList cd(FCBList cur, char *);
void copy(FCBList cur, char *a);
void dir(FCBList cur,char *a);
void delete(char*, FCBList cur);
void edit(char*, FCBList cur);
void type(char*, FCBList cur);
void find(char*);
void attrib(char*, FCBList cur);
void rm(char*, FCBList cur);
void rmdir(char *, FCBList cur,int f);
void rename_(char *a, FCBList cur);
void getTime();
void ver();
void help();

//��ϵͳ����
void import(char*, FCBList cur);
void export_(char*, FCBList cur);

//���ܺ���
void initFile();
void write_file();
void read_file();
void getRootPathName(FCBList cur);
char* split(char *str, char *s);
char* splitCommand(char *str);
char* splitForFileName(char*,char*);
FCBList findPath(char *a, FCBList start);
FCBList findPrePath(char *a, FCBList cur);
int findFATEmpty();
int findChildsEmpty(FCBList cur);
char* subString(char * des, char* source, int start, int count);