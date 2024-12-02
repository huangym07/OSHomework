CC = gcc
CFLAGS = -g -O2 -Wall -Wextra 

SRC = exp1.c	
TARGET = $(patsubst %.c, %, $(SRC))

all: $(TARGET)

run: $(TARGET)
	./$<

$(TARGET): %: %.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean run

clean: 
	rm $(TARGET)
