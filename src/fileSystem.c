#include "fileSystem.h"

//创建一个文件
FCBList md(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			int k;
			for (k = 0; k < childSize; ++k) {
				if (cur->childs[k] >= 0 && strcmp(content[cur->childs[k]]->fileId, a) == 0 && content[cur->childs[k]]->iden == 0) {
					printf("该名字已存在！\n");
					return NULL;
				}
			}
			k = findChildsEmpty(cur);
			if (k == -1) {
				printf("\n目录文件空间已满！\n");
				return;
			}
			content[i]->iden = 0;
			content[i]->fMode = 2;
			content[i]->father = cur->adr;
			content[i]->adr = i;
			strcpy(content[i]->fileId, a);
			cur->childs[k] = i;
			break;
		}
	write_file();
	return content[i];
}

//创建一个文件夹
FCBList mkdir(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			int k;
			for (k = 0; k < childSize; ++k) {
				if (cur->childs[k] >= 0 && strcmp(content[cur->childs[k]]->fileId, a) == 0 && content[cur->childs[k]]->iden == 1) {
					printf("该名字已存在！\n");
					return NULL;
				}
			}
			k = findChildsEmpty(cur);
			if (k == -1) {
				printf("\n目录文件空间已满！\n");
				return;
			}
			cur->childs[k] = i;
			content[i]->iden = 1;
			content[i]->fMode = 2;
			content[i]->father = cur->adr;
			content[i]->adr = i;
			strcpy(content[i]->fileId, a);
			break;
		}
	write_file();
	return content[i];
}

//以分页形式查看文件
void more(char *a, FCBList cur)
{
	FCBList temp = cur;
	for (int i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0 && content[cur->childs[i]]->iden == 0 && strcmp(a, content[cur->childs[i]]->fileId) == 0) {
			temp = content[cur->childs[i]];
			break;
		}
	}
	for (int i = 0; i < childSize; ++i) {
		if (temp->childs[i] >= 0) {
			int flag = 0;
			
			if (i > 0) {
				flag = 1;
				char turn[2];
				gets(turn);
				if (strcmp(turn, "n") == 0) {
					flag = 0;
					system("cls");
				}
				else
					return;
			}
			if(flag == 0)
			printf("%s", FAT[temp->childs[i]].contain);
			printf("\n\n按'n'字母键翻页,按任意键退出\n");
		}
	}
	return;
}

//将指定文件或文件夹移动到指定目录
void move(char *a, FCBList cur)
{
	char b[50];
	splitCommand(a);
	strcpy(a, result[0]);
	strcpy(b, result[1]);

	FCBList source = findPrePath(a,cur);
	if (source == NULL)
		goto fail;

	for (int i = 0; i < childSize; ++i) {
		if (source->childs[i] >= 0 && strcmp(content[source->childs[i]]->fileId, a) == 0) {
			int q = source->childs[i];
			source->childs[i] = -1;
			source = content[q];
			source->father = 0;
			break;
		}
	}

	split(b, "/");
	FCBList des = content[0];
	for (int i = 0; i < childSize && strcmp(result[i], "") != 0; ++i) {
		FCBList t = des;
		if ((t = findPath(result[i], des)) == NULL)
			goto fail;
		else
			des = t;
	}
	des->childs[findChildsEmpty(des)] = source->adr;
	source->father = des->adr;
	goto success;
		
fail: {
	printf("\n找不到指定路径\n");
	return;
}
success:{
	  write_file();
}
}

//切换目录
FCBList cd(FCBList cur, char *a)
{
	if (strcmp(a,"..")==0) {
		if (strcmp(cur->fileId,"root")==0)
			goto fail;
		cur = content[cur->father];
		goto success;
	}
	else if (strstr(a, "/") == NULL) {
		for (int i = 0; i < childSize; ++i) {
			if (cur->childs[i] >= 0 && content[cur->childs[i]]->iden == 1) {
				if (strcmp(content[cur->childs[i]]->fileId, a) == 0) {
					cur = content[cur->childs[i]];
					goto success;
				}
			}
		}
		goto fail;
	}
	else {
		split(a, "/");
		int i = 0;
		FCBList temp = content[0];
		if (strcmp(result[1],"")==0 && strcmp(result[0], cur->fileId) == 0)
			goto success;
		else if (strcmp(result[1], "") == 0 && strcmp(result[0], content[0]->fileId) == 0) {
			cur = content[0];
			goto success;
		}
		else if (strcmp(result[0], cur->fileId) == 0) {
			i = 1;
			temp = cur;
		}
		else if (strcmp(result[0], "root") == 0) {
			i = 1;
		}
		for (i; i < childSize && strcmp(result[i],"")!=0&&temp!=NULL; ++i) {
			temp = findPath(result[i], temp);
		}
		if (temp != NULL) {
				cur = temp;
				goto success;
			}
	}
 for (int i = 0; i < childSize; ++i)
		  memset(result[i],'\0', childSize * sizeof(char));
fail: {
	printf("找不到指定路径\n"); 
	return;
	}
 success:{
	  return cur;
}
}

//将指定文件或文件夹复制到指定目录
void copy(FCBList cur, char *a)
{
	char b[50];
	splitCommand(a);
	strcpy(a, result[0]);
	strcpy(b, result[1]);
	FCBList source = findPrePath(a,cur);
	if (source == NULL)
		source = cur;
	for (int i = 0; i < childSize; ++i) {
		if (source->childs[i] >= 0 && content[source->childs[i]]->iden == 0 && strcmp(content[source->childs[i]]->fileId, a) == 0){
			source = content[source->childs[i]];
			a = source->fileId;
			break;
		}
	}
	split(b, "/");
	FCBList des = content[0];
	for (int i = 0; i < childSize && strcmp(result[i], "") != 0; ++i) {
		FCBList t = des;
		if ((t = findPath(result[i], des)) == NULL)
			goto fail;
		else
			des = t;
	}
	
	des = md(des, a);//在目标文件夹下创建文件
	if (des == NULL)
		goto fail;
	for (int i = 0,j = 0; i < childSize; ++i) {
		if (source->childs[i] >= 0) {
			int q = findFATEmpty();
			des->childs[j] = q;
			strcpy(FAT[q].contain,FAT[source->childs[i]].contain);
			++j;
		}
	}
	goto success;
fail:{
	printf("\n找不到指定路径\n");
}
success:{
	 write_file();
}
}

//显示当前目录文件以及文件夹
void dir(FCBList cur ,char *a)
{
	read_file();
	printf("虚拟文件系统\n");
	printf("当前位置：/%s\n\n", cur->fileId);
	if(strstr(a, "*") != NULL&&strstr(a,"/s")==NULL)
	subString(a, a, 1, strlen(a) - 1);
	for (int i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0) {
			if (strcmp(a, "") != 0 && strstr(content[cur->childs[i]]->fileId, a) != NULL) {
				printf("%s\t", content[cur->childs[i]]->fileId);
				if (content[cur->childs[i]]->iden == 1)
					printf("<DIR>\n");
				else
					printf("<FIFL>\n");
			}
			else if (strcmp(a, "") == 0|| strstr(a, "/s") != NULL) {
				printf("%s\t", content[cur->childs[i]]->fileId);
				if (content[cur->childs[i]]->iden == 1)
					printf("<DIR>\n");
				else
					printf("<FIFL>\n");
			}
		}
	}
}

//删除文件
void delete(char* a, FCBList cur){
	rm(a, cur);
}

//编辑文件
void edit(char *a, FCBList cur)
{
	int i;
	FCBList temp = cur;
	for (i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0 && content[cur->childs[i]]->iden == 0 && strcmp(a, content[cur->childs[i]]->fileId) == 0) {
			temp = content[cur->childs[i]];
			break;
		}
	}
	if (temp == cur)
		goto fail;
	for (i = 0; i < childSize; ++i) {
		if (temp->childs[i] < 0) {
			int q = 0;
			q = findFATEmpty();
			temp->childs[i] = q;
			char newLine = "";
			int k = 0;
			while (k < 1023) {
				newLine = getchar();
				if (newLine == "\n")
					FAT[q].contain[k] = '\n';
				else if (newLine != 35)
					FAT[q].contain[k] = newLine;
				else
					goto success;
				++k;
			}
		}
		else {
			printf("%s", FAT[temp->childs[i]].contain);
			if (strlen(FAT[temp->childs[i]].contain) < 1023) {
				char newLine = "";
				int k = strlen(FAT[temp->childs[i]].contain);
				while (k < 1023) {
					newLine = getchar();
					if (newLine == "\n")
						FAT[temp->childs[i]].contain[k] = '\n';
					else if (newLine != 35)
						FAT[temp->childs[i]].contain[k] = newLine;
					else
						goto success;
					++k;
				}
			}
		}
	}
fail: {
	printf("\n找不到指定文件\n");
	}
 success:{
	  time_t t;
	  time(&t);
	  temp->chang = localtime(&t);
	  write_file();
}
}

//查看文件，直接查看所有文件内容
void type(char *a, FCBList cur)
{
	FCBList temp = cur;
	for (int i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0 && content[cur->childs[i]]->iden == 0 && strcmp(a, content[cur->childs[i]]->fileId) == 0) {
			temp = content[cur->childs[i]];
			break;
		}
	}
	for (int i = 0; i < childSize; ++i) {
		if (temp->childs[i] >= 0) {
			printf("%s", FAT[temp->childs[i]].contain);
		}
	}
	printf("\n");
}

//查找指定文件的指定内容
void find(char *a)
{
	splitCommand(a);
	char searchStr[500];
	char path[500];
	strcpy(searchStr, result[0]);//保存文件名
	strcpy(path, result[1]);//保存路径
	for (int i = 0; i < childSize; ++i)//初始化result数组
		memset(result[i], '\0', childSize * sizeof(char));
	if(strstr(path, "/")!=NULL)//如果在当前路径寻找文件，则不分割路径
	split(path,"/");
	FCBList temp = content[0];
	printf("\n--------------%s\n\n", path);
	int i;
	//切换到相应路径
	FCBList t = temp;
	for (i = 0; i < childSize && strcmp(result[i+1], "") != 0; ++i) {
		if ((t = findPath(result[i], temp)) != NULL)
			temp = t;
	}
	//把文件名复制到path变量
	if (strcmp("",result[i])!=0)
		strcpy(path, result[i]);
	//指定的文件
	for (int q = 0; q < childSize; ++q) {
		if (temp->childs[q] >= 0 && content[temp->childs[q]]->iden == 0 && strcmp(content[temp->childs[q]]->fileId, path) == 0) {
			temp = content[temp->childs[q]];
			break;
		}
	}
	for (int q = 0; q < childSize;++q) {
		if (temp->childs[q] >= 0) {
			char *adr = strstr(FAT[temp->childs[q]].contain, "\n");
			if(adr ==NULL){
				if (strstr(FAT[temp->childs[q]].contain, searchStr) != NULL)
					printf("%s\n", FAT[temp->childs[q]].contain);
			}
			else {
				char tt[1024];
				char duplicate[1024];
				strcpy(duplicate, FAT[temp->childs[q]].contain);
				adr = strstr(duplicate, "\n");
				while (adr != NULL) {
					subString(tt, duplicate, 0,adr-duplicate );
					subString(duplicate, duplicate, adr-duplicate+1,strlen(duplicate)-strlen(tt));
					if (strstr(tt, searchStr) != NULL)
						printf("%s\n",tt);
					adr = strstr(duplicate, "\n");
				}
			}
		}
	}
	for (int i = 0; i < childSize; ++i)
		memset(result[i], '\0', childSize * sizeof(char));
}

//显示文件的操作权限
void attrib(char *a, FCBList cur)
{
	char path[500];
	FCBList temp = cur;
	strcpy(path,a);
	if (strstr(a, "/") != NULL) {
		split(a, "/");
		temp = content[0];
		FCBList t = temp;
		int i;
		for (i = 0; i < childSize && strcmp(result[i + 1], "") != 0 && temp != NULL; ++i) {
			if ((t = findPath(result[i], temp)) != NULL)
				temp = t;
			else if(t == NULL && strcmp(result[i+1],"")!=0)
				goto fail;
		}
		strcpy(path, result[i]);
	}
	for (int i = 0; i < childSize; ++i) {
		if (temp->childs[i] >= 0 && content[temp->childs[i]]->iden == 0 && strcmp(path, content[temp->childs[i]]->fileId) == 0) {
			switch (content[temp->childs[i]]->fMode)
			{
			case 0:
				printf("只可读\t%s\n", a);
				break;
			case 1:
				printf("只可写\t%s\n", a);
				break;
			case 2:
				printf("可读可写\t%s\n", a);
				break;
			default:
				break;
			}
			goto success;
		}
	}
	for (int i = 0; i < childSize; ++i)
		memset(result[i], '\0', childSize * sizeof(char));
fail: {
	printf("\n找不到指定路径\n");
	}
success: {
}
}

//删除文件
void rm(char *a, FCBList cur)
{
	int adressOfCurInFathe;
	for (int i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0&&strcmp(a,content[cur->childs[i]]->fileId)==0&& content[cur->childs[i]]->iden == 0){
			for (int j = 0; j < childSize; ++j) {
				if (content[cur->childs[i]]->childs[j] >= 0)
					strcpy(FAT[content[cur->childs[i]]->childs[j]].contain,"");
			}
			content[cur->childs[i]] = (FCBList)malloc(sizeof(FCB));
			content[cur->childs[i]]->fMode = 3;
			cur->childs[i] = -1;
			goto success;
		}
	}
	printf("\n找不到指定文件\n");
success:{
	write_file();
}
}

//删除文件夹
void rmdir(char *a, FCBList cur,int f)
{
	char confirm[2];
	int flag = f;
	int adressOfCurInFathe;
	int i = 0;

	if (strstr(a, "/s") != NULL||f == 1) {
		splitCommand(a);
		strcpy(a, result[0]);
		strcpy(confirm, "y");
		confirm[1] = '\0';
		flag = 1;
		f = 1;
	}
	
	if (strstr(a, "/") != NULL) {
		split(a, "/");
		cur = findPrePath(a, cur);
		if (cur == NULL)
			goto fail;
	}

	for (int i = 0; i < childSize; ++i) {
			if (cur->childs[i] >= 0 && strcmp(a, content[cur->childs[i]]->fileId) == 0) {
				cur = content[cur->childs[i]];
				adressOfCurInFathe = i;
				break;
			}
		}

	for (i = 0; i < childSize; ++i) {
			if (cur->childs[i] >= 0) {

				if (content[cur->childs[i]]->iden == 0) {
					if (flag == 0) {
						printf("是否确认删除y/n?\t%s\t", a);
						gets(confirm);
					}
					if (strcmp(confirm,"y")==0)
						rm(content[cur->childs[i]]->fileId, cur);
				}
				else {
					rmdir(content[cur->childs[i]]->fileId, cur,f);
				}
			}
		}
	if (out == 1)
		return;
	if (cur->fMode != 3) {
		if (flag == 0) {
			printf("是否确认删除y/n?\t%s\t", a);
			gets(confirm);
		}
		if (strcmp(confirm, "y") == 0) {
			content[cur->father]->childs[adressOfCurInFathe] = -1;
			content[cur->adr] = (FCBList)malloc(sizeof(FCB));
			content[cur->adr]->fMode = 3;
			goto success;
		}
		else {
			out = 1;
			return;
		}
	}

fail: {
	printf("\n找不到指定文件\n");
	}
  success: {
	  write_file();
}
}

//重命名指定文件或文件夹
void rename_(char * a, FCBList cur)
{
	splitCommand(a);
	strcpy(a, result[0]);
	char b[MAXNAME];
	strcpy(b, result[1]);
	FCBList ans = findPrePath(a,cur);
	if (ans == NULL)
		ans = cur;
	for (int i = 0; i < childSize; ++i) {
		if (ans->childs[i] >= 0 && strcmp(content[ans->childs[i]]->fileId, a) == 0) {
			strcpy(content[ans->childs[i]]->fileId, b);
			write_file();
			return;
		}
	}
	printf("\n没有找到指定文件\n");
}

//获取当地时间
void getTime()
{
	time_t t;
	struct tm* lt;
	time(&t);
	lt = localtime(&t);
	printf("%02d:%02d:%02d\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
}

//版本信息
void ver()
{
	printf("文件模拟系统\n2018/6/6 CMD author:杨和彦\n\n");
}

//帮助
void help()
{
	printf("md [文件名]\t创建文件\n");
	printf("mkdir [目录名]\t创建目录\n");
	printf("more [文件名]\t以分页模式显示文件\n");
	printf("move [源文件名或目录名][目的目录]\t移动制定文件或目录到指定地点\n");
	printf("cd [路径]\t切换当前路径\n");
	printf("copy [源文件名][目的目录]\t复制指定文件到指定目录\n");
	printf("dir [包含字符][参数]\t显示当前路径下包含包含字符的目录或文件,如不输入包含字符或带参数‘/s’则显示所有子目录子文件\n");
	printf("delete [文件名]\t删除文件\n");
	printf("edit [文件名]\t编辑文件\n");
	printf("type [文件名]\t显示文件内容\n");
	printf("find [字符串][文件名]\t查找指定文件中是否包含查找字符串，如果包含输出包含的一行\n");
	printf("attrib [文件名]\t显示文件的读取权限\n");
	printf("rm [文件名]\t删除文件\n");
	printf("rmdir [文件名][参数]\t删除文件夹，如果不带参数'/s'，则每删除一个文件或文件夹都询问用户\n");
	printf("rename [文件名]\t更改文件名字\n");
	printf("time\t获取当前时间\n");
	printf("ver\t获取当前系统介绍\n");
	printf("help\t输出命令介绍文档\n");
	printf("import [文件名][目录]\t从windows系统中引入文件至指定目录\n");
	printf("export [文件名][目录]\t从当前系统寻找指定文件写入windows系统\n");
}

//从windows系统引入文件
void import(char *a, FCBList cur)
{
	splitCommand(a);
	char b[MAXNAME];
	strcpy(a, result[0]);
	strcpy(b, result[1]);
	FCBList temp = cur;
	
	//找到导入的目的文件夹
	if (strstr(b, ".") == NULL&& strstr(b, "/") != NULL) {
		int i = 0;
		if (strstr(b, "root") != NULL)
			i = 1;
		findPrePath(b,cur);
		temp = findPath(b,temp);
		if (temp == NULL)
			goto fail;
	}
	else if (strstr(b, ".") == NULL && strstr(b, "/") == NULL) {
		temp = findPath(b, temp);
		if (temp == NULL)
			goto fail;
	}
	FILE *fp = fopen(a,"r");
	if (fp==NULL) {
		printf("\n找不到指定文件！\n");
		fclose(fp);
		return;
	}
	else {
		char name[MAXNAME];
		strcpy(name,splitForFileName(a,"\\"));
		if ((temp = md(temp, name)) != NULL) {
			char z;
			int q = 0,i = 0;
			int index = findFATEmpty();
			temp->childs[i] = index;
			i = 1;
			while ((z = fgetc(fp)) != EOF&&i<childSize) {
				if (q < 1023) {
					FAT[index].contain[q] = z;
					++q;
				}
				else{
					index = findFATEmpty();
					q = 0;
					++i;
					temp->childs[i] = index;
				}

			}
			fclose(fp);
			goto success;
		}
	}
fail:{
	printf("\n找不到指定路径！\n");
	return;
}
success:{
	 write_file();
}
}

//从本系统相windows系统引入文件，不可写入C盘
void export_(char *a, FCBList cur)
{
	splitCommand(a);
	char b[50];
	strcpy(a, result[0]);//本系统的源文件
	strcpy(b, result[1]);//跨系统的目的目录
	FCBList temp = cur;
	if(strstr(a,"/")!=NULL)
	temp = findPrePath(a,cur);

	if (temp == NULL)
		goto fail;

	for (int i = 0; i < childSize; ++i) {
		if (temp->childs[i] >= 0 && strcmp(a, content[temp->childs[i]]->fileId) == 0 && content[temp->childs[i]]->iden == 0) {
			temp = content[temp->childs[i]];
			goto success;
		}
	}
	goto fail;

success: {
			strcat(b, a);
			
			FILE *fp = fopen(b, "w");
			if(fp == NULL)
				goto fail;
			for (int i = 0; i < childSize; ++i) {
				if (temp->childs[i] >= 0) {
					fputs(FAT[temp->childs[i]].contain, fp);
				}
			}
			fclose(fp);
			return;
		}
fail:{
		 printf("\n没有找到指定文件\n");
	}
}

//初始化系统
void initFile()
{
	//printf("%d   %d   %d", sizeof(SUPER), sizeof(FCB), sizeof(Block));
	
	for (int i = 0; i < DIRECTORYNUM; ++i) {
		content[i] = (FCBList)malloc(sizeof(FCB));
		content[i]->fMode = 3;
		strcpy(content[i]->fileId, "");
		//strcpy(content[i]->permissions, "");
		content[i]->iden = 3;
		//content[i]->fileSize = 0;
		//content[i]->setUp = (struct tm *)malloc(sizeof(struct tm));
		//content[i]->chang = (struct tm *)malloc(sizeof(struct tm));
		content[i]->childNum = content[i]->father = content[i]->adr = 0;
		memset(content[i]->childs, -1, sizeof(int)*childSize);
	}
	content[0]->fMode = 2;
	content[0]->iden = 1;
	time_t t;
	time(&t);
	content[0]->setUp = localtime(&t);
	strcpy(content[0]->fileId,"root");
	for (int i = 0; i < FILEBLOCKNUM; ++i)
		memset(FAT[i].contain, '\0', sizeof(char) * 1024);
	SUPER.dev_id = 000001;
	SUPER.s_blocksize = 1024 * 1024;
	SUPER.remainSize = 1024 * 1024-strlen(content)*sizeof(FCB)-strlen(FAT)*sizeof(Block);
	if (_access("shell", 0) == -1)
		write_file();
	else {
		read_file();
	}
}

//写入虚拟文件磁盘
void write_file()
{
	/*
	FILE* fp;
	fopen_s(&fp, "shell", "wb");
	fwrite(&SUPER,sizeof(super_block),1,fp);
	for (int i = 0; i < DIRECTORYNUM; ++i) {
			fwrite(content[i], sizeof(FCB), 1, fp);
	}
	for (int i = 0; i < FILEBLOCKNUM; ++i) {
			fwrite(FAT[i].contain, sizeof(Block), 1, fp);
	}
	fclose(fp);*/

	/*生成固定大小的文件，用其代表连续存储区域*/
	OFSTRUCT ofs;
	DWORD dwWritenSize;							//记录实际写入的字节数，DWORD类型为int
	static char chWriteBuffer[BUFSIZE];		//定义一个1M的写入缓冲区
	HANDLE pFile = (HANDLE)OpenFile("shell", &ofs, OF_CREATE | OF_READWRITE);	//用生成读写方式打开文件
	memset(chWriteBuffer, 0, BUFSIZE);		//往这个1M的缓冲区中全部写入0
	WriteFile(pFile, chWriteBuffer, BUFSIZE, &dwWritenSize, NULL);		//写入数据，创建一个1M的文件
	CloseHandle(pFile);		//关闭文件句柄
						/*初始化文件系统，在文件系统的头部构建超级块，写入目录数组*/

	pFile = (HANDLE)OpenFile("shell", &ofs, OF_READWRITE);	//用读写方式打开文件
	WriteFile(pFile, &SUPER, sizeof(super_block), &dwWritenSize, NULL);			//写入超级块
	for(int i= 0;i<DIRECTORYNUM;++i)
	WriteFile(pFile, content[i], sizeof(FCB), &dwWritenSize, NULL);	//写入目录
	for (int i = 0; i<FILEBLOCKNUM; ++i)
	WriteFile(pFile, FAT[i].contain, sizeof(Block), &dwWritenSize, NULL);		//写入文件数据块
	CloseHandle(pFile);		//关闭文件句柄
}

//读取虚拟文件磁盘
void read_file()
{
	/*
	//pid_t r;
	//r = fork();
	FILE* fp;
	fopen_s(&fp, "shell", "rb");
	int offset = sizeof(super_block);
	fread(&SUPER, sizeof(super_block), 1, fp);
	fseek(fp, sizeof(super_block),0);
	for (int i = 0; i < DIRECTORYNUM; ++i) {
		fread(content[i], sizeof(FCB), 1, fp);
		offset += sizeof(FCB);
		fseek(fp, offset, 0);
	}
	for (int i = 0; i < FILEBLOCKNUM; ++i) {
		fread(FAT[i].contain, sizeof(Block), 1, fp);
		offset += sizeof(FCB);
		fseek(fp, offset, 0);
	}
	fclose(fp);
	*/
	OFSTRUCT ofs;
	DWORD dwWritenSize;							//记录实际写入的字节数，DWORD类型为int
	HANDLE pFile = (HANDLE)OpenFile("shell", &ofs, OF_READWRITE);	//用读写方式打开文件
	ReadFile(pFile, &SUPER, sizeof(super_block), &dwWritenSize, NULL);			//写入超级块
	for (int i = 0; i<DIRECTORYNUM; ++i)
		ReadFile(pFile, content[i], sizeof(FCB), &dwWritenSize, NULL);	//写入目录
	for (int i = 0; i<FILEBLOCKNUM; ++i)
	ReadFile(pFile, FAT[i].contain, sizeof(Block), &dwWritenSize, NULL);		//写入文件数据块
	CloseHandle(pFile);		//关闭文件句柄
}

//获取完整路径
void getRootPathName(FCBList cur)
{
	while (cur->adr != 0) {
		getRootPathName(content[cur->father]);
		printf("/%s", cur->fileId);
		break;
	}
}

char* split(char *str, char *s)
{
	for (int i = 0; i < childSize; ++i)//初始化result数组
		memset(result[i], '\0', childSize * sizeof(char));
	char* adr = strstr(str, s);
	if (adr == NULL)
		return NULL;
	//char result[10][10];
	int k = 0;
	int q = 0;
	++adr;
	char tt[50];
	for (char i = *adr;;i = *(++adr), ++q) {

		if (i == '/'||i=='\0') {
			tt[q] = '\0';
			strcpy(result[k], tt);
			++k;
			q = -1;
			if (i == '\0')
				break;
		}
		else
		tt[q] = i;
	}
	result[++k][0] = NULL;
	return result;
}

char * splitCommand(char * str)
{
	for (int i = 0; i < childSize; ++i)//初始化result数组
		memset(result[i], '\0', childSize * sizeof(char));
	char* adr = str;
	int k = 0;
	int q = 0;
	char tt[50];
	for (char i = *adr;; i = *(++adr), ++q) {

		if (i == ' ' || i == '\0') {
			tt[q] = '\0';
			strcpy(result[k], tt);
			++k;
			q = -1;
			if (i == '\0')
				break;
		}
		else
			tt[q] = i;
	}
	result[++k][0] = NULL;
	return result;
}

//分割字符，获取str字符串最后一个分隔符后的所有字符
char * splitForFileName(char *str, char *split)
{
	char tt[50];
	if(strstr(str,split)==NULL)
	return str;
	else {
		int k = 0;
		int q = 0;
		char a[2];
		a[1] = '\0';
		for (int i = 0;i<strlen(str) ; ++i,++q) {
			a[0] = str[i];
			if (strcmp(split,a)==0) {
				strcpy(tt, "");
				q = -1;
			}
			else {
				tt[q] = a[0];
				tt[q + 1] = '\0';
			}
		}
		return tt;
	}
}

//从指定开始目录找，当前找不到就到目录的子目录找，直到找到符合条件的。
//此函数只找一个名字的目录（即不包含‘/’），不找文件类型
FCBList findPath(char *a, FCBList start)
{
	if (strcmp(a, "root") == 0)
		return content[0];
	FCBList temp = start;
	for (int i = 0; i < childSize; ++i) {
		if (temp->childs[i] >= 0&& content[temp->childs[i]]->iden==1) {
			if (strcmp(content[temp->childs[i]]->fileId, a) == 0) {
				return content[temp->childs[i]];
			}
		}
	}
	for (int i = 0; i < childSize; ++i) {
		if (temp->childs[i] >= 0 && content[temp->childs[i]]->iden == 1)
		findPath(a, content[temp->childs[i]]);
	}
	return NULL;
}

//找到指定文件路径的文件或目录的父目录，并把a改变为选中的文件或文件目录
//寻找成功：返回控制块，寻找失败：返回NULL
FCBList findPrePath(char *a,FCBList cur)
{
	char path[500];
	for (int i = 0; i < childSize; ++i)//初始化result数组
		memset(result[i], '\0', childSize * sizeof(char));
	strcpy(path, a);
	FCBList temp = content[0];
	if (strstr(path, "/") != NULL)//如果在当前路径寻找文件，则不分割路径
	{
		split(path, "/");
	}
	else
		temp = cur;
	
	int i;
	//切换到相应路径
	FCBList t = temp;
	for (i = 0; i < childSize && strcmp(result[i + 1], "") != 0; ++i) {
		if ((t = findPath(result[i], temp)) != NULL) {
			temp = t;
		}
		else
			return NULL;
	}
	if (strstr(result[i], " ") != NULL)
	strcpy(a, result[i]);
	return temp;
}

//寻找空的文件块
//寻找成功：返回索引，寻找失败：返回-1
int findFATEmpty()
{
	for (int i = 0; i < 512; ++i) {
		if (strcmp(FAT[i].contain, "") == 0)
			return i;
	}
	return -1;
}

//寻找当前控制块的子目录子文件位是否有为占用的
//寻找成功：返回索引，寻找失败：返回-1
int findChildsEmpty(FCBList cur) {
	for (int i = 0; i < childSize; ++i)
		if (cur->childs[i] < 0)
			return i;
	return -1;
}

//获取source数组中从start位置开始算的count个字符，并复制到des数组
char * subString(char * des, char * source, int start, int count)
{
	int j= 0;
	for (int i = start; i < start + count; ++i,++j)
		des[j] = source[i];
	des[j] = '\0';
	return des;
}
