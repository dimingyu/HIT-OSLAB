#include <stdarg.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <asm/segment.h>
char proc_buf[4096];
int sprintf(char *buf, const char *fmt, ...)
{
    va_list args; int i;
    va_start(args, fmt);
    i=vsprintf(buf, fmt, args);
    va_end(args);
    return i;
}
int get_psinfo()
{
	int ans=0;
	ans+=sprintf(proc_buf+ans,"%s","PID\tPPID\tS\tPRI\tTTY\tTIME\n");
	struct task_struct **p=NULL;
	for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
	{
		if (*p)
		{
			ans+=sprintf(proc_buf+ans, "%d\t", (*p)->pid);
			ans+=sprintf(proc_buf+ans, "%d\t", (*p)->father);
			ans+=sprintf(proc_buf+ans, "%d\t", (*p)->state);
			ans+=sprintf(proc_buf+ans, "%d\t", (*p)->priority);
			ans+=sprintf(proc_buf+ans, "%d\t", (*p)->tty);
			ans+=sprintf(proc_buf+ans, "%d\n", (*p)->cutime+(*p)->cstime);
		}
	}
	return ans;
}
int get_hdinfo()
{
	struct super_block * sb;
	unsigned int ans=0;
	unsigned int i,j;
	unsigned int blockused=0;
	unsigned char tmp;
	sb = get_super(current->root->i_dev);
	for(i=0;i<sb->s_zmap_blocks;i++)
		for(j=0;j<1024;j++)
			for(tmp=sb->s_zmap[i]->b_data[j];tmp;tmp>>=1)
				blockused+=1&tmp;

	ans+=sprintf(proc_buf+ans,"total_blocks :%u\n", sb->s_nzones);
	ans+=sprintf(proc_buf+ans,"Free blocks  :%d\n", sb->s_nzones-blockused);
	ans+=sprintf(proc_buf+ans,"Total inodes :%u\n", sb->s_ninodes);
	return ans;
}
int get_inodeinfo()
{
	int ans=0;
	struct super_block * sb;
	sb = get_super(current->root->i_dev);
	ans += sprintf(proc_buf+ans,"total counr:%d\n",sb->s_ninodes);
	return ans;
}
int proc_read(int dev, unsigned long * pos, char * buf, int count)
{
	int i;
	int ans;
	if(0 == *pos)
		if(dev==0)ans=get_psinfo();
		else if(dev==1)ans=get_hdinfo();
		else if(dev==2)ans=get_inodeinfo();
		else return 0;

	for(i=0;i<count;i++,(*pos)++)
		if(*pos>ans)return i;
		else put_fs_byte(proc_buf[*pos], &buf[i]);
	return i;
}
