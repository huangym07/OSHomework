#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS_NUMBER 10
#define MAX_PROCESS_NAME 30

typedef struct Process {
	int pid; // 进程号
	char name[MAX_PROCESS_NAME]; // 进程名
	double arrive_time; // 到达时间
	double run_time; // 运行时间
	int priority; // 优先级（越大，优先级越高）
	int run_order; // 运行次序
} Process;

Process process[MAX_PROCESS_NUMBER];

#endif
