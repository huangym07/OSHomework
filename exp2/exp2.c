#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <assert.h>

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
	printf("序号       进程名      进程号         到达时间     运行时间    优先级       开始运行时间   运行结束时间 运行次序    周转时间  带权周转时间\n");

	double turnaround_time = 0, weighted_turnaround_time = 0;
	for(int i = 0; i < process_num; i++) {
		printf("%-12d %-12s %-12d %-12lf %-12lf %-12d %-12lf %-12lf %-12d %-12lf %-12lf\n", i + 1, process[i].name, process[i].pid, process[i].arrive_time, process[i].run_time, process[i].priority, process[i].start_time, process[i].end_time, process[i].run_order, process[i].turnaround_time, process[i].weighted_turnaround_time);
		
		turnaround_time += process[i].turnaround_time;
		weighted_turnaround_time += process[i].weighted_turnaround_time;
	}

	printf("平均周转时间：%lf\n", turnaround_time / process_num);
	printf("平均带权周转时间：%lf\n", weighted_turnaround_time / process_num);
}

void update_process(Process *pro, int *order, double *clk) {
	pro->run_order = (*order)++;	
	pro->start_time = *clk < pro->arrive_time ? pro->arrive_time : *clk;
	pro->end_time = pro->start_time + pro->run_time;
	*clk = pro->end_time;
	pro->turnaround_time = pro->end_time - pro->arrive_time;
	pro->weighted_turnaround_time = pro->turnaround_time / pro->run_time;

	pro->state = RUNNING;
}

// 清除进程的非输入的信息
void reset_process(Process *pro) {
		pro->state = READY;
		pro->start_time = pro->end_time = pro->turnaround_time = pro->weighted_turnaround_time = 0;
		pro->run_order = 0; 
}

// 从 pros 地址处开始，清除 num 个进程的非输入的信息
void reset_all_process(Process *pros, int num) {
	for(int i = 0; i < num; i++) {
		reset_process(pros + i);
	}
}

// 从 pros 处开始的 num 个进程中，找到准备就绪的最先到达的进程
int find_first_arrive(Process *pros, int num) {
	int index = -1;
	for(int i = 0; i < num; i++) {
		Process *cur = pros + i;
		if(cur->state == READY) {
			if(index == -1) {
				index = i;
				continue;
			} 

			Process *pro = pros + index;
			if(cur->arrive_time < pro->arrive_time)
				index = i;
		}
	}

	return index;
}

void sche1() {
	double clk = -1; // 上一个进程结束运行时间
	int ok = 1; // 是否全部运行结束
	int index = -1, order = 1;

	int count = 0;
	while(1) {
		if(index >= 0) process[index].state = FINISH;

		index = -1; ok = 1;
		for(int i = 0; i < process_num; i++) {
			Process *cur = &process[i];
			if(cur->state == READY) {
				ok = 0;

				if(index == -1) {
					index = i;
					continue;
				} 

				Process *pro = &process[index];
				if(cur->arrive_time < pro->arrive_time)
					index = i;
			}
		}

		assert(++count <= 5);

		if(ok) break;

		update_process(&process[index], &order, &clk);
	}
}

void sche2() {
	double clk = -1; // 上一个进程结束运行时间
	int ok = 1; // 是否全部运行结束
	int index = -1, order = 1; 
	
	int count = 0;
	while(1) {
		if(index >= 0) process[index].state = FINISH;

		index = -1, ok = 1;
		int have = 0; // 是否有已到达，未运行的进程
		/* 上一个进程结束时，是否有未运行的已到达的进程 
		 * 如果有，在其中找到优先级最高的，即优先数最小的
		 * 如果没有，在未到达的进程中，找到到达时间最早的
		 * */
		for(int i = 0; i < process_num; i++) {
			Process *cur = &process[i];
			if(cur->state == READY) {
				ok = 0;
				if(process[i].arrive_time <= clk) have = 1;
			}
		}

		assert(++count <= 5);

		if(ok) break;

		if(have) {
			for(int i = 0; i < process_num; i++) {
				Process *cur = &process[i];
				if(cur->state == READY && cur->arrive_time <= clk) {
					if(index == -1) {
						index = i;
						continue;
					}

					Process *pro = &process[index];
					if(cur->priority < pro->priority)
						index = i;
				}
			}
		} else {
			if((index = find_first_arrive(process, process_num)) == -1) 
				break;
		}

		update_process(&process[index], &order, &clk);
	}
}

void sche3() {
	double clk = -1; // 上一个进程结束运行时间
	int ok = 1; // 是否全部运行结束
	int index = -1, order = 1;

	int count = 0;
	while(1) {
		if(index >= 0) process[index].state = FINISH;

		index = -1, ok = 1;
		int have = 0; // 是否有已到达，未运行的进程
		/* 上一个进程结束时，是否有未运行的已到达的进程 
			* 如果有，在其中找到运行时间最短的
			* 如果没有，在未到达的进程中，找到到达时间最早的
			* */
		for(int i = 0; i < process_num; i++) {
			Process *pro = &process[i];
			if(pro->state == READY) {
				ok = 0;
				if(pro->arrive_time <= clk) have = 1;
			}
		}

		assert(++count <= 5);

		if(ok) break;

		if(have) {
			for(int i = 0; i < process_num; i++) {
				Process *cur = &process[i];
				if(cur->state == READY && cur->arrive_time <= clk) {
					if(index == -1) {
						index = i;
						continue;
					} 

					Process *pro = &process[index];
					if(cur->run_time < pro->run_time) 
						index = i;
				}
			}
		} else {
			if((index = find_first_arrive(process, process_num)) == -1)
				break;
		}

		update_process(&process[index], &order, &clk);
	}
}

schedule sches[] = {
	[0]		sche1,
	[1]		sche2,
	[2]		sche3,
};

int main(int argc, char *argv[]) {
	input_process();

	while(1) {
		reset_all_process(process, process_num);

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
