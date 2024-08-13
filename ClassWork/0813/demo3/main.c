/****************************************************************************************************************************************************************
 * @file         : /Code/ClassWork/0813/demo3/main.c
 * @brief        : 小练习:利用递归检索目录，获取 ~下的所有bmp图片
 * @author       : a_23456@foxmail.com
 * @date         : 2024-08-13 15:27:18
 * @version      : 1.0
 * @note         :
 * @Copyright    : (c)   2024-2025   a_23456@foxmail.com   All Right Reserved
 *****************************************************************************************************************************************************************/

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 递归搜索目录下的所有bmp文件
void searchBmp(const char *dirPath) {
  // 打开目录
  DIR *dp = opendir(dirPath);
  if (!dp) {
    perror("打开目录失败");
    exit(EXIT_FAILURE);
  }

  struct dirent *entry;
  // 读取目录中的每个条目
  while ((entry = readdir(dp)) != NULL) {
    // 跳过 . 和 ..
    if (entry->d_name[0] == '.') {
      continue;
    }

    // 如果是目录，则递归调用
    if (entry->d_type == DT_DIR) {
      char path[4096] = {0};
      snprintf(path, sizeof(path), "%s/%s", dirPath, entry->d_name);
      searchBmp(path);
    } else {
      // 检查文件扩展名是否为 .bmp
      int name_len = strlen(entry->d_name);
      if (name_len > 4 && strcmp(entry->d_name + name_len - 4, ".bmp") == 0) {
        printf("BMP文件: %s/%s\n", dirPath, entry->d_name);
      }
    }
  }

  // 关闭目录
  closedir(dp);
}

int main(int argc, const char *argv[]) {
  // 检索用户主目录下的所有bmp文件
  searchBmp("/home/user");
  return 0;
}
