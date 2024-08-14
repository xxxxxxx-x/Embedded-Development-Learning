/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0813/homework2/main.c
 * @brief        : 作业：实现目录的拷贝（拷贝一个目录下的所有文件）
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-13 20:02:33
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// 拷贝文件的函数
bool copyFile(const char* srcFile, const char* destFile) {
  // 打开源文件和目标文件
  FILE* src_fp = fopen(srcFile, "r");
  FILE* dest_fp = fopen(destFile, "w");

  // 如果无法打开文件，输出错误信息并退出
  if (src_fp == NULL || dest_fp == NULL) {
    perror("无法打开文件");
    exit(EXIT_FAILURE);
  }

  // 从源文件读取字符并写入到目标文件
  int ch;
  while ((ch = fgetc(src_fp)) != EOF) {
    fputc(ch, dest_fp);
  }

  // 关闭文件
  fclose(src_fp);
  fclose(dest_fp);

  return true;
}

// 拷贝目录的函数
bool copyDir(const char* srcDir, const char* destDir) {
  // 打开源目录
  DIR* src_dp = opendir(srcDir);
  if (!src_dp) {
    perror("打开源目录失败");
    return false;
  }

  // 创建目标目录
  if (mkdir(destDir, 0777) == -1) {
    perror("创建目标目录失败");
    closedir(src_dp);  // 确保在错误情况下关闭源目录
    return false;
  }

  struct dirent* entry;
  // 读取源目录中的每个条目
  while ((entry = readdir(src_dp)) != NULL) {
    // 忽略 "." 和 ".." 目录
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
      continue;
    }

    // 构建完整的源文件和目标文件路径
    char srcFilePath[strlen(srcDir) + strlen(entry->d_name) + 2];
    sprintf(srcFilePath, "%s/%s", srcDir, entry->d_name);

    char destFilePath[strlen(destDir) + strlen(entry->d_name) + 2];
    sprintf(destFilePath, "%s/%s", destDir, entry->d_name);

    // 如果是目录，递归拷贝该子目录
    if (entry->d_type == DT_DIR) {
      if (!copyDir(srcFilePath, destFilePath)) {
        closedir(src_dp);  // 确保在错误情况下关闭源目录
        return false;
      }
    } else {
      // 如果是文件，拷贝文件
      if (!copyFile(srcFilePath, destFilePath)) {
        closedir(src_dp);  // 确保在错误情况下关闭源目录
        return false;
      }
    }
  }

  // 关闭源目录
  closedir(src_dp);
  return true;
}

int main(int argc, const char* argv[]) {
  // 检查命令行参数是否正确
  if (argc != 3) {
    printf("用法：%s <源目录路径> <目标目录路径>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 执行目录拷贝并输出结果
  printf("拷贝目录%s\n", copyDir(argv[1], argv[2]) ? "成功！" : "失败！");
  return 0;
}
