#include <stdarg.h>

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
	for(p = &LAST_TASK ; p > &FIRST_TASK ; --p)
if (*p)
   (*p)->counter = ((*p)->counter >> 1)+...;
}
int get_hdinfo()
{
	
}
int get inodeinfo()
{
	
}
int proc_read(int dev, unsigned long * pos, char * buf, int count)
{
	int i;
	if(0 == *pos % 2048)
		if(dev==0)get_psinfo();
		else if(dev==1)get_hdinfo();
		else if(dev==2)get_inodeinfo();
		else return -1;

	for(i=0;i<count;i++)
		if(!proc_buf[i+*pos])return i;
		else put_fs_byte(proc[i + *pos], buf + i + *pos);
}
