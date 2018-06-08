#include "fileSystem.h"

int main()
{
	initFile();//系统初始化
	FCBList cur = content[0];
	cur->adr = 0;
	ver();

	char command[20];
	printf("/%s>", cur->fileId);
	scanf("%s", command);
	while (strcmp(command, "exit") != 0) {
		out = 0;
		char commandFor[50];

		getchar();
		gets(commandFor);
		
		if (strcmp(command, "dir") == 0)
			dir(cur, commandFor);
		else if (strcmp(command, "delete") == 0)
			delete(commandFor, cur);
		else if (strcmp(command, "md") == 0)
			md(cur, commandFor);
		else if (strcmp(command, "mkdir") == 0)
			mkdir(cur, commandFor,0);
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
			rmdir(commandFor, cur,0);
		else if (strcmp(command, "rename") == 0)
			rename_(commandFor, cur);
		else if (strcmp(command, "ver") == 0)
			ver();
		else if (strcmp(command, "time") == 0)
			getTime();
		else if (strcmp(command, "import") == 0)
			import(commandFor, cur);
		else if (strcmp(command, "export") == 0)
			export_(commandFor, cur);
		else if (strcmp(command, "help") == 0)
			help();
		else
			printf("查找不到该命令\n\n");

		printf("/root");
		getRootPathName(cur->fileId);
		printf("> ");
		scanf("%s", command); }

	return 0;
}

