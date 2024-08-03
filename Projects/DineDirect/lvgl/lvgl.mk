# 设置LVGL路径
LVGL_PATH := lvgl

# 设置编译选项
INCLUDE_FLAGS := $(shell find $(LVGL_PATH) -type d | xargs -I{} echo "-I{}")
AFLAGS += $(INCLUDE_FLAGS)
CFLAGS += $(INCLUDE_FLAGS)
CXXFLAGS += $(INCLUDE_FLAGS)

# 收集所有的源文件
CSRCS += $(shell find $(LVGL_PATH) -type f -name '*.c')
