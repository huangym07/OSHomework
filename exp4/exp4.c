#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <my_fs.h>

#define ARRLEN(array) sizeof(array)/sizeof(array[0])

char login_uname[20];
char cmd[50];

struct {
	char cmd_name[20];
	fs_operation_pointer fsop;
} cmd_table[] = {
	{"create",		fs_create},
	{"open",			fs_open},
	// {"close",			fs_close},
	// {"read",			fs_read},
	// {"write",			fs_write},
	// {"delete",		fs_delete},
};

void clear_all() {
	memset(MFD, 0, sizeof(MFD));
	memset(UFD, 0, sizeof(UFD));
	memset(UOF, 0, sizeof(UOF));
	memset(bitmap, 0, sizeof(bitmap));
}

void init_MFD(int index, const char *uname, int uaddr) {
	strcpy(MFD[index].uname, uname);
	MFD[index].uaddr = uaddr;
}

void init_UFD(int index, const char *uname, const char *fname, const char *fattr, int len) {
	strcpy(UFD[index].uname, uname);
	strcpy(UFD[index].fname, fname);
	strcpy(UFD[index].fattr, fattr);
	UFD[index].len = len;
}

void init_UOF(int index, const char *fname, const char *fattr, int len, int fstatue, int readp, int writep) {
	strcpy(UOF[index].fname, fname);
	strcpy(UOF[index].fattr, fattr);
	UOF[index].len = len;
	UOF[index].fstatue = fstatue;
	UOF[index].readp = readp;
	UOF[index].writep = writep;
}

void init() {
	clear_all();

	disk_rest = DISK_SIZE;

	/* Initialize MFD */
	init_MFD(0, "user1", 20);
	init_MFD(1, "user2", 60);
	init_MFD(2, "user3", 100);
	init_MFD(3, "user4", 140);
	init_MFD(4, "user5", 180);
	init_MFD(5, "user6", 220);
	init_MFD(6, "user7", 260);
	init_MFD(7, "user8", 300);
	init_MFD(8, "user9", 340);
	init_MFD(9, "user10", 380);

	/* Initialize UFD */
	init_UFD(0, "user1", "u1f1", "r", 10);
	init_UFD(1, "user1", "u1f2", "r", 20);
	init_UFD(2, "user1", "u1f3", "r", 30);
	init_UFD(3, "user2", "u2f1", "r", 25);
	init_UFD(4, "user2", "u2f2", "r", 35);
	init_UFD(5, "user2", "u2f3", "r", 15);
	init_UFD(6, "user2", "u2f4", "w", 5);
	init_UFD(7, "user5", "u5f1", "w", 40);
	init_UFD(8, "user5", "u5f2", "w", 45);
	/* Caculate file's first address. */
	for(int i = 0; i < 9; i++) {
		UFD[i].faddr = DISK_SIZE - disk_rest;
		disk_rest -= UFD[i].len;
	}
	/* Modify bitmap block. */
	for(int i = 0; i < (DISK_SIZE - disk_rest); i++) bitmap[i] = 1;
		
	/* Initialize UOF */
	/* Nothing to do. */
}

void debug_print() {
	for(int i = 0; i < MAX_USER; i++) {
		if(strlen(MFD[i].uname) == 0) continue;
		printf("%s %d\n", MFD[i].uname, MFD[i].uaddr);
	}
	for(int i = 0; i < MAX_FILE; i++) {
		if(strlen(UFD[i].uname) == 0) continue;
		printf("%s %s %s %d %d\n", UFD[i].uname, UFD[i].fname, UFD[i].fattr, UFD[i].len, UFD[i].faddr);
	}
	for(int i = 0; i < MAX_USER_OPEN_FILE; i++) {
		if(strlen(UOF[i].fname) == 0) continue;
		printf("%s %s %d %d %d %d\n", UOF[i].fname, UOF[i].fattr, UOF[i].len, UOF[i].fstatue, UOF[i].readp, UOF[i].writep);
	}
}

int login() {
	printf("请输入用户名：");

	if(scanf("%s", login_uname) != 1) exit(-1);

	int success = -1;
	for(int i = 0; i < MAX_USER; i++) {
		if(strcmp(login_uname, MFD[i].uname) == 0) {
			success = i;
			break;
		}
	}

	if(success == -1) {
		printf("无此用户\n");
		return -1;
	}

	printf("Login success! Hello %s\n", login_uname);

	return 0;
}

void get_and_parse_cmd() {
	while(1) {
		printf("请输入命令：\n");
		printf("create: 建立文件\nopen: 打开文件\nclose: 关闭文件\nread: 读取文件\nwrite: 写入文件\ndelete: 删除文件\n");
		if(scanf("%s", cmd) != 1) exit(-1);

		if(strcmp(cmd, "end") == 0) {
			printf("程序结束\n");
			break;
		}

		fs_operation_pointer fsop = NULL;
		for(int i = 0; i < ARRLEN(cmd_table); i++) {
			if(strcmp(cmd_table[i].cmd_name, cmd) == 0) {
				fsop = cmd_table[i].fsop;
				break;
			}
		}

		if(fsop == NULL) {
			printf("错误命令，请重新输入\n");
			continue;
		}

		int ret = (*fsop)();	

		if(ret != 0) {
			printf("命令 %s 执行失败\n", cmd);
		}
	}
}

int fs_create() {
	char fname[20], fattr[20];
	int len;
	printf("请输入要创建的文件名，文件长度以及文件属性：\n");
	if(scanf("%s%d%s", fname, &len, fattr) != 3) exit(-1);

	for(int i = 0; i < MAX_FILE; i++) {
		if(strcmp(UFD[i].uname, login_uname) == 0
				&& strcmp(UFD[i].fname, fname) == 0) {
			printf("用户 %s 有同名文件 %s，不能创建\n", login_uname, fname);
			return -1;
		}
	}

	int user_file_count = 0;
	int	index_free_ufd = -1;
	for(int i = 0; i < MAX_FILE; i++) {
		if(index_free_ufd == -1 && strlen(UFD[i].uname) == 0) index_free_ufd = i;
		if(strcmp(UFD[i].uname, login_uname) == 0) {
			user_file_count++;
		}
	}

	if(index_free_ufd == -1 || user_file_count >= MAX_USER_FILE) {
		printf("在 UFD 中无空登记栏，不能建立\n");
		return -1;
	}

	int index_free_uof = -1;
	for(int i = 0; i < MAX_USER_OPEN_FILE; i++) {
		if(strlen(UOF[i].fname) == 0) {
			index_free_uof = i;
			break;
		}
	}

	if(index_free_uof == -1) {
		printf("在 UOF 中无空登记栏，不能创建，请先关闭文件\n");
		return -1;
	}

	if(disk_rest < len) {
		printf("磁盘无空闲块，不能建立\n");
		return -1;
	}

	int free_block_no = DISK_SIZE - disk_rest;	
	disk_rest -= len;
	for(int i = 0; i < len; i++) bitmap[free_block_no + i] = 1;

	init_UFD(index_free_ufd, login_uname, fname, fattr, len);
	UFD[index_free_ufd].faddr = free_block_no;

	init_UOF(index_free_uof, fname, fattr, len, 1, 0, free_block_no);

	printf("建立成功\n");

	return 0;
}

int fs_open() {
	char fname[20], fop[20];
	printf("请输入要打开的文件名与操作类型：\n");	
	if(scanf("%s%s", fname, fop) != 2) exit(-1);

	int index_file_ufd = -1;
	for(int i = 0; i < MAX_FILE; i++) {
		if(strcmp(login_uname, UFD[i].uname) == 0
				&& strcmp(fname, UFD[i].fname) == 0) {
				index_file_ufd = i;
				break;
		}
	}

	if(index_file_ufd == -1) {
		printf("文件不存在，不能打开\n");
	}

	int index_file_uof = -1;
	for(int i = 0; i < MAX_USER_OPEN_FILE; i++) {
		if(strcmp(UOF[i].fname, fname) == 0) {
			index_file_uof = i;
			break;
		}
	}

	if(index_file_uof != -1) {
		if(UOF[index_file_uof].fstatue == 1) {
			printf("正在建立，不能打开\n");
		} else {
			printf("文件已打开\n");
		}
	} else {
		if(strcmp(fop, UFD[index_file_ufd].fattr) == 0) {
			int index_free_uof = -1;
			for(int i = 0; i < MAX_USER_OPEN_FILE; i++) {
				if(strlen(UOF[i].fname) == 0) {
					index_free_uof = i;
					break;
				}
			}

			if(index_free_uof == -1) {
				printf("UOF 无空栏，无法打开文件\n");
				return -1;
			}

			init_UOF(index_free_uof, UFD[index_file_ufd].fname, UFD[index_file_ufd].fattr, UFD[index_file_ufd].len, 0, UFD[index_file_ufd].faddr, UFD[index_file_ufd].faddr);

			printf("打开成功\n");
		} else {
			printf("操作不合法，不能打开\n");
		}
	}

}

int main() {
	init();

	// debug_print();
	
	if(login() != 0) return 0;

	get_and_parse_cmd();

	return 0;
}
