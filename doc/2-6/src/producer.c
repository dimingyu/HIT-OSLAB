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
	int *p=NULL;
	int *pcnt=NULL;
	int data=1;
	int shmid;
	int shmcntid;
	int fullvalue;
#ifndef _DEBUG_
	freopen("out.txt","w",stdout);
#endif 
	sem_unlink("empty");
	sem_unlink("full");
	sem_unlink("mutex");
	empty=sem_open("empty",O_CREAT|O_EXCL,0666,BUFSIZE);
	mutex=sem_open("mutex",O_CREAT|O_EXCL,0666,1);
	full=sem_open("full",O_CREAT|O_EXCL,0666,0);
#ifdef _DEBUG_
	if(!(empty&&mutex&&full))
		perror("producer open sem fail\r\n");
	else
		perror("producer open sem sucess\r\n");
#endif
	shmid=shmget(1,(BUFSIZE)*sizeof(int),IPC_CREAT|0777);
	shmcntid=shmget(2,sizeof(int),IPC_CREAT|0777);
#ifdef _DEBUG_
	if(-1==shmid||-1==shmcntid)
		perror("producer shmget fail\r\n");
	else
		perror("producer shmget sucess\r\n");
#endif
	p=(int*)shmat(shmid,NULL,0);
	pcnt=(int*)shmat(shmcntid,NULL,0);
	*pcnt=0;
	while(data<=M)
	{
		sem_wait(empty);
		sem_wait(mutex);

#ifdef _DEBUG_
		sem_getvalue(full,&fullvalue);
		sprintf(strbuf,"%d put data %d\r\n",getpid(),data);
		write(1,strbuf,strlen(strbuf));
#endif

		p[data-1]=data;
		data++;

		sem_post(mutex);
		sem_post(full);
#ifdef _DEBUG_
		sem_getvalue(full,&fullvalue);
		sprintf(strbuf,"%d res %d\r\n",getpid(),fullvalue);
		write(1,strbuf,strlen(strbuf));
#endif
	}
#ifdef _DEBUG_
	sprintf(strbuf,"producer %d end\r\n",getpid());
	write(1,strbuf,strlen(strbuf));
#endif
	return 0;
}
