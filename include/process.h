#ifndef __PROCESS_H__
#define __PROCESS_H__

#define MAX_PROCESS_NAME 30

typedef int ProcessDataType;

typedef struct Process {
	ProcessDataType id; // 进程序号
	ProcessDataType pid; // 进程号
	char name[MAX_PROCESS_NAME]; // 进程名
	ProcessDataType submit_time, // 提交时间
									need_run_time, // 尚需运行时间
									start_time, // 开始运行时间
									end_time, // 结束运行时间
									actual_run_time, // 实际运行时间
									turnaround_time, // 周转时间
									weighted_turnaround_time; // 带权周转时间
	ProcessDataType priority; // 优先级（越大，优先级越高）
	ProcessDataType run_order; // 运行次序
}
#endif
