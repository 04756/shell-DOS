#include "fileSystem.h"

void openFile() {
	FILE* fp;
	FILE* test;
	fopen_s(&test, "shell.txt", "r");
	
	if(test == NULL){
		fopen_s(&fp, "shell.txt", "w");
		for (int i = sizeof(0x00); i <= 1048576; ++i)
			putc(0x00, fp);
		fseek(fp, 0, 0);
		fclose(fp);
	}
	fclose(test);

	for (int i = 0; i < 400; ++i) {
		content[i] = (FCBList)malloc(sizeof(FCB));
		content[i]->fMode = 3;
		memset(content[i]->childs, -1, sizeof(int)*childSize);
	}
	for (int i = 0; i < 512; ++i)
		memset(FAT[i].contain ,'\0',sizeof(char)*1024);
}

int getline(char *s, int lim) {
	int i;
	i = 0;
	char c;
	while (c = getchar() != EOF && c != '\n'&&i < lim - 1)
		s[i++] = c;
	s[i] = '\0';
	return i;
}

int main()
{
	openFile();
	
	//root的初始化
	FCBList root = (FCBList)malloc(sizeof(FCB));
	strcpy(root->fileId,"root");
	root->iden = 1;
	root->fileSize = 1024*1024;
	root->fMode = 2;

	//root->setUp = localtime(time(0));
	FCBList cur = root;//当前目录
	content[0] = cur;
	cur->adr = 0;
	
	char command[20];
	printf("%s>", cur->fileId);
	scanf("%s", command);
	while (strcmp(command, "exit") != 0) {
		char commandFor[20];
		//getline(commandFor, 20);
		//scanf("%s", commandFor);
		getchar();
		gets(commandFor);
		if (strcmp(command, "dir") == 0) {
			dir(cur);
			goto com;
		}
		else if (strcmp(command, "md") == 0)
			md(cur, commandFor);
		else if (strcmp(command, "mkdir") == 0)
			mkdir(cur, commandFor);
		else if (strcmp(command, "cd") == 0)
			cur = cd(cur, commandFor);
		else if (strcmp(command, "find") == 0)
			find(commandFor);
		else if (strcmp(command, "edit") == 0)
			edit(commandFor, cur);
		else if (strcmp(command, "type") == 0)
			type(commandFor, cur);
		else if (strcmp(command, "attrib") == 0)
			attrib(commandFor, cur);
		else
			printf("查找不到该命令\n\n");
	com: {
		printf("%s>", cur->fileId);
		scanf("%s", command); }
	}
	return 0;
}

