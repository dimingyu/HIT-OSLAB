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
	int shmid=0;
	int shmcntid;
	int endflag;
	int *p=NULL;
	int *pcnt=NULL;

#ifndef _DEBUG_
	freopen("./out.txt","a",stdout);
#endif 
	empty=sem_open("empty",0);
	mutex=sem_open("mutex",0);
	full=sem_open("full",0);
#ifdef _DEBUG_
	if(!(empty&&mutex&&full))
		perror("consumer open sem fail\r\n");
	else
		perror("consumer open sem sucess\r\n");
#endif
	shmid=shmget(1,(BUFSIZE)*sizeof(int));
	shmcntid=shmget(2,sizeof(int));
#ifdef _DEBUG_
	if(-1==shmid||-1==shmcntid)
		perror("consumer shmget fail\r\n");
	else
		perror("consumer shmget sucess\r\n");
#endif
	p=(int*)shmat(shmid);
	pcnt=(int*)shmat(shmcntid);
	while(*pcnt<M)
	{
		sem_wait(full);
		sem_wait(mutex);
		fflush(stdout);
#if defined _DEBUG_
		sprintf(strbuf,"p:%d d:%d@%d\r\n",getpid(),p[*pcnt],*pcnt);
#else
		sprintf(strbuf,"pid: %d\tdata:%d\r\n",getpid(),p[*pcnt]);		
#endif		
		write(1,strbuf,strlen(strbuf));
		p[*pcnt]=-1;
		(*pcnt)++;
		fflush(stdout);
		sem_post(mutex);
		sem_post(empty);
	}
#ifdef _DEBUG_
	sprintf(strbuf,"consumer %d end\r\n",getpid());
	write(1,strbuf,strlen(strbuf));
#endif
	if(*pcnt>=M)sem_post(empty);
	return 0;
}
