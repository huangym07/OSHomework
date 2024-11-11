#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <page.h>

#define INSTRUCTION_SIZE 20

#define debug(M, ...) fprintf(stderr, "[DEBUG] %s:%d " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define my_assert(test, message, ...) do { \
	if(!(test)) { \
		fprintf(stderr, "[ERROR] %s:%d " message "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
		goto error; \
	} \
} while(0)

void init() {
#ifdef DEBUG
	// debug("checkpoint1");
	my_assert(freopen("input.txt", "r", stdin) != NULL, "Failed to open input.txt");
	// debug("checkpoint2");
#elif
	printf("请输入作业的全部初始页表数据：\n");
#endif
	for(int i = 0; i < JOB_SIZE; i++) {
		page *jp = job_pages + i;
		if(i < MEM_SIZE) {
			my_assert(scanf("%d%d%d%d%d", &jp->pno, &jp->f, &jp->bno, &jp->mfd, &jp->pos) == 5, "Failed to read page data from input.txt");
			mem_pages[i] = i;
		} else {
			my_assert(scanf("%d%d%d%d", &jp->pno, &jp->f, &jp->mfd, &jp->pos) == 4, "Failed to read page data from input.txt");
			jp->bno = -1;
		}
			// debug("%d page %d %d %d %d %d", i, jp->pno, jp->f, jp->bno, jp->mfd, jp->pos);
	}
	queue_pointer = 0;

	return;

error:
	exit(-1);
}

void manage() {
#ifndef DEBUG
	printf("请逐条输入指令：\n");
#endif

	char *inst = (char*)malloc(INSTRUCTION_SIZE);
	my_assert(inst != NULL, "Failed to memory alloc for instruction");
	int pno, addr;

	int page_fault = 0, inst_count = 0;
	while(scanf("%s%d%d", inst, &pno, &addr) == 3) {
		inst_count++;

		page *jp = job_pages + pno;
		if(!jp->f) {
			// 不在主存中
			page_fault++;

			printf("第 %d 页不在内存\n", jp->pno);
			
			page *out_jp = job_pages + mem_pages[queue_pointer]; // 换出的页	
			printf("第 %d 页为淘汰的页\n", out_jp->pno);
			if(out_jp->mfd) {
				printf("%d 页被修改过，因此把 %d 页调出内存\n", out_jp->pno, out_jp->pno);
			}

			printf("第 %d 页调入内存（%d 页所在位置，即主存块号 %d）\n", jp->pno, out_jp->pno, out_jp->bno);

			mem_pages[queue_pointer] = jp->pno;
			queue_pointer = (queue_pointer + 1) % MEM_SIZE;

			jp->f = 1; jp->bno = out_jp->bno;
			out_jp->f = 0; out_jp->bno = -1; out_jp->mfd = 0;
		}

		int abs_addr = vm(pno, addr);

		if(strcmp(inst, "store") == 0) {
			jp->mfd = 1;
		}

		printf("绝对地址 %d\n", vm(jp->bno, addr));
	}

	free(inst);

	printf("共有 %d 条指令，缺页中断 %d 次，所以缺页中断率为 %f\n", inst_count, page_fault, 1.0 * page_fault / inst_count);
	return;

error:
	free(inst);
	exit(-1);
}

int main() {
	// debug("start--------------------");
	
	init();

	manage();

	return 0;
}
