/* 定义显示菜单函数 */

#define _CRT_SECURE_NO_WARNINGS

#include "database.h"
#include "operation.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//登录界面菜单
char login_menu(void) {
    char optional;

    printf("-- 欢迎使用图书管理系统 --\n"
           "   使用之前请先登录/注册  \n");
    printf("%s%s%s\n", STARS, STARS, STARS);
    printf("输入对应标号以进行需要操作的选项：\n");
    printf("1) 登录系统    2) 注册用户\n"
           "Q) 退出系统\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();

    return optional;
}

//主界面菜单
char main_menu(int usr_type, const char usr_name[]) {
    char optional;

    switch (usr_type) {
    case nomal_user:
        printf("%s  欢迎  学生：%s  %s\n\n", STARS, usr_name, STARS);
        printf("输入对应标号以进行需要操作的选项----->\n");
        printf("1) 查找书籍       2) 借阅/归还书籍\n"
               "3) 查看个人信息   L) 注销当前用户\n"
               "Q) 退出系统\n");
        printf("请输入：");
        optional = getchar();
        flash_buff();
        break;

    case admin_user:
        printf("%s  欢迎  管理员：%s  %s\n\n", STARS, usr_name, STARS);
        printf("输入对应标号以进行需要操作的选项----->\n");
        printf("1) 查找书籍       2) 借阅/归还书籍\n"
               "3) 查看个人信息   4) 管理用户信息\n"
               "5) 管理图书信息   L) 注销当前用户\n"
               "Q) 退出系统\n");
        printf("请输入：");
        optional = getchar();
        flash_buff();
        break;
    }

    return optional;
}

//查找图书界面菜单
char check_books_menu(char *str) {
    char optional;

    printf("%s 查找书籍 %s\n\n", STARS, STARS);
    printf("请选择需要操作的选项----->\n");
    printf("1) 按书名查找    2) 按图书类型查找\n"
           "3) 按图书ID查找  4) 按图书作者查找\n"
           "Q) 退出本界面\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();

    switch (optional) {
    case '1':
        printf("请输入书名：");
        break;

    case '2':
        printf("请输入图书类别：");
        break;

    case '3':
        printf("请输入图书ID：");
        break;

    case '4':
        printf("请输入图书作者：");
        break;

    case QUIT:
        break;

    default:
        printf("\n对不起，没有这个选项.\n\n");
        optional = 'f';
        break;
    }

    if (optional != QUIT && optional != 'f')
        str_get(str, NOMAL_LENGTH * 10);

    return optional;
}

//查找用户界面菜单
char check_users_menu(char *str, int *status) {
    char optional;

    printf("%s 查找用户 %s\n\n", STARS, STARS);
    printf("请选择需要操作的选项----->\n");
    printf("1) 按用户名查找    2) 按用户ID查找\n"
           "3) 按用户类型查找  Q) 退出本界面\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();

    switch (optional) {
    case '1':
        printf("请输入用户名：");
        break;

    case '2':
        printf("请输入用户ID：");
        break;

    case '3':
        printf("\n管理员 = 1，普通用户 = 2，禁止登录 = 3\n");
        printf("请输入用户类型代码：");
        scanf("%d", status);
        flash_buff();
        break;

    case QUIT:
        break;

    default:
        printf("\n对不起，没有这个选项.\n\n");
        optional = 'f';
        break;
    }

    if (optional != QUIT && optional != 'f' && optional != '3')
        str_get(str, NOMAL_LENGTH * 10);

    return optional;
}

//借阅/归还图书界面菜单
char borrow_books_menu(char *str) {
    char optional;

    printf("%s 借阅/归还书籍 %s\n\n", STARS, STARS);
    printf("请选择需要操作的选项----->\n");
    printf("1) 借阅图书    2) 归还图书\n"
           "Q) 退出本界面\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();

    switch (optional) {
    case '1':
        printf("输入图书ID以借阅图书\n"
               "一次性借阅30天，请输入：");
        break;
    case '2':
        printf("输入图书ID以归还图书：\n请输入：");
        break;
    case QUIT:
        break;
    default:
        printf("对不起，没有这个选项\n");
        optional = 'f';
        break;
    }

    if (optional != QUIT && optional != 'f') {
        str_get(str, NOMAL_LENGTH * 10);
        printf("\n");
    }

    return optional;
}

//个人信息界面菜单
char modify_userinfo_menu(char *str) {
    char optional;

    printf("%s 查看个人信息 %s\n\n", STARS, STARS);
    printf("请选择需要操作的选项----->\n");
    printf("1) 查看借阅图书信息    2) 修改用户名\n"
           "3) 修改用户密码        Q) 退出本界面\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();

    switch (optional) {
    case '1':
        break;

    case '2':
        printf("请输入新用户名：");
        str_get(str, USR_NAMES_LENGTH);
        break;

    case '3':
        printf("请输入新密码：");
        str_get(str, USR_PASSWD_LENGTH);
        break;

    case QUIT:
        break;

    default:
        printf("对不起，没有这个选项\n");
    }

    return optional;
}

//管理员--管理图书界面菜单
char admin_managebooks_menu(char *str) {
    char optional;

    printf("%s 管理图书信息 %s\n\n", STARS, STARS);
    printf("请选择需要操作的选项----->\n");
    printf("1) 查看全部图书信息    2) 查看指定图书信息\n"
           "3) 添加图书            4) 删除图书\n"
           "5) 修改图书信息        Q) 退出本界面\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();

    switch (optional) {
    case '1':
    case '2':
        break;

    case '3':
        printf("请输入需要添加的图书信息：\n");
        printf("  -- 输入格式 --\n"
               "图书库存数量 <回车>\n"
               "图书名称     <回车>\n"
               "图书类型     <回车>\n"
               "图书ID       <回车>\n"
               "图书作者     <回车>\n");
        printf("\n请在下面依次输入：\n");
        printf("%s%s\n", ULINE, ULINE);
        break;

    case '4':
        printf("请输入需要删除的图书ID：");
        str_get(str, BOOKS_ID_LENGTH);
        printf("\n");
        break;

    case '5':
        break;

    case QUIT:
        break;

    default:
        printf("对不起，没有这个选项\n");
        optional = 'f';
        break;
    }

    return optional;
}

//管理员--管理用户界面菜单
char admin_manageusers_menu(char *str) {
    char optional;

    printf("%s 管理用户信息 %s\n\n", STARS, STARS);
    printf("请选择需要操作的选项----->\n");
    printf("1) 查看全部用户信息    2) 查看指定用户信息\n"
           "3) 添加用户            4) 删除用户\n"
           "5) 修改用户信息        Q) 退出本界面\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();

    switch (optional) {
    case '1':
    case '2':
        break;

    case '3':
        printf("请输入需要添加的用户信息：\n");
        printf("  -- 输入格式 --\n"
               "用户类型   <回车>\n"
               "用户ID     <回车>\n"
               "用户名     <回车>\n"
               "--> 注意：密码将默认设置为空\n");
        printf("\n请在下面依次输入：\n");
        printf("%s%s\n", ULINE, ULINE);
        break;

    case '4':
        printf("请输入需要删除的用户ID：");
        str_get(str, USR_ID_LENGTH);
        printf("\n");
        break;

    case '5':
        break;

    case QUIT:
        break;

    default:
        printf("对不起，没有这个选项\n");
        optional = 'f';
        break;
    }

    return optional;
}

//管理用户界面--子菜单：更改用户信息选项
char modify_usersinfo_menu(void) {
    char optional;

    printf("\n%s%s%s%s\n", ULINE, ULINE, ULINE, ULINE);
    printf("选择一个需要更改的选项\n");
    printf("1) 更改用户类型  2) 更改用户ID\n"
           "3) 更改用户名    4) 更改用户密码\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();
    
    return optional;
}

//管理图书界面--子菜单：更改图书信息选项
char modify_booksinfo_menu(void) {
    char optional;

    printf("\n%s%s%s%s\n", ULINE, ULINE, ULINE, ULINE);
    printf("选择一个需要更改的选项\n");
    printf("1) 更改图书类型  2) 更改图书ID\n"
           "3) 更改图书名称  4) 更改图书作者\n"
           "5) 更改图书数量\n");
    printf("请选择：");
    optional = getchar();
    flash_buff();
    
    return optional;
}