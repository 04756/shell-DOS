#include "fileSystem.h"

//����һ���ļ�
FCBList md(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			int k;
			for (k = 0; k < childSize; ++k) {
				if (cur->childs[k] >= 0 && strcmp(content[cur->childs[k]]->fileId, a) == 0 && content[cur->childs[k]]->iden == 0) {
					printf("�������Ѵ��ڣ�\n");
					return NULL;
				}
			}
			k = findChildsEmpty(cur);
			if (k == -1) {
				printf("\nĿ¼�ļ��ռ�������\n");
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

//����һ���ļ���
FCBList mkdir(FCBList cur, char *a)
{
	int i = 0;
	for (i; i<400; ++i)
		if (content[i]->fMode == 3) {
			int k;
			for (k = 0; k < childSize; ++k) {
				if (cur->childs[k] >= 0 && strcmp(content[cur->childs[k]]->fileId, a) == 0 && content[cur->childs[k]]->iden == 1) {
					printf("�������Ѵ��ڣ�\n");
					return NULL;
				}
			}
			k = findChildsEmpty(cur);
			if (k == -1) {
				printf("\nĿ¼�ļ��ռ�������\n");
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

//�Է�ҳ��ʽ�鿴�ļ�
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
			printf("\n\n��'n'��ĸ����ҳ,��������˳�\n");
		}
	}
	return;
}

//��ָ���ļ����ļ����ƶ���ָ��Ŀ¼
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
	printf("\n�Ҳ���ָ��·��\n");
	return;
}
success:{
	  write_file();
}
}

//�л�Ŀ¼
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
	printf("�Ҳ���ָ��·��\n"); 
	return;
	}
 success:{
	  return cur;
}
}

//��ָ���ļ����ļ��и��Ƶ�ָ��Ŀ¼
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
	
	des = md(des, a);//��Ŀ���ļ����´����ļ�
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
	printf("\n�Ҳ���ָ��·��\n");
}
success:{
	 write_file();
}
}

//��ʾ��ǰĿ¼�ļ��Լ��ļ���
void dir(FCBList cur ,char *a)
{
	read_file();
	printf("�����ļ�ϵͳ\n");
	printf("��ǰλ�ã�/%s\n\n", cur->fileId);
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

//ɾ���ļ�
void delete(char* a, FCBList cur){
	rm(a, cur);
}

//�༭�ļ�
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
	printf("\n�Ҳ���ָ���ļ�\n");
	}
 success:{
	  time_t t;
	  time(&t);
	  temp->chang = localtime(&t);
	  write_file();
}
}

//�鿴�ļ���ֱ�Ӳ鿴�����ļ�����
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

//����ָ���ļ���ָ������
void find(char *a)
{
	splitCommand(a);
	char searchStr[500];
	char path[500];
	strcpy(searchStr, result[0]);//�����ļ���
	strcpy(path, result[1]);//����·��
	for (int i = 0; i < childSize; ++i)//��ʼ��result����
		memset(result[i], '\0', childSize * sizeof(char));
	if(strstr(path, "/")!=NULL)//����ڵ�ǰ·��Ѱ���ļ����򲻷ָ�·��
	split(path,"/");
	FCBList temp = content[0];
	printf("\n--------------%s\n\n", path);
	int i;
	//�л�����Ӧ·��
	FCBList t = temp;
	for (i = 0; i < childSize && strcmp(result[i+1], "") != 0; ++i) {
		if ((t = findPath(result[i], temp)) != NULL)
			temp = t;
	}
	//���ļ������Ƶ�path����
	if (strcmp("",result[i])!=0)
		strcpy(path, result[i]);
	//ָ�����ļ�
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

//��ʾ�ļ��Ĳ���Ȩ��
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
				printf("ֻ�ɶ�\t%s\n", a);
				break;
			case 1:
				printf("ֻ��д\t%s\n", a);
				break;
			case 2:
				printf("�ɶ���д\t%s\n", a);
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
	printf("\n�Ҳ���ָ��·��\n");
	}
success: {
}
}

//ɾ���ļ�
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
	printf("\n�Ҳ���ָ���ļ�\n");
success:{
	write_file();
}
}

//ɾ���ļ���
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
						printf("�Ƿ�ȷ��ɾ��y/n?\t%s\t", a);
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
			printf("�Ƿ�ȷ��ɾ��y/n?\t%s\t", a);
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
	printf("\n�Ҳ���ָ���ļ�\n");
	}
  success: {
	  write_file();
}
}

//������ָ���ļ����ļ���
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
	printf("\nû���ҵ�ָ���ļ�\n");
}

//��ȡ����ʱ��
void getTime()
{
	time_t t;
	struct tm* lt;
	time(&t);
	lt = localtime(&t);
	printf("%02d:%02d:%02d\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
}

//�汾��Ϣ
void ver()
{
	printf("�ļ�ģ��ϵͳ\n2018/6/6 CMD author:�����\n\n");
}

//����
void help()
{
	printf("md [�ļ���]\t�����ļ�\n");
	printf("mkdir [Ŀ¼��]\t����Ŀ¼\n");
	printf("more [�ļ���]\t�Է�ҳģʽ��ʾ�ļ�\n");
	printf("move [Դ�ļ�����Ŀ¼��][Ŀ��Ŀ¼]\t�ƶ��ƶ��ļ���Ŀ¼��ָ���ص�\n");
	printf("cd [·��]\t�л���ǰ·��\n");
	printf("copy [Դ�ļ���][Ŀ��Ŀ¼]\t����ָ���ļ���ָ��Ŀ¼\n");
	printf("dir [�����ַ�][����]\t��ʾ��ǰ·���°��������ַ���Ŀ¼���ļ�,�粻��������ַ����������/s������ʾ������Ŀ¼���ļ�\n");
	printf("delete [�ļ���]\tɾ���ļ�\n");
	printf("edit [�ļ���]\t�༭�ļ�\n");
	printf("type [�ļ���]\t��ʾ�ļ�����\n");
	printf("find [�ַ���][�ļ���]\t����ָ���ļ����Ƿ���������ַ���������������������һ��\n");
	printf("attrib [�ļ���]\t��ʾ�ļ��Ķ�ȡȨ��\n");
	printf("rm [�ļ���]\tɾ���ļ�\n");
	printf("rmdir [�ļ���][����]\tɾ���ļ��У������������'/s'����ÿɾ��һ���ļ����ļ��ж�ѯ���û�\n");
	printf("rename [�ļ���]\t�����ļ�����\n");
	printf("time\t��ȡ��ǰʱ��\n");
	printf("ver\t��ȡ��ǰϵͳ����\n");
	printf("help\t�����������ĵ�\n");
	printf("import [�ļ���][Ŀ¼]\t��windowsϵͳ�������ļ���ָ��Ŀ¼\n");
	printf("export [�ļ���][Ŀ¼]\t�ӵ�ǰϵͳѰ��ָ���ļ�д��windowsϵͳ\n");
}

//��windowsϵͳ�����ļ�
void import(char *a, FCBList cur)
{
	splitCommand(a);
	char b[MAXNAME];
	strcpy(a, result[0]);
	strcpy(b, result[1]);
	FCBList temp = cur;
	
	//�ҵ������Ŀ���ļ���
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
		printf("\n�Ҳ���ָ���ļ���\n");
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
	printf("\n�Ҳ���ָ��·����\n");
	return;
}
success:{
	 write_file();
}
}

//�ӱ�ϵͳ��windowsϵͳ�����ļ�������д��C��
void export_(char *a, FCBList cur)
{
	splitCommand(a);
	char b[50];
	strcpy(a, result[0]);//��ϵͳ��Դ�ļ�
	strcpy(b, result[1]);//��ϵͳ��Ŀ��Ŀ¼
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
		 printf("\nû���ҵ�ָ���ļ�\n");
	}
}

//��ʼ��ϵͳ
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

//д�������ļ�����
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

	/*���ɹ̶���С���ļ���������������洢����*/
	OFSTRUCT ofs;
	DWORD dwWritenSize;							//��¼ʵ��д����ֽ�����DWORD����Ϊint
	static char chWriteBuffer[BUFSIZE];		//����һ��1M��д�뻺����
	HANDLE pFile = (HANDLE)OpenFile("shell", &ofs, OF_CREATE | OF_READWRITE);	//�����ɶ�д��ʽ���ļ�
	memset(chWriteBuffer, 0, BUFSIZE);		//�����1M�Ļ�������ȫ��д��0
	WriteFile(pFile, chWriteBuffer, BUFSIZE, &dwWritenSize, NULL);		//д�����ݣ�����һ��1M���ļ�
	CloseHandle(pFile);		//�ر��ļ����
						/*��ʼ���ļ�ϵͳ�����ļ�ϵͳ��ͷ�����������飬д��Ŀ¼����*/

	pFile = (HANDLE)OpenFile("shell", &ofs, OF_READWRITE);	//�ö�д��ʽ���ļ�
	WriteFile(pFile, &SUPER, sizeof(super_block), &dwWritenSize, NULL);			//д�볬����
	for(int i= 0;i<DIRECTORYNUM;++i)
	WriteFile(pFile, content[i], sizeof(FCB), &dwWritenSize, NULL);	//д��Ŀ¼
	for (int i = 0; i<FILEBLOCKNUM; ++i)
	WriteFile(pFile, FAT[i].contain, sizeof(Block), &dwWritenSize, NULL);		//д���ļ����ݿ�
	CloseHandle(pFile);		//�ر��ļ����
}

//��ȡ�����ļ�����
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
	DWORD dwWritenSize;							//��¼ʵ��д����ֽ�����DWORD����Ϊint
	HANDLE pFile = (HANDLE)OpenFile("shell", &ofs, OF_READWRITE);	//�ö�д��ʽ���ļ�
	ReadFile(pFile, &SUPER, sizeof(super_block), &dwWritenSize, NULL);			//д�볬����
	for (int i = 0; i<DIRECTORYNUM; ++i)
		ReadFile(pFile, content[i], sizeof(FCB), &dwWritenSize, NULL);	//д��Ŀ¼
	for (int i = 0; i<FILEBLOCKNUM; ++i)
	ReadFile(pFile, FAT[i].contain, sizeof(Block), &dwWritenSize, NULL);		//д���ļ����ݿ�
	CloseHandle(pFile);		//�ر��ļ����
}

//��ȡ����·��
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
	for (int i = 0; i < childSize; ++i)//��ʼ��result����
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
	for (int i = 0; i < childSize; ++i)//��ʼ��result����
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

//�ָ��ַ�����ȡstr�ַ������һ���ָ�����������ַ�
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

//��ָ����ʼĿ¼�ң���ǰ�Ҳ����͵�Ŀ¼����Ŀ¼�ң�ֱ���ҵ����������ġ�
//�˺���ֻ��һ�����ֵ�Ŀ¼������������/�����������ļ�����
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

//�ҵ�ָ���ļ�·�����ļ���Ŀ¼�ĸ�Ŀ¼������a�ı�Ϊѡ�е��ļ����ļ�Ŀ¼
//Ѱ�ҳɹ������ؿ��ƿ飬Ѱ��ʧ�ܣ�����NULL
FCBList findPrePath(char *a,FCBList cur)
{
	char path[500];
	for (int i = 0; i < childSize; ++i)//��ʼ��result����
		memset(result[i], '\0', childSize * sizeof(char));
	strcpy(path, a);
	FCBList temp = content[0];
	if (strstr(path, "/") != NULL)//����ڵ�ǰ·��Ѱ���ļ����򲻷ָ�·��
	{
		split(path, "/");
	}
	else
		temp = cur;
	
	int i;
	//�л�����Ӧ·��
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

//Ѱ�ҿյ��ļ���
//Ѱ�ҳɹ�������������Ѱ��ʧ�ܣ�����-1
int findFATEmpty()
{
	for (int i = 0; i < 512; ++i) {
		if (strcmp(FAT[i].contain, "") == 0)
			return i;
	}
	return -1;
}

//Ѱ�ҵ�ǰ���ƿ����Ŀ¼���ļ�λ�Ƿ���Ϊռ�õ�
//Ѱ�ҳɹ�������������Ѱ��ʧ�ܣ�����-1
int findChildsEmpty(FCBList cur) {
	for (int i = 0; i < childSize; ++i)
		if (cur->childs[i] < 0)
			return i;
	return -1;
}

//��ȡsource�����д�startλ�ÿ�ʼ���count���ַ��������Ƶ�des����
char * subString(char * des, char * source, int start, int count)
{
	int j= 0;
	for (int i = start; i < start + count; ++i,++j)
		des[j] = source[i];
	des[j] = '\0';
	return des;
}
