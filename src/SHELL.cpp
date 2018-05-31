#include <stdio.h>

void openFile() {
	FILE* fp;
	FILE* test;
	fopen_s(&test, "shell.txt", "r");
	
	if(test == NULL){
		fopen_s(&fp, "shell.txt", "w");
		for (int i = sizeof(0x00); i <= 1048576; ++i)
			putc(0x00, fp);
		fclose(fp);
	}
	fclose(test);
}

int main()
{
	openFile();
	return 0;
}

