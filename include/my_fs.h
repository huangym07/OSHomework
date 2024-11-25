/* My file system */

#ifndef __MY_FS_H__
#define __MY_FS_H__

#define MAX_USER 10
#define MAX_USER_FILE 10
#define MAX_USER_OPEN_FILE 6

#define MAX_FILE MAX_USER * MAX_USER_FILE

struct MFD {
	char uname[20];			// 用户名
	int uaddr;					// 用户文件目录地址		
} MFD[MAX_USER];

struct UFD {
	char uname[20];			// 文件所属用户名
	char fname[20];			// 文件名
	char fattr[10];			// 文件属性
	int len;						// 文件长度 
	int faddr;					// 文件首地址（指磁盘块号）
} UFD[MAX_FILE];

struct UOF {
	char fname[20];			// 文件名
	char fattr[10];			// 文件属性
	int len;						// 文件长度
	int fstatue;				// 文件状态
	int readp, writep;	// 文件读写指针（指向磁盘块号）
} UOF[MAX_USER_OPEN_FILE];

#define DISK_SIZE 512

int bitmap[DISK_SIZE]; // free: 0
											 // allocated: 1

int disk_rest;

/* File system function */
int fs_create();
int fs_open();
int fs_close();
int fs_read();
int fs_write();
int fs_delete();
int fs_list();

typedef int (* fs_operation_pointer)();

#endif
