/* 定义系统基本操作函数原型 */

#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "database.h"
#include <stdio.h>

//输出格式化宏定义
#define STARS "**********"
#define ULINE "----------"
#define LOGOUT 'L'
#define QUIT 'Q'

//定义模块操作返回状态的枚举标识符
enum operation { failure = -1, success = -2, logout = -3, exits = -4 };

/* 操作：显示用户菜单
   后置条件：根据传入的用户类型参数选择对应的菜单显示
   函数定义在showmenu.c中 */
char login_menu(void);
char main_menu(int users_type, const char usr_name[]);
char check_books_menu(char *str);
char check_users_menu(char *str, int *status);
char borrow_books_menu(char *str);
char modify_userinfo_menu(char *str);
char admin_managebooks_menu(char *str);
char admin_manageusers_menu(char *str);
char modify_usersinfo_menu(void);
char modify_booksinfo_menu(void);

/* 操作：提供登录界面进行身份认证
   后置条件：返回用户身份是否认证成功/失败的状态枚举标识符
   函数定义在login.c中 */
int login(Users *cur_user);

/* 操作：进入系统主界面
   后置条件：无 */
int MainMenu(int usr_type, Users *cur_user);

/* 操作：进入查找图书的界面
   后置条件：无 */
int CheckBooks(void);

/* 操作：进入查找用户的界面
   后置条件：无 */
int CheckUsers(void);

/* 操作：进入借阅图书的界面
   后置条件：无 */
int BorrowBooks(Users *cur_user);

/* 操作：进入修改用户信息的界面
   后置条件：无 */
int ModifyUserInfo(Users *cur_user);

/* 操作：进入管理图书的界面
   后置条件：无 */
int AdminManagerBooks(void);

/* 操作：进入管理用户的界面
   后置条件：无 */
int AdminManagerUsers(void);

/* 操作：读取用户的基本字符输入
   后置条件：函数读取输入字符串及其长度，处理后返回字符串地址*/
char *str_get(char str[], int len_str);

/* 操作：清空缓冲区 */
void flash_buff(void);

#endif