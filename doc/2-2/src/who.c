#define __LIBRARY__
#include "unistd.h"
#include "asm/segment.h"
#include "errno.h"
#define MAX_SIZE 23

char user_name[MAX_SIZE+1];

int sys_iam(const char *name)
{
    int i;
    for(i=0; get_fs_byte(name+i)&&i<=MAX_SIZE; i++);
    if(i>MAX_SIZE)
    {
	//错误做法 return errno=EINVAL,-1
	//EINVAL是个宏定义,置errno并返回-1下同
        return -EINVAL;
    }
    for(i=0; user_name[i]=get_fs_byte(name+i); i++);
    return i;
}

int sys_whoami(char *name, unsigned int size)
{
    int i;
    //需要复制'\0'在判断中进行
    for(i=0; put_fs_byte(user_name[i], name+i), user_name[i]&&i<=MAX_SIZE; i++);
    return i>size? -EINVAL: i;
}

