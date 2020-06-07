#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>>
#include<signal.h>
int num = 0;
void rwaltimer_set()
{
	struct itimerval itv, oldtv;
	itv.it_interval.tv_sec = 1;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = 1;
	itv.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL,&itv, &oldtv);
}
void sigalrm_handler(int sig)
{
	num++;
}
int main(int argc, char* argv[])
{
	signal(SIGALRM, sigalrm_handler);
	rwaltimer_set();
	puts("you have only 10s:");
	while (num < 10);
	puts("now time is up!");
	return 0;
}
