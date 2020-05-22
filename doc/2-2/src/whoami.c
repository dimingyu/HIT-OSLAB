#define __LIBRARY__        
#include "unistd.h" 
#include "stdio.h"
_syscall2(int, whoami,char*,name,unsigned int,size);  
int main(int argc, char** argv)
{
	char buf[32];
	return whoami(buf,24)>0?puts(buf),0:-1;
}
