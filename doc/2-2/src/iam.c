#define __LIBRARY__        
#include "unistd.h" 
#include "stdio.h"
_syscall1(int, iam, const char*, name);
int main(int argc, char** argv)
{
	return argc==2&&iam(argv[1])>0?0:-1;
}
