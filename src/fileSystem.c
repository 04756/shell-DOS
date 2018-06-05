
#include "fileSystem.h"
//#include "test.h"

FCBList md(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			for (int k = 0; k < childSize; ++k) {
				if (cur->childs[k] >= 0 && strcmp(content[cur->childs[k]]->fileId, a) == 0 && content[cur->childs[k]]->iden == 0) {
					printf("该名字已存在！\n");
					return NULL;
				}
				if (cur->childs[k]<0) {
					content[i]->iden = 0;
					content[i]->fMode = 2;
					content[i]->father = cur->adr;
					content[i]->adr = i;
					time_t t;
					time(&t);
					content[i]->setUp = localtime(&t);
					strcpy(content[i]->fileId, a);
					cur->childs[k] = i;
					break;
				}
			}
			break;
		}
	write_file();
	return content[i];
}

FCBList mkdir(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			
			for (int k = 0; k < childSize; ++k) {
				if (cur->childs[k] >= 0 && strcmp(content[cur->childs[k]]->fileId, a) == 0 && content[cur->childs[k]]->iden == 1) {
					printf("该名字已存在！\n");
					return NULL;
				}
				if (cur->childs[k] < 0) {
					cur->childs[k] = i;
					content[i]->iden = 1;
					content[i]->fMode = 2;
					content[i]->father = cur->adr;
					content[i]->adr = i;
					time_t t;
					time(&t);
					content[i]->setUp = localtime(&t);
					strcpy(content[i]->fileId, a);
					break;
				}
			}
			break;
		}
	write_file();
	return content[i];
}

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
			}
			if(flag == 0)
			printf("%s", FAT[temp->childs[i]].contain);
			printf("\n\n按'n'字母键翻页\n");
		}
	}
	return;
}

void move(char *a, FCBList cur)
{
	char b[50];
	splitCommand(a);
	strcpy(a, result[0]);
	strcpy(b, result[1]);
	FCBList source = findPrePath(a);

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
	for (int i = 0; i < childSize; ++i) {
		if (des->childs[i] < 0) {
			des->childs[i] = source->adr;
			source->father = des->adr;
			goto success;
		}
	}
		
fail: {
	printf("\n找不到指定路径\n");
	return;
}
success:{
	  write_file();
}
}

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

fail: {
	printf("找不到指定路径\n"); }
  success:{}
	  for (int i = 0; i < childSize; ++i)
		  memset(result[i],'\0', childSize * sizeof(char));
	  return cur;
}

void copy(FCBList cur, char *a)
{
	char b[50];
	splitCommand(a);
	strcpy(a, result[0]);
	strcpy(b, result[1]);
	FCBList source = findPrePath(a);
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

void dir(FCBList cur)
{
	printf("虚拟文件系统\n");
	printf("当前位置：/%s\n\n", cur->fileId);
	for (int i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0) {
			//printf("%d%d%d %02d:%02d:%02d\t%s\t", content[cur->childs[i]]->setUp->tm_year+1900, content[cur->childs[i]]->setUp->tm_mon, content[cur->childs[i]]->setUp->tm_mday, content[cur->childs[i]]->setUp->tm_hour, content[cur->childs[i]]->setUp->tm_min, content[cur->childs[i]]->setUp->tm_sec,content[cur->childs[i]]->fileId);
			printf("%s\t",content[cur->childs[i]]->fileId);
			if (content[cur->childs[i]]->iden == 1)
				printf("<DIR>\n");
			else
				printf("<FIFL>\n");
		}
	}
}

void delete(char* a, FCBList cur){
	rm(a, cur);
}

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
}

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

void rm(char *a, FCBList cur)
{
	int adressOfCurInFathe;
	for (int i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0&&strcmp(a,content[cur->childs[i]]->fileId)==0&& content[cur->childs[i]]->iden == 0){
			for (int j = 0; j < childSize; ++j) {
				if (content[cur->childs[i]]->childs[j] >= 0)
					strcpy(FAT[content[cur->childs[i]]->childs[j]].contain,'\0');
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

void rmdir(char *a, FCBList cur)
{
	int adressOfCurInFathe;
	if (strstr(a, "/") != NULL) {
		split(a, "/");
		cur = content[0];
		for (int i = 0; i < childSize && strcmp(result[i], "") != 0; ++i) {
			FCBList t = findPath(result[i], cur);
			if (t != NULL)
				cur = t;
			else
				goto fail;
		}
	}
	else {
		for (int i = 0; i < childSize; ++i) {
			if (cur->childs[i] >= 0 && strcmp(a, content[cur->childs[i]]->fileId) == 0) {
				cur = content[cur->childs[i]];
				adressOfCurInFathe = i;
				break;
			}
		}
	}
	if (strstr(a, "/s") == NULL) {
		for (int i = 0; i < childSize; ++i) {
			if (cur->childs[i] >= 0) {

				if (content[cur->childs[i]]->iden == 0) {
					printf("是否确认删除y/n?\t%s", content[cur->childs[i]]->fileId);
					char confirm = getchar();
					if (confirm == "y")
						rm(content[cur->childs[i]]->fileId, cur);
				}
				else {
					rmdir(content[cur->childs[i]]->fileId, cur);
				}
			}
		}
	}
		if (cur->fMode != 3) {
			printf("是否确认删除y/n?\t%s\t", cur->fileId);
			char confirm[2];
			gets(confirm);
			if (strcmp(confirm,"y")==0) {
				content[cur->father]->childs[adressOfCurInFathe] = -1;
				content[cur->adr] = (FCBList)malloc(sizeof(FCB));
				content[cur->adr]->fMode = 3;
				goto success;
			}
		}

fail: {
		printf("\n找不到指定文件\n");
		}
success: {
		  write_file();
	}
}

void rename_(char * a, FCBList cur)
{
	splitCommand(a);
	strcpy(a, result[0]);
	char b[MAXNAME];
	strcpy(b, result[1]);
	FCBList ans = findPrePath(a);
	if (ans == NULL)
		ans = cur;
	for (int i = 0; i < childSize; ++i) {
		if (ans->childs[i] >= 0 && strcmp(content[ans->childs[i]]->fileId, a) == 0) {
			strcpy(content[ans->childs[i]]->fileId, b);
			return;
		}
	}
	printf("\n没有找到指定文件\n");
}

void getTime()
{
	time_t t;
	struct tm* lt;
	time(&t);
	lt = localtime(&t);
	printf("%02d:%02d:%02d\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
}

void ver()
{
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(os);
	if(GetVersionEx((OSVERSIONINFO*)&os)){
		char firstVersion[10] = "windows10";
		char secondVersion[10] = "windows8";
		char thirdVersion[10] = "windows7";
		char *ver;
		if (IsWindows10OrGreater()>=10)
			ver == firstVersion;
		else if (IsWindows8OrGreater())
			ver = secondVersion;
		else if (IsWindows7OrGreater());
			ver = thirdVersion;
		printf("\n系统版本号： %s\n", ver);
		printf("\n%ld\n", os.dwMajorVersion);

	}
	//else {
		//printf("\n获取版本信息出错\n");
	//}
}

void import(char *a, FCBList cur)
{
	splitCommand(a);
	char b[MAXNAME];
	strcpy(a, result[0]);
	strcpy(b, result[1]);
	
}

void initFile()
{
	for (int i = 0; i < 600; ++i) {
		content[i] = (FCBList)malloc(sizeof(FCB));
		content[i]->fMode = 3;
		strcpy(content[i]->fileId, "");
		strcpy(content[i]->permissions, "");
		content[i]->iden = 3;
		content[i]->fileSize = 0;
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
	for (int i = 0; i < 963; ++i)
		memset(FAT[i].contain, '\0', sizeof(char) * 1024);
	SUPER.dev_id = 000001;
	SUPER.s_blocksize = 1024 * 1024;
	SUPER.remainSize = 1024 * 1024;
	if (_access("shell", 0) == -1)
		write_file();
	else
		read_file();
}

void write_file()
{
	FILE* fp;
	fopen_s(&fp, "shell", "wb");
	fwrite(&SUPER,sizeof(super_block),1,fp);
	for (int i = 0; i < 600; ++i) {
			fwrite(content[i], sizeof(FCB), 1, fp);
	}
	for (int i = 0; i < 963; ++i) {
			fwrite(FAT[i].contain, sizeof(Block), 1, fp);
	}
	fclose(fp);
}

void read_file()
{
	//pid_t r;
	//r = fork();
	FILE* fp;
	fopen_s(&fp, "shell", "rb");
	int offset = sizeof(super_block);
	fread(&SUPER, sizeof(super_block), 1, fp);
	fseek(fp, sizeof(super_block),0);
	for (int i = 0; i < 600; ++i) {
		fread(content[i], sizeof(FCB), 1, fp);
		offset += sizeof(FCB);
		fseek(fp, offset, 0);
	}
	for (int i = 0; i < 963; ++i) {
		fread(FAT[i].contain, sizeof(Block), 1, fp);
		offset += sizeof(FCB);
		fseek(fp, offset, 0);
	}
	fclose(fp);
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

//找到指定文件路径的文件或目录的父目录，并把a改变为选着的文件或文件目录
FCBList findPrePath(char *a)
{
	char path[500];
	for (int i = 0; i < childSize; ++i)//初始化result数组
		memset(result[i], '\0', childSize * sizeof(char));
	strcpy(path, a);
	if (strstr(path, "/") != NULL)//如果在当前路径寻找文件，则不分割路径
	{
		split(path, "/");
	}
	else
		return NULL;

	FCBList temp = content[0];
	int i;
	//切换到相应路径
	FCBList t = temp;
	for (i = 0; i < childSize && strcmp(result[i + 1], "") != 0; ++i) {
		if ((t = findPath(result[i], temp)) != NULL)
			temp = t;
	}
	strcpy(a, result[i]);
	return temp;
	//把文件名复制到path变量
	/*if (strcmp("", result[i]) != 0)
		strcpy(path, result[i]);
	//指定的文件
	for (int q = 0; q < childSize; ++q) {
		if (temp->childs[q] >= 0 && content[temp->childs[q]]->iden == 0 && strcmp(content[temp->childs[q]]->fileId, path) == 0) {
			temp = content[temp->childs[q]];
			a = path;
			return temp;
		}
	}*/
}

int findFATEmpty()
{
	for (int i = 0; i < 512; ++i) {
		if (strcmp(FAT[i].contain, "") == 0)
			return i;
	}
}

char * subString(char * des, char * source, int start, int count)
{
	int j= 0;
	for (int i = start; i < start + count; ++i,++j)
		des[j] = source[i];
	des[j] = '\0';
	return des;
}
