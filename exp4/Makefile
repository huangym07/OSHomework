default: run

SRC_DIR = $(abspath .)
SRCS = $(shell find $(SRC_DIR) -name "*.c")
INC_PATH = $(abspath ../include)

BUILD_DIR = $(SRC_DIR)/build
OBJ_DIR = $(BUILD_DIR)/obj_dir
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS)))))
BIN = $(BUILD_DIR)/exp4

CC = gcc
CFLAGS = -MMD -O3
CFLAGS += $(addprefix -I, $(INC_PATH))
CFLAGS += -g -DDEBUG

-include $(OBJS:.o=.d)

$(shell mkdir -p $(OBJ_DIR))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

run: $(BIN)
	@echo "-- exp4: 文件系统实验 -------"
	@echo "-- RUN ----------------------"
	$(BIN)
	@echo "-- DONE ---------------------"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: default run clean
