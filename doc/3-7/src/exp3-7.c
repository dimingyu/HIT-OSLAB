#define __LIBRARY__        
#include "unistd.h" 
#include "time.h"
#include "stdio.h"
_syscall1(int, mycall,struct tm*,ptm);  
int main(int argc, char** argv)
{
	struct tm time;
	mycall(&time);
	printf("2%03d-%02d-%02d %02d:%02d:%02d\n",
	time.tm_year,
	time.tm_mon,
	time.tm_mday,
	time.tm_hour,
	time.tm_min,
	time.tm_sec);
	return 0;
}
