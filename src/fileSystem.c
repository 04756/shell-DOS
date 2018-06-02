#include "fileSystem.h"

void creatFile(FCBList a)
{
	a->iden = 0;
	
	a->signedId = root->dev_id + &a;
	a->next = NULL;
	a->childs = NULL;
	//char str[20];
	//sprintf_s(str, 20, "%d", a->signedId);
	//*(a->adr) = strcat_s("/root/",sizeof("/root/")+sizeof(str),str);
	//a->adr = (char*)malloc(sizeof(char) * 30);
	//char str[30] = "/root/";
	//printf("%s    %d\n", str,sizeof(a->adr));
	//strcat_s(str, sizeof("/root/") + sizeof(a->fileId), a->fileId);
	//a->adr[0] = '\0';
	//printf("%s    %s\n", str, a->adr);
	//strcat_s(a->adr, sizeof(str), str);
	//printf("%s    %d", a->adr, sizeof(a->adr));


	if (root->childs == NULL)
		root->childs = a;
	else {
		FCBList temp = root->childs;
		while (temp->next != NULL) {
			if (strcmp(temp->fileId,a->fileId)==0) {
				printf("已存在同名文件！创建失败！\n");
				goto fail;
			}
			temp = temp->next;
		}
		temp->next = (FCBList)malloc(sizeof(FCB));
		temp->next = a;
	}
	fail:{}
}

void creatItem(FCBList a)
{
	//a->adr = (char*)malloc(sizeof(char)* 30);
	a->iden = 1;
	a->signedId = root->dev_id + &a;
	a->next = NULL;
	a->childs = NULL;
	//char str[20];
	//sprintf_s(str,20, "%d", a->signedId);
	//*(a->adr) = strcat_s("/root/", sizeof("/root/") + sizeof(str), str);
	//char str[30] = "/root/";
	//strcat_s(str, sizeof("/root/") + sizeof(a->fileId), a->fileId);
	//a->adr[0] = '\0';
	//strcat_s(a->adr, sizeof(str), str);
	if (root->childs == NULL)
		root->childs = a;
	else {
		FCBList temp = root->childs;
		while (temp->next != NULL) {
			if (strcmp(temp->fileId, a->fileId) == 0) {
				printf("已存在同名目录！创建失败！\n");
				goto fail;
			}
			temp = temp->next;
		}
		temp->next = (FCBList)malloc(sizeof(FCB));
		temp->next = a;
	}
	fail: {}
}

void creatItemFile(FCBList item,  FCBList a)
{
	a->childs = NULL;
		//a->adr = (char*)malloc(sizeof(char) * 30);
		//strcat_s(strcat_s(item->adr,sizeof(item->adr)+sizeof("/"),"/"), sizeof(item->adr) + sizeof(a->fileId), a->fileId);
		//a->adr[0] = '\0';
		//strcat_s(a->adr, sizeof(str), str);
		
		
		if (item->childs == NULL)
		item->childs = a;
		else {
			FCBList temp = item->childs;
			while (temp->next != NULL) {
				if (strcmp(temp->fileId, a->fileId) == 0) {
					printf("已存在同名文件！创建失败！\n");
					goto fail;
				}
				temp = temp->next;
			}
			temp->next = (FCBList)malloc(sizeof(FCB));
			temp->next = a;
		}
		fail: {}
}

void creatItemItem(FCBList item, FCBList a)
{
	if (a->iden == 1) {
		a->iden = 1;
		a->next = NULL;
		a->childs = NULL;
		//a->adr = (char*)malloc(sizeof(char) * 30);
		//strcat_s(strcat_s(item->adr, sizeof(item->adr) + sizeof("/"), "/"), sizeof(item->adr) + sizeof(a->fileId), a->fileId);
		if (item->childs == NULL)
			item->childs = a;
		else {
			FCBList temp = item->childs;
			while (temp->next != NULL) {
				if (strcmp(temp->fileId, a->fileId) == 0) {
					printf("已存在同名文件！创建失败！\n");
					goto fail;
				}
				temp = temp->next;
			}
			temp->next = (FCBList)malloc(sizeof(FCB));
			temp->next = a;
		}
	fail: {}
	}
}

void ls(FCBList curChannel)
{
	FCB* temp;
	if (curChannel != NULL)
		temp = curChannel;
	else
		temp = root->childs;
	while (temp->next != NULL) {
		printf("%s\n", temp->fileId);
		temp = temp->next;
	}
	printf("%s\n", temp->fileId);
}
