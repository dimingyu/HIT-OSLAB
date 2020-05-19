#include<linux/mm.h>
#include<linux/sched.h>
#include<unistd.h>
#define SHM_TABLE_SIZE 16

struct shm_table
{
	unsigned int key;
	unsigned int size;
	int used;
	unsigned long p_address;
}shmt[SHM_TABLE_SIZE]={};


int sys_shmget(unsigned int key, unsigned int size)
{
	int i;
	if(size>PAGE_SIZE)return -1;

	for(i=0;i<SHM_TABLE_SIZE;i++)
		if(shmt[i].used&&key==shmt[i].key)
			return i;

	for(i=0;i<SHM_TABLE_SIZE;i++)
		if(!shmt[i].used)
			{
				shmt[i].key=key;
				shmt[i].size=size;
				shmt[i].used=1;
				shmt[i].p_address=get_free_page();
				return i;
			}
	return -1;
}
void* sys_shmat(int shmid)
{
	if(!shmt[shmid].used||shmid<0||shmid>SHM_TABLE_SIZE)return -1;
	current->brk += PAGE_SIZE;
	put_shm_page(shmt[shmid].p_address, get_base(current->ldt[2])+current->brk);
	current->brk+=+PAGE_SIZE;
	return (void *)(current->brk - PAGE_SIZE);
}
