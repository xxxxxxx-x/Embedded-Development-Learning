# 定义编译器和标志
CC = gcc
CFLAGS = -I./include
LDFLAGS = -L./lib -static
LIBS = -lfont -lm
TARGET_DIR = build/bin
TARGET = $(TARGET_DIR)/dine_simple
OBJ_DIR = build/obj

# 所有源文件
SRC = $(wildcard src/**/*.c) src/main.c

# 所有目标文件
OBJ = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRC))

# 默认目标
all: $(TARGET)

# 链接目标
$(TARGET): $(OBJ)
	@mkdir -p $(TARGET_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)

# 编译源文件为目标文件
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)  # 确保目标目录存在
	@echo "Compiling: $< to $@"  # 显示编译过程
	$(CC) $(CFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
