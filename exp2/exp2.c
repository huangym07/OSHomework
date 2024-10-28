#include <stdio.h>
#include <stdlib.h>
#include <process.h>

#ifdef DEBUG
#define debug(M, ...) fprintf(stderr, "[DEBUG]: file %s, line %d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

int process_num;

void input_process() {
#ifdef DEBUG
	if(NULL == freopen("test.txt", "r", stdin)) { printf("freopen failed, exit\n"); exit(-1); }
#endif

	printf("请输入进程个数：");
	if(scanf("%d", &process_num) <= 0) { printf("scanf failed, exit\n"); exit(-1); }

	for(int i = 0; i < process_num; i++) {
		printf("请输入进程名: ");
		if(scanf("%s", process[i].name) <= 0) { printf("scanf failed, exit\n"); exit(-1); }

		printf("请输入进程号：");
		if(scanf("%d", &process[i].pid) <= 0) { printf("scanf failed, exit\n"); exit(-1); }

		printf("请输入进程到达时间：");
		if(scanf("%lf", &process[i].arrive_time) <= 0) { printf("scanf failed, exit\n"); exit(-1); }

		printf("请输入进程运行时间：");
		if(scanf("%lf", &process[i].run_time) <= 0) { printf("scanf failed, exit\n"); exit(-1); }

		printf("请输入进程的优先级：");
		if(scanf("%d", &process[i].priority) <= 0) { printf("scanf failed, exit\n"); exit(-1); }
	}

#ifdef DEBUG
	if(NULL == freopen("/dev/tty", "r", stdin)) { printf("freopen failed, exit\n"); exit(-1); }
#endif
}

void output_process() {
	printf("序号       进程名      进程号         到达时间    优先级       开始运行时间   运行结束时间 运行次序    周转时间  带权周转时间\n");

	double turnaround_time = 0, weighted_turnaround_time = 0;
	for(int i = 0; i < process_num; i++) {
		printf("%-12d %-12s %-12d %-12lf %-12d %-12lf %-12lf %-12d %-12lf %-12lf\n", i + 1, process[i].name, process[i].pid, process[i].arrive_time, process[i].priority, process[i].start_time, process[i].end_time, process[i].run_order, process[i].turnaround_time, process[i].weighted_turnaround_time);
		
		turnaround_time += process[i].turnaround_time;
		weighted_turnaround_time += process[i].weighted_turnaround_time;
	}

	printf("平均周转时间：%lf\n", turnaround_time / process_num);
	printf("平均带权周转时间：%lf\n", weighted_turnaround_time / process_num);
}

void sche1() {
	double clk = -1; // 上一个进程结束运行时间
	int ok = 1; // 是否仍有准备就绪的进程
	int index = -1, order = 1;

	int count = 0;
	while(1) {
		if(index >= 0) process[index].state = FINISH;

		index = 0; ok = 1;
		for(int i = 0; i < process_num; i++) {
			if(process[i].state == READY) {
				ok = 0;

				if(process[index].state != READY || process[i].arrive_time < process[index].arrive_time) {
					index = i;
				}
			}
		}

		debug("count is %d, ok is %d", ++count, ok);
		debug("index is %d", index);

		if(ok) break;

		process[index].run_order = order++;
		process[index].start_time = clk < process[index].arrive_time ? process[index].arrive_time : clk;
		process[index].end_time = process[index].start_time + process[index].run_time;
		clk = process[index].end_time;
		process[index].turnaround_time = process[index].end_time - process[index].arrive_time;
		process[index].weighted_turnaround_time = process[index].turnaround_time / process[index].run_time;

		process[index].state = RUNNING;
	}
}

int main(int argc, char *argv[]) {
	sches[0] = sche1;

	input_process();
	while(1) {
		printf("\n0 ———— 退出程序\n");
		printf("\n1 ———— 先来先服务调度\n");
		printf("\n2 ———— 优先级调度\n");
		printf("\n3 ———— 短作业调度\n");
		printf("\n4 ———— 响应比高调度\n");
		printf("\n请从中选择一项：");

		int op;
		if(scanf("%d", &op) <= 0) { printf("scanf failed, exit\n"); exit(-1); }
		if(op == 0) {
			break;
		} else if (op >= 5) {
			printf("您输入的选项有误，请输入 0、1、2、3、4 中的一个\n");
		} else {
			sches[op - 1]();
		}

		output_process();
	}

	return 0;
}
