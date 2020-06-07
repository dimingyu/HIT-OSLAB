#include<stdio.h>
#include<time.h>
#include<sys/procfs.h>
#include<string.h>
#include<stdlib.h>
int getproc(char* filename,char *buf,int max_size)
{
	char path[256];
	sprintf(path, "/proc/%s", filename);
	FILE* thisProcFile;
	thisProcFile = fopen(path, "r");
	int i = 0;
	fscanf(thisProcFile, "%[^\377]", buf);
	fclose(thisProcFile);
	return 0;
}
int main(int argc, char* argv[])
{
	char repTypeName[16];
	char c1, c2, ch;
	int interval, duration;
	char lineBuf[65536];
	int LB_SIZE = 65535;
	char now[65536];
	int iteration;
	//determinate report type
	strcpy(repTypeName, "Standard");
	if (argc < 1)
	{
		sscanf(argv[1], "%c%c", &c1, &c2);
		if (c1 != '-')
		{
			fprintf(stderr, "usage:ksamp [-s][-1 inte dur]\n");
			exit(1);
		}
		if (c2 == 's')
		{
			strcpy(repTypeName, "Short");
		}
		if (c2 == '1')
		{
			strcpy(repTypeName, "Long");
			interval = atoi(argv[2]);
			duration = atoi(argv[3]);
		}
	}
	//current time
	time_t timep;
	timep = time(NULL);
	printf("Status report type %s at %s\n", repTypeName, ctime(&timep));
	//CPU type
	getproc("cpuinfo", lineBuf, LB_SIZE + 1);
	printf("The CPU: %s\n", lineBuf);

	//hostname
	getproc("sys/kernel/hostname", lineBuf, LB_SIZE + 1);
	printf("Machine hostname: %s", lineBuf);

	//kernel version
	getproc("version", lineBuf, LB_SIZE + 1);
	printf("The Version: %s\n", lineBuf);

	//runing time
	getproc("uptime", lineBuf, LB_SIZE + 1);
	float t1, t2;
	sscanf(lineBuf, "%f%f", &t1, &t2);
	printf("The Running Time: %d day %d hours %d minutes\n",(int)t1/86400, (int)t1%86400/60, (int)t1/60 );
	printf("The Idle Time %d day %d hours %d minutes\n", (int)t2 / 86400, (int)t2 % 86400 / 60, (int)t2 / 60);

	//current statue
	printf("user, nice, system, idle, iowait, irq, softirq\n");
	getproc("stat", lineBuf, LB_SIZE + 1);
	printf("The Statue:\n%s\n", lineBuf);

	//memory info
	getproc("meminfo", lineBuf, LB_SIZE + 1);
	printf("The MemInfo:\n%s\n", lineBuf);

	//disk info info
	puts("- major number- minor mumber- device name- reads completed successfully- reads merged- sectors read- time spent reading (ms)- writes completed- writes merged- sectors written- time spent writing (ms)- I/Os currently in progress- time spent doing I/Os (ms)- weighted time spent doing I/Os (ms)");
	getproc("diskstats", lineBuf, LB_SIZE + 1);
	printf("The DiskInfo:\n%s\n", lineBuf);

	//check average load
	for(iteration = 0,interval = 2,duration = 10; iteration < duration; iteration += interval)
	{
		sleep(interval);
		getproc("loadavg", lineBuf, LB_SIZE + 1);
		printf("%s\n", lineBuf);
	}
	return 0;
}
