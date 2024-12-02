make run					编译运行
make clean				清除 build/ 目录

input.txt 中是实验数据

默认情况下，定义了 DEBUG 宏，会从 input.txt 中读取实验数据
如果想要从终端手动输入数据，在 Makefile 中将 -DDEBUG 去掉即可
