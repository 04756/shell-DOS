
#include "fileSystem.h"
//#include "test.h"

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
	initFile();//系统初始化
	FCBList cur = content[0];
	cur->adr = 0;

	

	char command[20];
	printf("%s>", cur->fileId);
	scanf("%s", command);
	while (strcmp(command, "exit") != 0) {
		char commandFor[50];
		//getline(commandFor, 20);
		//scanf("%s", commandFor);
		getchar();
		gets(commandFor);
		if (strcmp(command, "dir") == 0) {
			dir(cur);
			goto com;
		}
		else if (strcmp(command, "delete") == 0)
			delete(commandFor, cur);
		else if (strcmp(command, "md") == 0)
			md(cur, commandFor);
		else if (strcmp(command, "mkdir") == 0)
			mkdir(cur, commandFor);
		else if (strcmp(command, "more") == 0)
			more(commandFor, cur);
		else if (strcmp(command, "move") == 0)
			move(commandFor, cur);
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
		else if (strcmp(command, "copy") == 0)
			copy(cur, commandFor);
		else if (strcmp(command, "rm") == 0)
			rm(commandFor, cur);
		else if (strcmp(command, "rmdir") == 0)
			rmdir(commandFor, cur);
		else if (strcmp(command, "rename") == 0)
			rename_(commandFor, cur);
		else if (strcmp(command, "ver") == 0)
			ver();
		else if (strcmp(command, "time") == 0)
			getTime();
		else
			printf("查找不到该命令\n\n");
	com: {
		printf("%s>", cur->fileId);
		scanf("%s", command); }
	}
	return 0;
}

