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
		Process *cur = &process[i];
		printf("请依次输入进程名、进程号、到达时间、运行时间、优先级（数字越小，优先级越高），中间请用空格分开：\n");
		if(scanf("%s %d %lf %lf %d", cur->name, &cur->pid, &cur->arrive_time, &cur->run_time, &cur->priority) != 5) {
			printf("input_process: scanf failed, exit\n");
			exit(-1);
		}
	}

#ifdef DEBUG
	if(NULL == freopen("/dev/tty", "r", stdin)) { printf("freopen failed, exit\n"); exit(-1); }
#endif
}

void output_process() {
	printf("序号       进程名      进程号         到达时间     运行时间    优先级       开始运行时间   运行结束时间 运行次序    周转时间  带权周转时间\n");

	double turnaround_time = 0, weighted_turnaround_time = 0;
	for(int i = 0; i < process_num; i++) {
		Process *cur = &process[i];
		printf("%-12d %-12s %-12d %-12lf %-12lf %-12d %-12lf %-12lf %-12d %-12lf %-12lf\n", i + 1, cur->name, cur->pid, cur->arrive_time, cur->run_time, cur->priority, cur->start_time, cur->end_time, cur->run_order, cur->turnaround_time, cur->weighted_turnaround_time);
		
		turnaround_time += cur->turnaround_time;
		weighted_turnaround_time += cur->weighted_turnaround_time;
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

// 从 pros 处开始的 num 个进程中，根据比较函数，找到已到达的，就绪的进程中，最优的进程
int find_best_arrived(Process *pros, int num, process_cmp cmp, double clk) {
	int index = -1;
	
	for(int i = 0; i < num; i++) {
		Process *cur = pros + i;
		if(cur->state == READY && cur->arrive_time <= clk) {
			if(index == -1) {
				index = i;
				continue;
			}

			Process *pro = pros + index;
			if(cmp(cur, pro, clk))
				index = i;
		}
	}

	return index;
}

void sche1() {
	double clk = -1; // 上一个进程结束运行时间
	int index = -1, order = 1;

	while(1) {
		if(index >= 0) process[index].state = FINISH;

		if((index = find_first_arrive(process, process_num)) == -1)
			break;

		update_process(&process[index], &order, &clk);
	}
}

void schedule_process(process_cmp cmp) {
	double clk = -1; // 上一个进程结束运行时间
	int index = -1, order = 1; 
	
	while(1) {
		if(index >= 0) process[index].state = FINISH;

		int have = 0; // 是否有已到达，未运行的进程
		/* 上一个进程结束时，是否有未运行的已到达的进程 
		 * 如果有，在其中根据比较函数找到最优进程
		 * 如果没有，在未到达的进程中，找到到达时间最早的
		 * */
		for(int i = 0; i < process_num; i++) {
			Process *cur = &process[i];
			if(cur->state == READY && cur->arrive_time <= clk)
				have = 1;
		}

		if(have) {
			if((index = find_best_arrived(process, process_num, cmp, clk)) == -1)
				break;
		} else {
			if((index = find_first_arrive(process, process_num)) == -1) 
				break;
		}

		update_process(&process[index], &order, &clk);
	}
}

int sche2_cmp(Process *pro1, Process *pro2, double clk) {
	return pro1->priority < pro2->priority;
}

int sche3_cmp(Process *pro1, Process *pro2, double clk) {
	return pro1->run_time < pro2->run_time;
}

int sche4_cmp(Process *pro1, Process *pro2, double clk) {
	// response time 响应比
	double rps_pro1 = 1 + (clk - pro1->arrive_time) / pro1->run_time;
	double rps_pro2 = 1 + (clk - pro2->arrive_time) / pro2->run_time;

	return rps_pro1 > rps_pro2 || (rps_pro1 == rps_pro2 && pro1->arrive_time < pro2->arrive_time);
}

void sche2() {
	schedule_process(sche2_cmp);
}

void sche3() {
	schedule_process(sche3_cmp);
}

void sche4() {
	schedule_process(sche4_cmp);
}

schedule sches[] = {
	[0]		sche1,
	[1]		sche2,
	[2]		sche3,
	[3]		sche4
};

int main() {
	input_process();

	while(1) {
		reset_all_process(process, process_num);

		printf("\n\nMENU--------------------");
		printf("\n0 ———— 退出程序\n");
		printf("\n1 ———— 先来先服务调度\n");
		printf("\n2 ———— 优先级调度\n");
		printf("\n3 ———— 短作业调度\n");
		printf("\n4 ———— 响应比高调度\n");
		printf("--------------------");
		printf("\n请从中选择一项：");

		int op;
		if(scanf("%d", &op) <= 0) { printf("scanf failed, exit\n"); exit(-1); }
		if(op == 0) {
			break;
		} else if (op >= 5) {
			printf("您输入的选项有误，请输入 0、1、2、3、4 中的一个\n");
			continue;
		} else {
			sches[op - 1]();
		}

		output_process();
	}

	return 0;
}
