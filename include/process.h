#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS_NUMBER 10
#define MAX_PROCESS_NAME 30

enum State {READY = 0, RUNNING, FINISH}; // 准备就绪，运行中，运行结束 

typedef enum State State;

typedef struct Process {
	int pid; // 进程号
	char name[MAX_PROCESS_NAME]; // 进程名
	double arrive_time; // 到达时间
	double run_time; // 运行时间
	int priority; // 优先级（优先数越小，优先级越高）
	
	State state; // 进程状态

	int run_order; // 运行次序
	double start_time; // 开始运行时间
	double end_time; // 运行结束时间
	double turnaround_time; // 周转时间
	double weighted_turnaround_time; // 带权周转时间
} Process;

Process process[MAX_PROCESS_NUMBER];

typedef void(* schedule)();
typedef int(* process_cmp)(Process *, Process *, double);

void input_process();
void output_process();
void update_process(Process *, int *, double *);
void reset_process(Process *);
void reset_all_process(Process *, int);
int find_first_arrive(Process *, int);
int find_best_arrived(Process *, int, process_cmp, double);
void schedule_process(process_cmp);

#endif
