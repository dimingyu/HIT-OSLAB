#define __LIBRARY__
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>
#define N 3
#define M 500
#define BUFSIZE 10

_syscall2(sem_t *, sem_open, const char*,name,unsigned int,value);
_syscall1(int,sem_wait,sem_t *,sem);
_syscall1(int,sem_post,sem_t *,sem);
_syscall1(int,sem_unlink,const char *,name);

int Producer();
int Consumer();

sem_t *empty=NULL,*mutex=NULL,*full=NULL;
char strbuf[32];
int fd=0;

int main()
{
	int i=0;
	pid_t p;
	freopen("out.txt","w",stdout);
	empty=sem_open("empty",BUFSIZE);
	mutex=sem_open("mutex",1);
	full=sem_open("full",0);
	
	fd = open("./pc.data",O_RDWR|O_CREAT|O_TRUNC,0777);
        lseek(fd,M*sizeof(int),SEEK_SET);
	write(fd,(char*)&i,sizeof(int));
	close(fd);

	if(!fork())Producer();
	for(i=0;i<N;i++)
	{
		if(!fork())
			{
				Consumer();
			}
	}

	wait(&i);
	return 0;
}
int Producer()
{
	int data=1;
	int loc=0;
	do
	{
		sem_wait(empty);
		sem_wait(mutex);

		fd = open("./pc.data",O_RDWR,0777);
	        lseek(fd,loc*sizeof(int),SEEK_SET);
		write(fd,(char*)&data,sizeof(int));
		close(fd);
#ifdef _MYDEBUG_
		sprintf(strbuf,"#locw%ddataw%d#\r\n",loc,data);
		write(1,strbuf,strlen(strbuf));
#endif
		loc++;
		data++;

		sem_post(mutex);
		sem_post(full);
	}while(data<=M);
	exit(0);
}

int Consumer()
{
	int data=0;
	int loc=0;
	int space=0;
	while(data<M)
	{
		sem_wait(full);
		sem_wait(mutex);		
		
		fd = open("./pc.data",O_RDWR,0777);
	        lseek(fd,M*sizeof(int),SEEK_SET);
		read(fd,(char*)&loc,sizeof(int));
#ifdef _MYDEBUG_
		sprintf(strbuf,"#locr%d#\r\n",loc);
		write(1,strbuf,strlen(strbuf));
#endif
		if(loc>=M)
		{
			sem_post(mutex);
			sem_post(empty);

			sem_wait(empty);
			sem_post(full);
			exit(0);
		}

		lseek(fd,loc*sizeof(int),SEEK_SET);
		read(fd,(char*)&data,sizeof(int));

		sprintf(strbuf,"pid: %d\tdata:%d\r\n",getpid(),data);
		write(1,strbuf,strlen(strbuf));
	
		lseek(fd,loc*sizeof(int),SEEK_SET);
		write(fd,(char*)&space,sizeof(int));

		loc++;

		lseek(fd,M*sizeof(int),SEEK_SET);
		write(fd,(char*)&loc,sizeof(int));
#ifdef _MYDEBUG_
		sprintf(strbuf,"#locw%d#\r\n",loc);
		write(1,strbuf,strlen(strbuf));
#endif

		close(fd);

		sem_post(mutex);
		sem_post(empty);
	}
	exit(0);

}
