# 定义编译器和标志
CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib -static
TARGET_DIR = build/bin
TARGET = $(TARGET_DIR)/dine_simple
OBJ_DIR = build/obj
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJ)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJ)

# 编译源文件为目标文件
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
