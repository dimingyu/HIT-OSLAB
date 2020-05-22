#include<stdio.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<semaphore.h>
#include<stdlib.h>
#include<fcntl.h>
#define M 500
#define BUFSIZE 10


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
	int *pendflag=NULL;
	int fullvalue;
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
	shmid=shmget(1,(BUFSIZE)*sizeof(int),IPC_CREAT|0777);
	shmcntid=shmget(2,sizeof(int),IPC_CREAT|0777);
#ifdef _DEBUG_
	if(-1==shmid||-1==shmcntid)
		perror("consumer shmget fail\r\n");
	else
		perror("consumer shmget sucess\r\n");
#endif
	p=(int*)shmat(shmid,NULL,0);
	pcnt=(int*)shmat(shmcntid,NULL,0);
	while(*pcnt<M)
	{
		sem_wait(full);
		if(*pcnt>=M)continue;
		sem_wait(mutex);
		fflush(stdout);
#if defined _DEBUG_
		sem_getvalue(full,&fullvalue);
		sprintf(strbuf,"p:%d d:%d@%d r:%d\r\n",getpid(),p[*pcnt],*pcnt,fullvalue);
#else

		sprintf(strbuf,"pid: %d\tdata:%d",getpid(),p[*pcnt]);
		puts(strbuf);
		
#endif
		
		
		
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
	if(*pcnt>=M)sem_post(full);
	return 0;
}
