#ifndef __PAGE_H__
#define __PAGE_H__

typedef struct page {
	int pno,					// page number 页号
			f,						// flag 标志（指示页面是否在内存中） 
			bno,					// block number 主存块号（如果当前页面不在主存，设置为 -1 
			mfd,					// modifyed 修改标志（指示当前页面是否被膝修改） 
			pos;					// position 在磁盘上的位置 
} page;

// 主存块个数
#define MEM_SIZE 4
// 作业页表个数
#define JOB_SIZE 7
// 主存块长度
#define BLOCK_SIZE 1024
// 通过块号与页内地址，计算出绝对地址
#define vm(bno, address_inside_page) ((bno) * BLOCK_SIZE + (address_inside_page))

// 主存中页面的队列
int mem_pages[MEM_SIZE];
int queue_pointer;
// 作业的页面
page job_pages[JOB_SIZE];

#endif
