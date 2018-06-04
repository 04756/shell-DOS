#include "fileSystem.h"

FCBList md(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			content[i]->iden = 0;
			content[i]->fMode = 2;
			content[i]->father = cur->adr;
			strcpy(content[i]->fileId, a);
			for (int k = 0; k < childSize; ++k) {
				if (cur->childs[k]<0) {
					cur->childs[k] = i;
					break;
				}
			}
			break;
		}
	return content[i];
}

FCBList mkdir(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			content[i]->iden = 1;
			content[i]->fMode = 2;
			content[i]->father = cur->adr;
			strcpy(content[i]->fileId, a);
			for (int k = 0; k < childSize; ++k) {
				if (cur->childs[k] < 0) {
					cur->childs[k] = i;
					break;
				}
			}
			break;
		}
	return content[i];
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
	for (int i = 0; i < childSize; ++i) {
		if (source->childs[i] >= 0 && content[source->childs[i]]->iden == 0 && strcmp(content[source->childs[i]]->fileId, a) == 0){
			source = content[source->childs[i]];
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
	split(a, "/");
	int ii = 0;
	while(strcmp(result[ii],"")==0)
	strcpy(a, result[ii]);
	des = md(des, a);
	for (int i = 0,j = 0; i < childSize; ++i) {
		if (source->childs[i] >= 0) {
			int q = findFATEmpty();
			des->childs[j] = q;
			strcpy(FAT[q].contain,FAT[source->childs[i]].contain);
			++j;
		}
	}
fail:{
	printf("\n找不到指定路径\n");
}
}

void dir(FCBList cur)
{
	printf("虚拟文件系统\n");
	printf("当前位置：/%s\n\n", cur->fileId);
	for (int i = 0; i < childSize; ++i) {
		if (cur->childs[i] >= 0) {
			printf("%s\t", content[cur->childs[i]]->fileId);
			if (content[cur->childs[i]]->iden == 1)
				printf("<DIR>\n");
			else
				printf("<FIFL>\n");
		}
	}
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
			for (q; q < 512; ++q) {
				if (FAT[q].contain != "") {
					temp->childs[i] = q;
					char newLine = "";
					int k = 0;
					while (k < 1024) {
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
			}
		}
	}
fail: {
	printf("\n找不到指定文件\n");
	}
success:{}
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

char* split(char *str, char *s)
{
	for (int i = 0; i < childSize; ++i)//初始化result数组
		memset(result[i], '\0', childSize * sizeof(char));
	char* adr = strstr(str, s);
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

FCBList findPrePath(char * a)
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
	//把文件名复制到path变量
	if (strcmp("", result[i]) != 0)
		strcpy(path, result[i]);
	//指定的文件
	for (int q = 0; q < childSize; ++q) {
		if (temp->childs[q] >= 0 && content[temp->childs[q]]->iden == 0 && strcmp(content[temp->childs[q]]->fileId, path) == 0) {
			temp = content[temp->childs[q]];
			a = path;
			return temp;
		}
	}
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
