#include <unistd.h>
#include <errno.h>
#include <asm/segment.h>
#include <asm/system.h>
#include <linux/kernel.h>

//只用了三个同步量
static sem_t sem_array[3]={{{'\0'},0,NULL},{{'\0'},0,NULL},{{'\0'},0,NULL}};

sem_t *sys_sem_open(const char *name, unsigned int value)
{
	sem_t*	ans=NULL;
	char	tmp_name[64];
	int i=0;
	//复制最后的\0
	for(i=0;tmp_name[i]=get_fs_byte(name+i);i++);
	for(i=0;i<3;i++)
	{
		if(!strcmp(tmp_name,sem_array[i].name))
		{
			ans=sem_array+i;
			#ifdef _DEBUG_
			printk("%s,%s,%d\r\n",sem_array[i].name,"reopen",sem_array[i].value);
			#endif
			return ans;
		}
	}
	for(i=0;i<3;i++)
	{
		if(sem_array[i].name[0]=='\0')
		{
			strcpy(sem_array[i].name,tmp_name);
			sem_array[i].value=value;
			sem_array[i].block_queue=NULL;
			ans=sem_array+i;
			#ifdef _DEBUG_
			printk("%s,%s,%d\r\n",sem_array[i].name,"open",sem_array[i].value);
			#endif
			return ans;
		}
	}
	#ifdef _DEBUG_
	printk("error open\r\n");
	#endif
	return ans;

}
int sys_sem_wait(sem_t *sem)
{
	int ans=0;
	cli();
	if(sem<sem_array||sem_array+2<sem||sem->name[0]=='\0')
	{
		#ifdef _DEBUG_
		printk("%p %p %p %s wait error\r\n",sem_array,sem,sem_array+2,sem->name);
		#endif
		sti();
		return -1;
	}	
	//挂载到队列直到sem->value>=0
	while(sem->value<=0)
		sleep_on(&(sem->block_queue));
	--(sem->value);
	sti();
	#ifdef _DEBUG_
	printk("%s,%s,%d\r\n",sem->name,"wait",sem->value);
	#endif
	return ans;
}
int sys_sem_post(sem_t *sem)
{
	int ans=0;
	cli();
	if(sem<sem_array||sem_array+2<sem||sem->name[0]=='\0')
	{
		#ifdef _DEBUG_
		printk("%p %p %p %s post error\r\n",sem_array,sem,sem_array+2,sem->name);
		#endif
		sti();
		return -1;
	}
	++(sem->value);
	//不用while 只要等待队列还有唤醒让其继续运行就可
	wake_up(&(sem->block_queue));
	sti();
	#ifdef _DEBUG_
	printk("%s,%s,%d\r\n",sem->name,"post",sem->value);
	#endif
	return ans;
}
int sys_sem_unlink(const char *name)
{
	char 	tmp_name[64];
	int i=0;
	for(i=0;tmp_name[i]=get_fs_byte(name+i);i++);
	for(i=0;i<3;i++)
		if(!strcmp(name,sem_array[i].name))
		{
			sem_array[i].name[0]='\0';
			#ifdef _DEBUG_
			printk("%s,%s,%d\r\n",sem_array[i].name,"unlink",sem_array[i].value);
			#endif
			return 0;
		}
	#ifdef _DEBUG_
	printk("unlink error\r\n");
	#endif
	
	return -1;
}
