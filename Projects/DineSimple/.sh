#!/bin/sh

# 定义一些常量
SOURCE_DIR="../DineSimple"
DESTINATION_DIR="root@192.168.46.6:~/workspace/code"
TAR_FILE="DineSimple.tar.gz"
EXCLUDE_DIR="resources/animations/boot_logo"
SSH_PASSWORD="."

# 提示用户是否执行 make clean，设置默认值为 n
echo "是否执行 'make clean'？ (y/n) [默认: n]: \c"
read do_clean
# 如果没有输入，则默认值为 n
if [ -z "$do_clean" ]; then
    do_clean="n"
fi

if [ "$do_clean" = "y" ]; then
    echo "执行 make clean 和 make..."
    make clean && make
    if [ $? -ne 0 ]; then
        echo "构建失败。中止后续操作。"
        exit 1
    fi
elif [ "$do_clean" = "n" ]; then
    echo "执行 make..."
    make
    if [ $? -ne 0 ]; then
        echo "构建失败。中止后续操作。"
        exit 1
    fi
else
    echo "无效输入。请输入 'y' 或 'n'。"
    exit 1
fi

# 在当前目录中创建 tar 压缩包并排除指定目录
echo "创建压缩包..."
tar --exclude="$TAR_FILE" --exclude="$EXCLUDE_DIR" -cvzf "$TAR_FILE" "$SOURCE_DIR"

if [ $? -ne 0 ]; then
    echo "压缩包创建失败。"
    exit 1
fi

# 传输压缩包
echo "传输压缩包到远程服务器..."
sshpass -p "$SSH_PASSWORD" scp "$TAR_FILE" "$DESTINATION_DIR"
if [ $? -ne 0 ]; then
    echo "压缩包传输失败。"
    exit 1
fi

# 在目标机器上解压缩包
echo "在远程服务器上解压缩包并执行构建..."
sshpass -p "$SSH_PASSWORD" ssh root@192.168.46.6 "
    tar -xvzf ~/workspace/code/$TAR_FILE -C ~/workspace/code &&
    rm ~/workspace/code/$TAR_FILE
"
if [ $? -ne 0 ]; then
    echo "解压缩或删除压缩包失败。"
    exit 1
fi

# 提示用户是否执行远程可执行文件，设置默认值为 y
echo "是否在远程服务器上执行 './build/bin/dine_simple'？ (y/n) [默认: n]: \c"
read do_run
# 如果没有输入，则默认值为 n
if [ -z "$do_run" ]; then
    do_run="n"
fi

if [ "$do_run" = "y" ]; then
    echo "执行远程可执行文件..."
    sshpass -p "$SSH_PASSWORD" ssh root@192.168.46.6 "
        cd /root/workspace/code/DineSimple && ./build/bin/dine_simple
    "
    if [ $? -ne 0 ]; then
        echo "远程可执行文件执行失败。"
        exit 1
    fi
elif [ "$do_run" = "n" ]; then
    echo "跳过远程可执行文件执行。"
else
    echo "无效输入。请输入 'y' 或 'n'。"
    exit 1
fi

echo "所有操作成功完成。"
