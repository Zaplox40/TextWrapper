#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define _GNU_SOURCE
#include <dirent.h>
#include <libgen.h>

int is_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int is_dir(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISDIR(path_stat.st_mode);
}

typedef struct node {
	char* word;
	struct node* next;
} node_t;

int printlist(node_t* head) {
	
	node_t* ptr=head;
	char*pos;
	
	while(ptr->next!=NULL) {
		if((pos=strchr(ptr->word,'\n')) != NULL) {
			*pos='\0';
		}
		printf("%s",ptr->word);
		printf(" ");
		ptr=ptr->next;
	}
	
	if(strchr(ptr->word,'\n')!=0) {
		printf("%s",ptr->word);
	}
	else {
		printf("%s\n",ptr->word);
	}
	
	return 1;	
}

int freelist(node_t** head) {
	
	node_t* ptr=*head;
	node_t* next;
	
	while(ptr != NULL) 
	{
		next=ptr->next;
		free(ptr->word);
		free(ptr);
		ptr=next;
	}
	
	*head=NULL;
	
	return 1;
}

int writeSpace(char* path) 
{
	FILE* newfile;
	char wrap[256],newpath[256],pathcpy[256];
	char* base;
	char* dir;
	char ch ='/';		
	strcpy(wrap,"wrap.");
	strcpy(pathcpy,path);
	base = basename(path);
	dir = dirname(pathcpy);
	strcat(wrap,base);
	strcpy(newpath,dir);
	strncat(newpath,&ch,1);
	strcat(newpath,wrap);
	newfile = fopen(newpath,"a");
	
	if(newfile==NULL)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	else
	{
		fprintf(newfile,"\n");
	}
	
	fclose(newfile);
	
	return 1;


}

int writeToFile(node_t* head,char* path) 
{
	FILE* newfile;
	char wrap[256],newpath[256],pathcpy[256];
	char* base;
	char* dir;
	char ch ='/';		
	strcpy(wrap,"wrap.");
	strcpy(pathcpy,path);
	base = basename(path);
	dir = dirname(pathcpy);
	strcat(wrap,base);
	strcpy(newpath,dir);
	strncat(newpath,&ch,1);
	strcat(newpath,wrap);
	newfile = fopen(newpath,"a");
	node_t* ptr;
	char* pos;
	
	if(newfile==NULL)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	else
	{
		ptr=head;
		while(ptr->next!=NULL) 
		{
			if((pos=strchr(ptr->word,'\n')) != NULL)
			{
				*pos='\0';
			}
			fprintf(newfile,"%s",ptr->word);
			fprintf(newfile,"%s"," ");
			ptr=ptr->next;
		}
		if(strchr(ptr->word,'\n')!=0) 
		{
			
			fprintf(newfile,"%s",ptr->word);
		}
		else
		{
			fprintf(newfile,"%s\n",ptr->word);
		}
	}
	
	fclose(newfile);
	
	return 1;

}

int writeError(char* word,char* path) 
{
	FILE* newfile;
	char wrap[256],newpath[256],pathcpy[256];
	char* base;
	char* dir;
	char ch ='/';		
	strcpy(wrap,"wrap.");
	strcpy(pathcpy,path);
	base = basename(path);
	dir = dirname(pathcpy);
	strcat(wrap,base);
	strcpy(newpath,dir);
	strncat(newpath,&ch,1);
	strcat(newpath,wrap);
	newfile = fopen(newpath,"a");
	
	if(newfile==NULL)
	{
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	else
	{	
		if(strchr(word,'\n')!=0) 
		{
			
			fprintf(newfile,"%s",word);
		}
		else
		{
			fprintf(newfile,"%s\n",word);
		}
	}
	
	fclose(newfile);
	
	return 1;

}

int wrapfile(char* path,int type, int width) 
{
	if(type==0) 
	{
		char* line=NULL;
		char* tokens;
		ssize_t read;
		size_t len = 0;
		int i=0;
		int wordlength=0;
		int errorcheck=0;
		int k=0;
		int total=0;
		node_t* head=NULL;
		node_t* ptr;
		char delimit[]=" \n\t\v\f\r";

		while((read=getline(&line,&len,0)) != -1) {
			if(line[0] == '\n' && head !=NULL) {
				printlist(head);
				freelist(&head);
				printf("\n");
				continue;
			}
			tokens = strtok(line,delimit);
			while(tokens != NULL) {
				wordlength = strlen(tokens);
				if(wordlength > width) {
					printlist(head);
					printf("%s\n",tokens);
					freelist(&head);
					errorcheck=1;
					total=0;
					continue;
				}
				if(total+wordlength>=width) {
					printlist(head);//create function printline set
					freelist(&head);//free lineset	
					head = (node_t*)malloc(sizeof(node_t));
					head->word=(char*)malloc(wordlength+1);
					head->next=NULL;
						//printf("%d\n",wordlength);
					strcpy(head->word,tokens);
					ptr=head;//add last token to the new lineset and increase length	
					total=wordlength+1;

				}
				else 
				{
					total+=wordlength+1;
					if(head == NULL) {
						head = (node_t*)malloc(sizeof(node_t));
						head->word=(char*)malloc(wordlength+1);
						head->next=NULL;
						strcpy(head->word,tokens);
						ptr=head;
					}
					else {
						node_t* n = (node_t*)malloc(sizeof(node_t));
						n->word=(char*)malloc(wordlength+1);
						n->next=NULL;
						strcpy(n->word,tokens);
						ptr->next=n;
						ptr=n;
					}
				}
				
				tokens = strtok(NULL," ");
				
				if(tokens == NULL) {
					total--;
				}
			}
		}

		if(head != NULL) {
			printlist(head);
			freelist(&head);	
		}
		if(errorcheck==1) {
			exit(EXIT_FAILURE);
		}
				
		return 0;		
	}
	if(type==1) 
	{
		FILE* fp;
		char* line = NULL;
		size_t len = 0;
		ssize_t read;
		char* tokens;
		int i=0;
		int wordlength=0;
		int errorcheck=0;
		int k=0;
		int total=0;
		node_t* head=NULL;
		node_t* ptr;
		char delimit[]=" \n\t\v\f\r";
		
		fp = fopen(path,"r");
		if(fp == NULL) {
			perror("Error:");
			exit(EXIT_FAILURE);
		}
		
		while((read=getline(&line,&len,fp)) != -1) {
			if(line[0] == '\n' && head !=NULL) {
				printlist(head);
				freelist(&head);
				printf("\n");
				continue;
			}
			tokens = strtok(line,delimit);
			while(tokens != NULL) {
				wordlength = strlen(tokens);
				if(wordlength > width) {
					printlist(head);
					printf("%s\n",tokens);
					freelist(&head);
					errorcheck=1;
					total=0;
					continue;
				}
				if(total+wordlength>=width) {
					printlist(head);//create function printline set
					freelist(&head);//free lineset	
					head = (node_t*)malloc(sizeof(node_t));
					head->word=(char*)malloc(wordlength+1);
					head->next=NULL;
						//printf("%d\n",wordlength);
					strcpy(head->word,tokens);
					ptr=head;//add last token to the new lineset and increase length	
					total=wordlength+1;
				}
				else 
				{
					total+=wordlength+1;
					if(head == NULL) {
						head = (node_t*)malloc(sizeof(node_t));
						head->word=(char*)malloc(wordlength+1);
						head->next=NULL;
						strcpy(head->word,tokens);
						ptr=head;
					}
					else {
						node_t* n = (node_t*)malloc(sizeof(node_t));
						n->word=(char*)malloc(wordlength+1);
						n->next=NULL;
						strcpy(n->word,tokens);
						ptr->next=n;
						ptr=n;
					}
				}
				
				tokens = strtok(NULL," ");
				
				if(tokens == NULL) {
					total--;
				}
			}
		}

		if(head != NULL) {
			printlist(head);
			freelist(&head);	
		}
		
		fclose(fp);
		
		if(line) {
			free(line);
		}
		
		if(errorcheck==1) {
			exit(EXIT_FAILURE);
		}
				
		return 0;		
	}
	
	if(type==2)
	{
		if(is_file(path) != 0) 
		{
			FILE* fp;
			char* line = NULL;
			size_t len = 0;
			ssize_t read;
			char* tokens;
			int i=0;
			int wordlength=0;
			int errorcheck=0;
			int k=0;
			int total=0;
			node_t* head=NULL;
			node_t* ptr;
			char delimit[]=" \n\t\v\f\r";
			fp = fopen(path,"r");
			
			if(fp == NULL) {
				perror("Error:");
				exit(EXIT_FAILURE);
			}
		
			while((read=getline(&line,&len,fp)) != -1) {
				if(line[0] == '\n' && head !=NULL) 
				{
					writeToFile(head,path);
					freelist(&head);
					writeSpace(path);
					total=0;
					continue;
				}
				tokens = strtok(line,delimit);
				while(tokens != NULL) {
					wordlength = strlen(tokens);
					if(wordlength > width) {
						writeToFile(head,path);
						writeError(tokens,path);
						freelist(&head);
						errorcheck=1;
						total=0;
						continue;
					}
					if(total+wordlength>=width) {
						writeToFile(head,path);//create function printline set
						freelist(&head);//free lineset	
						head = (node_t*)malloc(sizeof(node_t));
						head->word=(char*)malloc(wordlength+1);
						head->next=NULL;
						strcpy(head->word,tokens);
						ptr=head;//add last token to the new lineset and increase length	
						total=wordlength+1;
					}
					else 
					{
						total+=wordlength+1;
						if(head == NULL) {
							head = (node_t*)malloc(sizeof(node_t));
							head->word=(char*)malloc(wordlength+1);
							head->next=NULL;
							strcpy(head->word,tokens);
							ptr=head;
						}
						else 
						{
							node_t* n = (node_t*)malloc(sizeof(node_t));
							n->word=(char*)malloc(wordlength+1);
							n->next=NULL;
							strcpy(n->word,tokens);
							ptr->next=n;
							ptr=n;
						}
					}
				
					tokens = strtok(NULL," ");
				
					if(tokens == NULL) {
						total--;
					}
				}
			}

			if(head != NULL) 
			{
				writeToFile(head,path);
				freelist(&head);	
			}
		
			fclose(fp);
			
			if(line) {
				free(line);
			}
				
			return 0;		
		}
		else if(is_dir(path)!=0) 
		{
			struct dirent *de;
			DIR* dr = opendir(path);
			char* wraptest="wrap.";
			char ch ='/';
			strncat(path,&ch,1);
			
			if(dr==NULL)
			{
				perror("Error: ");
				exit(EXIT_FAILURE);
			}

			while((de=readdir(dr))!=NULL)
			{
				if(!strcmp(de->d_name,".") || !strcmp(de->d_name,"..")) 
				{

				}
				else
				{
					if((strstr(de->d_name,wraptest) != NULL) || de->d_name[0] == '.') {
						continue;
					}

					char newpath[PATH_MAX];
					strcpy(newpath,path);
					strcat(newpath,de->d_name);
					wrapfile(newpath,2,width);
				}
			}
			
			closedir(dr);
		}

	}
	
	return 0;
}

int main(int argc,char *argv[]) 
{
	int type=0;
	
	int width=atoi(argv[1]);
	
	if(argv[1]<=0) {
		perror("Error: width is less than or equal to 0\n");
		exit(EXIT_FAILURE);
	}
	if(argv[2] == NULL) {
		wrapfile(0,0,width);
	}

	if(is_file(argv[2]) != 0) {
		type=1; //Used to define which part of ww either make a new file or just return wrapped text
		wrapfile(argv[2],type,width); //function used to wrap the text in a file 
	}
	else if(is_dir(argv[2]) != 0) {
		type=2;
		wrapfile(argv[2],type,width);
	}
	else {
		perror("Error: ");
		exit(EXIT_FAILURE);
	}
	
	exit(EXIT_SUCCESS);
	
}
	
	
	
