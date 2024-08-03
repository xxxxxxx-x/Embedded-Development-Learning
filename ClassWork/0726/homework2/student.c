/****************************************************************************************************************************************************************
 * @file         : /ClassWork/0726/homework2/student.c
 * @brief        : 实现学生信息管理功能
 * @author       : a_23456@foxmail.com
 * @date         : 2024-07-27 10:34:29
 * @version      : 1.1
 * @note         :
 * @Copyright    : (c) 2024-2025 a_23456@foxmail.com All Right Reserved
 *****************************************************************************************************************************************************************/

#include "student.h"
#include "tools.h"
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>

/****************************************************************************************************************************************************************
 * @brief: 根据姓名查找学生
 * @param {char} *name - 学生姓名
 * @return: struct student* - 返回找到的学生指针，未找到返回NULL
 * @date: 2024-07-27 14:03:41
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
struct student *findName(const char *name) {
  for (int i = 0; i < studentCount; i++) {
    if (strcmp(studentArray[i].name, name) == 0) {
      return &studentArray[i];
    }
  }
  return NULL;
}

/****************************************************************************************************************************************************************
 * @brief: 根据手机号查找学生
 * @param {char} *phone - 学生手机号
 * @return: struct student* - 返回找到的学生指针，未找到返回NULL
 * @date: 2024-07-27 14:03:41
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
struct student *findPhone(const char *phone) {
  for (int i = 0; i < studentCount; i++) {
    if (strcmp(studentArray[i].phone, phone) == 0) {
      return &studentArray[i];
    }
  }
  return NULL;
}

/****************************************************************************************************************************************************************
 * @brief: 校验学生姓名
 * @param {char} *name - 学生姓名
 * @return: bool - 合法返回true，不合法返回false
 * @date: 2024-07-27 14:03:41
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
bool validateStudentName(const char *name) {
  size_t len = strlen(name); // 获取姓名长度
  if (len < 2 || len > 3)    // 校验姓名长度是否合法
    return false;
  for (size_t i = 0; i < len; i++) { // 校验姓名字符是否合法
    if ((name[i] < 'a' || name[i] > 'z') && (name[i] < 'A' || name[i] > 'Z'))
      return false;
  }
  return true; // 姓名合法
}

/****************************************************************************************************************************************************************
 * @brief: 校验手机号
 * @param {char} *phone - 学生手机号
 * @return: bool - 合法返回true，不合法返回false
 * @date: 2024-07-28 14:03:41
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
bool validatePhone(const char *phone) {
  size_t len = strlen(phone); // 获取手机号长度
  if (len != 11) {            // 校验手机号长度是否合法
    printf("手机号长度不合法，请输入任意键继续！\n");
    pauseWait();
    return false;
  }

  if (phone[0] != '1' || phone[1] < '3' || phone[1] > '9') {
    printf("手机号格式不正确，请重新输入！\n");
    pauseWait();
    return false;
  }

  for (size_t i = 0; i < len; i++) { // 校验手机号字符是否合法
    if (phone[i] < '0' || phone[i] > '9') {
      printf("手机号包含非法字符，请输入任意键继续！\n");
      pauseWait();
      return false;
    }
  }
  return true; // 手机号合法
}

/****************************************************************************************************************************************************************
 * @name: printStudentInfo
 * @brief: 打印学生信息
 * @param {char} *name - 学生姓名
 * @param {signed int} age - 学生年龄
 * @param {char} *phone - 学生手机号
 * @return: void
 * @date: 2024-07-28
 * @version: 1.0
 * @note: 打印学生信息，内容居中对齐
 *****************************************************************************************************************************************************************/
void printStudentInfo(const char *name, signed int age, const char *phone) {
  int nameWidth = 20;  // 姓名字段宽度
  int ageWidth = 20;   // 年龄字段宽度
  int phoneWidth = 20; // 手机字段宽度

  // 拼接前缀和字段内容
  char nameField[nameWidth + strlen("姓名：") + 1];
  char ageField[ageWidth + strlen("年龄：") +
                4]; // 4 是为了存储最大的三位数年龄和结束符
  char phoneField[phoneWidth + strlen("手机号：") + 1];

  sprintf(nameField, "姓名：%s", name);
  sprintf(ageField, "年龄：%d", age);
  sprintf(phoneField, "手机号：%s", phone);

  // 打印表头
  printf("|");
  printCenteredField(nameField, nameWidth);
  printf("|");
  printCenteredField(ageField, ageWidth);
  printf("|");
  printCenteredField(phoneField, phoneWidth);
  printf("|\n");
}

/****************************************************************************************************************************************************************
 * @brief: 添加学生信息
 * @return: bool - 成功返回true，失败返回false
 * @date: 2024-07-27 10:49:25
 * @version: 1.1
 * @note:
 *****************************************************************************************************************************************************************/
bool addStudent() {

  if (setjmp(quit_env)) {
    printf("已退出[添加学生信息]！\n");
    printf("按任意键继续\n");
    pauseWait();
    return false;
  }

  if (studentCount == STUDENT_ARRAY_SIZE) {
    system("clear");
    printf("[添加学生信息]\n");
    printf("添加失败，学生已满，请输入任意键回到主菜单！\n");
    pauseWait();
    return false;
  }

  struct student newStudent;
  memset(&newStudent, 0, sizeof(newStudent));

  // 输入并校验学生姓名
  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[添加学生信息]吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[添加学生信息]\n");
    printf("请输入学生姓名的拼音简写（如小明：xm,名字长度只能2~3个字,输入 "
           "'!q' 退出添加学生信息）:");
    inputString(newStudent.name, sizeof(newStudent.name));

    if (!validateStudentName(newStudent.name)) {
      printf("学生姓名长度不合法或存在非法字符，请输入任意键继续！\n");
      pauseWait();
      continue;
    }

    if (findName(newStudent.name) != NULL) {
      printf("\"%s\"该学生已存在，请输入任意键继续！\n", newStudent.name);
      pauseWait();
      continue;
    }
    break;
  }

  // 输入并校验学生年龄
  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[添加学生信息]吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[添加学生信息]\n");
    printf("请输入学生姓名的拼音简写（如小明：xm,名字长度只能2~3个字,输入 "
           "'!q' 退出添加学生信息）:%s\n",
           newStudent.name);
    printf("请输入学生年龄（输入 '!q' 退出添加学生信息）:");
    newStudent.age = inputNumber();
    if (newStudent.age <= 0 || newStudent.age > 100) {
      printf("学生年龄不合法，请输入任意键继续！\n");
      pauseWait();
      continue;
    }
    break;
  }

  // 输入并校验学生手机号
  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[添加学生信息]吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[添加学生信息]\n");
    printf("请输入学生姓名的拼音简写（如小明：xm,名字长度只能2~3个字,输入 "
           "'!q' 退出添加学生信息）:%s\n",
           newStudent.name);
    printf("请输入学生年龄（输入 '!q' 退出添加学生信息）:%d\n", newStudent.age);
    printf("请输入学生的手机号（输入 '!q' 退出添加学生信息）:");
    inputString(newStudent.phone, sizeof(newStudent.phone));

    if (!validatePhone(newStudent.phone)) {
      continue;
    }

    if (findPhone(newStudent.phone) != NULL) {
      printf(
          "学生的手机号\"%"
          "s\"已被使用，请使用其他手机号或联系管理员，请输入任意键继续！！\n",
          newStudent.phone);
      pauseWait();
      memset(newStudent.phone, 0, sizeof(newStudent.phone));
      continue;
    }
    break;
  }

  studentArray[studentCount++] = newStudent;
  printf("成功添加学生！\n");
  printf("------------------------------------------------------------\n");
  printStudentInfo(newStudent.name, newStudent.age, newStudent.phone);
  printf("------------------------------------------------------------\n");
  printf("按任意键回到主菜单\n");
  pauseWait();

  return true;
}

/****************************************************************************************************************************************************************
 * @brief: 显示所有学生信息
 * @return: void
 * @date: 2024-07-27 10:50:12
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
void showStudent() {
  if (studentCount == 0) {
    system("clear");
    printf("[显示学生信息]\n");
    printf("惊讶！居然没有一个学生，请赶紧添加一个吧！\n");
  } else {
    system("clear");
    printf("[显示学生信息]\n");
    printf("当前学生数量：%d\n", studentCount);
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < studentCount; i++) {
      printStudentInfo(studentArray[i].name, studentArray[i].age,
                       studentArray[i].phone);
    }
    printf("------------------------------------------------------------\n");
  }
  printf("按任意键回到主菜单\n");
  pauseWait();
}

/****************************************************************************************************************************************************************
 * @brief: 查找学生信息
 * @return: void
 * @date: 2024-07-27 10:51:00
 * @version: 1.0
 * @note:
 *****************************************************************************************************************************************************************/
void findStudent() {

  if (setjmp(quit_env)) {
    printf("已退出[查找学生]！\n");
    printf("按任意键继续\n");
    pauseWait();
    return;
  }

  if (studentCount == 0) {
    system("clear");
    printf("[查找学生]\n");
    printf("无法查询，当前列表中无任何学生！\n");
    printf("按任意键回到主菜单\n");
    pauseWait();
    return;
  }

  char name[20] = {0};
  struct student *student;

  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[查找学生]吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[查找学生]\n");
    printf("请输入要查找学生的简体拼音（如小明：xm，（输入 '!q' "
           "退出查找学生））:");
    inputString(name, sizeof(name));
    student = findName(name);
    if (student == NULL) {
      printf("查找学生失败，当前列表中没有这个学生！\n");
      printf("按任意键继续\n");
      pauseWait();
      continue;
      ;
    }
    break;
  }
  printf("------------------------------------------------------------\n");
  printStudentInfo(student->name, student->age, student->phone);
  printf("------------------------------------------------------------\n");
  printf("按任意键回到主菜单\n");
  pauseWait();
}

/****************************************************************************************************************************************************************
 * @brief: 修改学生信息
 * @return: 成功返回 true，失败返回 false
 * @date: 2024-07-28
 * @version: 1.0
 * @note: 此函数用于修改学生的信息，包括年龄和手机号
 *****************************************************************************************************************************************************************/
bool modifyStudent() {

  if (setjmp(quit_env)) {
    printf("已退出[修改学生信息]！\n");
    printf("按任意键继续\n");
    pauseWait();
    return false;
  }

  if (studentCount == 0) {
    system("clear");
    printf("[修改学生信息]\n");
    printf("无法修改，当前列表中无任何学生！\n");
    printf("按任意键回到主菜单\n");
    pauseWait();
    return false;
  }

  char name[20] = {0};
  struct student *student;

  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[修改学生信息]吗？非Y/y表示取消退出：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[修改学生信息]\n");
    printf("请输入要修改学生的简体拼音（如小明：xm，输入 '!q' "
           "退出修改学生信息）:");

    inputString(name, sizeof(name));
    student = findName(name);
    if (student == NULL) {
      printf("修改学生失败，当前列表中没有这个学生！\n");
      printf("按任意键继续\n");
      pauseWait();
      continue;
    }
    break;
  }

  signed int newAge = student->age;
  char newPhone[12];
  strcpy(newPhone, student->phone);

  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[修改学生信息]吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[修改学生信息]\n");
    printf("请输入要修改学生的简体拼音（如小明：xm，输入 '!q' "
           "退出修改学生信息）:%s\n",
           student->name);
    printf("请输入\"%s\"学生的新年龄（当前年龄是%d，回车表示不修改，输入 '!q' "
           "退出修改学生信息）:",
           student->name, student->age);
    char ageInput[4];
    inputString(ageInput, sizeof(ageInput));
    if (ageInput[0] != '\0') {
      newAge = (signed int)atoi(ageInput);
      if (newAge <= 0 || newAge > 100) {
        printf("输入的新年龄不合法，请输入任意键继续！\n");
        pauseWait();
        continue;
      }
    }
    break;
  }

  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[修改学生信息]吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[修改学生信息]\n");
    printf("请输入要修改学生的简体拼音（如小明：xm，输入 '!q' "
           "退出修改学生信息）:%s\n",
           student->name);
    printf("请输入\"%s\"学生的新年龄（当前年龄是%d，回车表示不修改，输入 '!q' "
           "退出修改学生信息）:%d\n",
           student->name, student->age, newAge);
    printf("请输入\"%s\"学生的新手机号（当前手机号是%s，回车表示不修改，输入 "
           "'!q' 退出修改学生信息）:",
           student->name, student->phone);
    char phoneInput[12];
    inputString(phoneInput, sizeof(phoneInput));
    if (phoneInput[0] != '\0') {
      strcpy(newPhone, phoneInput);
      if (!validatePhone(newPhone)) {
        continue;
      }
      if (findPhone(newPhone) != NULL) {
        printf(
            "输入的新手机号\"%"
            "s\"已被使用，请使用其他手机号或联系管理员，请输入任意键继续！！\n",
            newPhone);
        pauseWait();
        continue;
      }
    }
    break;
  }

  printf("原信息：\n");
  printf("------------------------------------------------------------\n");
  printStudentInfo(student->name, student->age, student->phone);
  printf("------------------------------------------------------------\n");

  // 更新学生信息
  student->age = newAge;
  strcpy(student->phone, newPhone);

  printf("已变更：\n");
  printf("------------------------------------------------------------\n");
  printStudentInfo(student->name, student->age, student->phone);
  printf("------------------------------------------------------------\n");
  printf("成功修改学生信息！\n");
  printf("按任意键回到主菜单\n");
  pauseWait();

  return true;
}

/****************************************************************************************************************************************************************
 * @brief: 删除学生信息
 * @return: 成功返回 true，失败返回 false
 * @date: 2024-07-28
 * @version: 1.0
 * @note: 此函数用于删除学生信息
 *****************************************************************************************************************************************************************/
bool deleteStudent() {

  if (setjmp(quit_env)) {
    printf("已退出[删除学生信息]！\n");
    printf("按任意键继续\n");
    pauseWait();
    return false;
  }

  if (studentCount == 0) {
    system("clear");
    printf("[删除学生信息]\n");
    printf("无法删除，当前列表中无任何学生！\n");
    printf("按任意键回到主菜单\n");
    pauseWait();
    return false;
  }

  char name[20] = {0};
  struct student *student;

  while (true) {
    if (setjmp(back_env)) {
      printf("您确定要退出[删除学生信息]吗？（非Y/y表示取消退出）：");
      char confirmation[3];
      inputString(confirmation, sizeof(confirmation));
      if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
        continue;
      }
      longjmp(quit_env, 1);
    }
    system("clear");
    printf("[删除学生信息]\n");
    printf("请输入要删除学生的简体拼音（如小明：xm，输入 '!q' "
           "退出删除学生信息）:");
    inputString(name, sizeof(name));
    student = findName(name);
    if (student == NULL) {
      printf("删除学生失败，当前列表中没有这个学生！\n");
      printf("按任意键继续\n");
      pauseWait();
      continue;
    }

    printf("待删除学生：\n");
    printf("------------------------------------------------------------\n");
    printStudentInfo(student->name, student->age, student->phone);
    printf("------------------------------------------------------------\n");
    printf("请确认删除学生（非Y/y表示取消删除，输入 '!q' 退出删除学生信息）：");
    char confirmation[3];
    inputString(confirmation, sizeof(confirmation));

    if (strcmp(confirmation, "Y") != 0 && strcmp(confirmation, "y") != 0) {
      printf("已取消删除操作。\n");
      printf("按任意键回到主菜单\n");
      pauseWait();
      continue;
    }

    break;
  }

  for (int i = 0; i < studentCount; i++) {
    if (strcmp(studentArray[i].name, name) == 0) {
      for (int j = i; j < studentCount - 1; j++) {
        studentArray[j] = studentArray[j + 1];
      }
      memset(&studentArray[studentCount - 1], 0,
             sizeof(struct student)); // 将最后一个元素置空
      studentCount--;
      printf("已成功删除学生\"%s\"！\n", name);
      printf("按任意键回到主菜单\n");
      pauseWait();
      return true;
    }
  }

  return false;
}
