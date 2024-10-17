CC = gcc
CFLAGS = -g -O2 -Wall -Wextra 

SRC = exp1.c	
TARGET = $(patsubst %.c, %, $(SRC))

all: $(TARGET)

$(TARGET): %: %.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean: 
	rm $(TARGET)
