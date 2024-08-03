# 设置源文件和头文件路径
SRC_PATH := src
INCLUDE_PATH := include

# 设置编译选项
INCLUDE_FLAGS := $(shell find $(INCLUDE_PATH) -type d | xargs -I{} echo "-I{}")
AFLAGS += $(INCLUDE_FLAGS)
CFLAGS += $(INCLUDE_FLAGS)
CXXFLAGS += $(INCLUDE_FLAGS)

# 收集所有的源文件
CSRCS += $(shell find $(SRC_PATH) -type f -name '*.c')



