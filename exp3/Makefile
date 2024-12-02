SRC_DIR = $(abspath .)
SRCS = $(shell find $(SRC_DIR) -name "*.c")
INC_PATH = $(abspath ../include)

BUILD_DIR = $(SRC_DIR)/build
OBJ_DIR = $(BUILD_DIR)/obj_dir
OBJS = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(notdir $(SRCS)))))
BIN = $(BUILD_DIR)/exp3

CC = gcc
CFLAGS = -MMD -O3
CFLAGS += $(addprefix -I, $(INC_PATH))
CFLAGS += -g -DDEBUG

# 这里 OBJS 与 .o 中间不要有空格，会识别成 NULL character
-include $(OBJS:.o=.d)

$(shell mkdir -p $(OBJ_DIR))

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


run: $(BIN)
	@echo "-- exp3: 动态页式存储管理实验"
	@echo "-- RUN ----------------------"
	$(BIN)
	@echo "-- DONE ---------------------"

clean:
	rm -rf $(BUILD_DIR)

.PHONY: default run clean
