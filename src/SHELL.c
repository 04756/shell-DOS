
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
}

int main()
{
	openFile();
	//FILE* fp;
	//char* buf = (char*)malloc(1048576 * sizeof(0x00));
	//fopen_s(&fp, "shell.txt", "w");
	//setvbuf(fp, buf, _IOFBF, sizeof(buf));
	FCBList curChannel = NULL;//当前目录,等于NULL代表当前目录为root
	root = (super_block*)malloc(sizeof(super_block));
	root->childs = NULL;
	int n = 0;
	scanf_s("%d", &n);
	while (n != 2) {
		FCBList a = (FCBList)malloc(sizeof(FCB));
		a->childs = a->next = NULL;
		printf("输入文件名\n");
		scanf_s("%s", a->fileId,sizeof(a->fileId));
		a->iden = 0;
		if (curChannel == NULL) {
			creatFile(a);
		}
		FCBList B = (FCBList)malloc(sizeof(FCB));
		B->childs = B->next = NULL;
		printf("输入目录名\n");
		scanf_s("%s", B->fileId, sizeof(B->fileId));
		a->iden = 1;
		if (curChannel == NULL) {
			creatFile(B);
		}
		scanf_s("%d", &n);
	}
	ls(curChannel);
	return 0;
}

