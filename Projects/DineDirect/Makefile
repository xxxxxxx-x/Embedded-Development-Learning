# 使用的交叉编译器
CC = gcc


# LVGL目录名称和路径
LVGL_DIR_NAME ?= lvgl
LVGL_DIR      ?= .

# 编译警告选项
WARNINGS := -Wall -Wshadow -Wundef -Wmissing-prototypes -Wno-discarded-qualifiers \
            -Wall -Wextra -Wno-unused-function -Wno-error=strict-prototypes \
            -Wpointer-arith -fno-strict-aliasing -Wno-error=cpp -Wuninitialized \
            -Wmaybe-uninitialized -Wno-unused-parameter -Wno-missing-field-initializers \
            -Wtype-limits -Wsizeof-pointer-memaccess -Wno-format-nonliteral \
            -Wno-cast-qual -Wunreachable-code -Wno-switch-default -Wreturn-type \
            -Wmultichar -Wformat-security -Wno-ignored-qualifiers -Wno-error=pedantic \
            -Wno-sign-compare -Wno-error=missing-prototypes -Wdouble-promotion \
            -Wclobbered -Wdeprecated -Wempty-body -Wstack-usage=2048 \
            -Wno-unused-value -Wno-unused-parameter -Wno-missing-field-initializers \
            -Wuninitialized -Wmaybe-uninitialized -Wall -Wextra -Wno-unused-parameter \
            -Wno-missing-field-initializers -Wtype-limits -Wsizeof-pointer-memaccess \
            -Wno-format-nonliteral -Wpointer-arith -Wno-cast-qual -Wmissing-prototypes \
            -Wunreachable-code -Wno-switch-default -Wreturn-type -Wmultichar \
            -Wno-discarded-qualifiers -Wformat-security -Wno-ignored-qualifiers \
            -Wno-sign-compare -std=c99

# 编译选项，包括优化和调试信息
CFLAGS ?= -O3 -g0 -I$(LVGL_DIR)/ $(WARNINGS)
CFLAGS += -D_POSIX_C_SOURCE=200809L


# 链接选项
LDFLAGS ?= -lxkbcommon -linput -pthread
#X11需要的链接库
LDFLAGS +=  -lXext -lXrender -lXrandr -lXinerama -lXi -lX11 -lxcb -lXau -lXdmcp


# 生成的可执行文件名
BIN = demo

# 构建目录
BUILD_DIR = ./buildc
BUILD_OBJ_DIR = $(BUILD_DIR)/obj
BUILD_BIN_DIR = $(BUILD_DIR)/bin

# 安装目录前缀
prefix ?= /usr
bindir ?= $(prefix)/bin

# 要编译的源文件
MAINSRC = main.c

# 包含LVGL和src的Makefile片段
include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/src/src.mk

# 目标文件扩展名
OBJEXT ?= .o

# 源文件编译为目标文件的规则
AOBJS = $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))
OBJS = $(AOBJS) $(COBJS) $(MAINOBJ)
TARGET = $(addprefix $(BUILD_OBJ_DIR)/, $(patsubst ./%, %, $(OBJS)))

# 默认目标，构建可执行文件
all: default

$(BUILD_OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "CC $<"


default: $(TARGET)
	@mkdir -p $(dir $(BUILD_BIN_DIR)/)
	$(CC) -o $(BUILD_BIN_DIR)/$(BIN) $(TARGET) $(LDFLAGS)

# 清理目标文件和生成的可执行文件
clean:
	rm -rf $(BUILD_DIR)

# 安装可执行文件到指定目录
install:
	install -d $(DESTDIR)$(bindir)
	install $(BUILD_BIN_DIR)/$(BIN) $(DESTDIR)$(bindir)

# 卸载已安装的可执行文件
uninstall:
	$(RM) -r $(addprefix $(DESTDIR)$(bindir)/,$(BIN))
    
