#include<stdio.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
static int realtime, virttime, proctime;
//0 ITIMER_REAL 1 ITIMER_VIRTUAL 2 ITIMER_PROF
int tottime = 100;
void rwaltimer_set()
{
	struct itimerval itv, oldtv;
	itv.it_interval.tv_sec = tottime;
	itv.it_interval.tv_usec = 0;
	itv.it_value.tv_sec = tottime;
	itv.it_value.tv_usec = 0;

	setitimer(ITIMER_REAL, &itv, &oldtv);
	setitimer(ITIMER_VIRTUAL, &itv, &oldtv);
	setitimer(ITIMER_PROF, &itv, &oldtv);
}
void sigalrm_handler(int sig)
{
	fflush(stdout);
	switch (sig)
	{
	case SIGALRM:realtime++; break;
	case SIGVTALRM:virttime++; break;
	case SIGPROF:proctime++; break;
	default:
		break;
	}
	fflush(stdout);
}
int main(int argc, char* argv[])
{
	int pid = 0;
	signal(SIGALRM, sigalrm_handler);
	signal(SIGVTALRM, sigalrm_handler);
	signal(SIGPROF, sigalrm_handler);
	rwaltimer_set();
	for (int i = 2; i % (2 << 30 - 1); i++);
	for (int i = 0; i < 1000; i++)
		if (!fork())
			exit(0);
	sleep(2);
	printf("%16s%16s%16s%16s\n","realtime","cpu time","user time","kernel time");
	struct itimerval t1, t2, t3;
	getitimer(ITIMER_REAL, &t1);
	getitimer(ITIMER_VIRTUAL, &t2);
	getitimer(ITIMER_PROF, &t3);
	double ans1 = t1.it_value.tv_sec * 1000 + t1.it_value.tv_usec / 1000.0;
	ans1 = tottime * 1000 - ans1;
	double ans2 = t2.it_value.tv_sec * 1000 + t2.it_value.tv_usec / 1000.0;
	ans2 = tottime * 1000 - ans2;
	double ans3 = t3.it_value.tv_sec * 1000 + t3.it_value.tv_usec / 1000.0;
	ans3 = tottime * 1000 - ans3;
	printf("%13.4fms %13.4fms %13.4fms %13.4fms\n",ans1,ans3,ans2,ans3-ans2);
	return 0;
}
