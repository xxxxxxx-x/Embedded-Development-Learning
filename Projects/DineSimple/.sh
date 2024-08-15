#!/bin/bash

# 执行 make clean 和 make
make clean
make

# 检查 make 是否成功
if [ $? -eq 0 ]; then
  echo "构建成功。正在将二进制文件传输到远程服务器..."
  scp -r ../DineSimple root@192.168.46.6:~/workspace/code
  if [ $? -eq 0 ]; then
    echo "文件传输成功。"
  else
    echo "文件传输失败。"
  fi
else
  echo "构建失败。中止传输。"
fi
