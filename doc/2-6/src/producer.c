#define __LIBRARY__
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#define N 3
#define M 10
#define BUFSIZE 2
_syscall2(sem_t *, sem_open, const char*,name,unsigned int,value);
_syscall1(int,sem_wait,sem_t *,sem);
_syscall1(int,sem_post,sem_t *,sem);
_syscall1(int,sem_unlink,const char *,name);
_syscall2(int,shmget,unsigned int,key,unsigned int,size);
_syscall1(int,shmat,int,shmid);

sem_t *empty=NULL,*mutex=NULL,*full=NULL;
char strbuf[32];
int main()
{
	int i=0;
	int *p=NULL;
	int *pcnt=NULL;
	int data=1;
	int shmid;
	int shmcntid;
#ifndef _DEBUG_
	freopen("out.txt","w",stdout);
#endif 
	empty=sem_open("empty",BUFSIZE);
	mutex=sem_open("mutex",1);
	full=sem_open("full",0);
#ifdef _DEBUG_
	if(!(empty&&mutex&&full))
		perror("producer open sem fail\r\n");
	else
		perror("producer open sem sucess\r\n");
#endif
	shmid=shmget(1,(BUFSIZE)*sizeof(int));
	shmcntid=shmget(2,sizeof(int));
#ifdef _DEBUG_
	if(-1==shmid||-1==shmcntid)
		perror("producer shmget fail\r\n");
	else
		perror("producer shmget sucess\r\n");
#endif
	p=(int*)shmat(shmid);
	pcnt=(int*)shmat(shmcntid);
	*pcnt=0;
	while(data<=M)
	{
		sem_wait(empty);
		sem_wait(mutex);

#ifdef _DEBUG_
		sprintf(strbuf,"%d put data %d\r\n",getpid(),data);
		write(1,strbuf,strlen(strbuf));
#endif

		p[data-1]=data;
		data++;

		sem_post(mutex);
		sem_post(full);
	}
#ifdef _DEBUG_
	sprintf(strbuf,"producer %d end\r\n",getpid());
	write(1,strbuf,strlen(strbuf));
#endif
	return 0;
}
