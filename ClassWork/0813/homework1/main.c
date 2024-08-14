/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0813/homework1/main.c
 * @brief        : 作业：统计一个目录下的所有文件大小总和
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-13 18:48:23
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// 计算指定路径下的所有文件大小总和
long calculateTotalSize(const char* dirPath) {
  long totalSize = 0;           // 文件大小总和
  DIR* dir = opendir(dirPath);  // 打开目录

  // 如果无法打开目录，输出错误信息并退出
  if (!dir) {
    perror("打开目录失败");
    exit(EXIT_FAILURE);
  }

  struct dirent* entry;
  // 读取目录中的每个条目
  while ((entry = readdir(dir)) != NULL) {
    // 忽略 "." 和 ".." 目录
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // 构建完整的文件路径
    char filePath[strlen(dirPath) + strlen(entry->d_name) + 2];
    sprintf(filePath, "%s/%s", dirPath, entry->d_name);

    // 如果是目录，递归计算该子目录的文件大小
    if (entry->d_type == DT_DIR) {
      totalSize += calculateTotalSize(filePath);
    } else {
      // 如果是文件，获取文件属性并累加文件大小
      struct stat fileStat;
      if (lstat(filePath, &fileStat) == -1) {
        perror("获取文件属性失败");
        exit(EXIT_FAILURE);
      }
      totalSize += fileStat.st_size;
    }
  }

  closedir(dir);     // 关闭目录
  return totalSize;  // 返回总文件大小
}

int main(int argc, const char* argv[]) {
  // 检查命令行参数是否正确
  if (argc != 2) {
    printf("用法：%s <目录路径>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 计算并输出指定目录下的所有文件大小总和
  printf("该目录下的所有文件大小总和是：%ld 字节\n",
         calculateTotalSize(argv[1]));
  return 0;
}
