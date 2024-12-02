编译链接：make
编译链接运行：make run

test.txt 中包含着实验数据

默认情况下，编译链接时定义了 DEBUG 宏，会从 test.txt 中读取数据
如果想要从终端手动输入数据，在 Makefile 中去掉 -DDEBUG 宏定义编译选项即可

