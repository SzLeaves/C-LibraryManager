/* 定义登录界面模块 */

#include "database.h"
#include "files.h"
#include "operation.h"
#include <conio.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* login.c私有函数 */

//比对用户信息
static bool user_info_cmp(char users_id[], char users_psaawd[],
                          const UsersNodePtr *users_plist, Users *cur_user);
//控制输入密码时的非明文显示
static void passwd_get(char users_passwd[]);

int login(Users *cur_user) {
    bool isfind = false;
    char optional, set;
    char users_id[USR_ID_LENGTH];
    char users_psaawd[USR_PASSWD_LENGTH];

    FILE *fp = NULL;
    Users users_temp;
    UsersNode *users_plist;

    do {
        //载入链表数据
        InitUsersList(&users_plist);
        if (UsersDataOpen(fp, &users_plist) == false) {
            //载入异常的处理
            fprintf(stderr, "载入失败\n请检查.ldb数据库文件或内存空间\n");
            return exits;
        }

        //初始化当前用户信息
        memset(cur_user, 0, sizeof(Users));

        optional = login_menu();
        switch (optional) {
        case '1':
            printf("请输入用户ID：");
            str_get(users_id, USR_NAMES_LENGTH);
            printf("请输入密码：");
            passwd_get(users_psaawd);

            isfind =
                user_info_cmp(users_id, users_psaawd, &users_plist, cur_user);
            if (isfind) {
                if (cur_user->status == nomal_user) {
                    EmptyUsersList(&users_plist);
                    return nomal_user;
                }

                else if (cur_user->status == admin_user) {
                    EmptyUsersList(&users_plist);
                    return admin_user;
                }

                //用户被禁用时的反馈
                else if (cur_user->status == nologin)
                    printf("该用户已被禁止登录系统\n\n");

            } else {
                //用户不存在时的反馈
                printf("\n登录失败！学号或密码错误\n"
                       "如果您是第一次使用系统，请先注册\n");
            }
            break;

        case '2':
            printf("请输入新用户的ID：");
            str_get(users_temp.usr_id, USR_ID_LENGTH);
            printf("请输入新用户的姓名：");
            str_get(users_temp.usr_names, USR_NAMES_LENGTH);
            printf("请输入新用户的密码：");
            passwd_get(users_temp.usr_passwd);

            //当用户数据库没有信息时，首先分配管理员
            if (users_plist == NULL)
                users_temp.status = admin_user;
            //分配普通用户
            else
                users_temp.status = nomal_user;

            //初始化用户借阅数据
            memset(&users_temp.borrow, 0, sizeof(UsersBooks));

            printf("\n核认信息%s%s%s\n\n", ULINE, ULINE, ULINE);
            PrintUsersMembersInfo(&users_temp);
            printf("\n是否进行注册？(Y/N)：");
            set = getchar();
            flash_buff();

            if (set == 'Y' || set == 'y') {
                if (AddUsersNode(users_temp, &users_plist)) {
                    printf("操作成功完成\n");
                    UsersDataWrite(fp, &users_plist);
                    EmptyUsersList(&users_plist);
                } else
                    printf("操作失败，内存空间已满\n");

            } else if (set == 'N' || set == 'n')
                printf("操作已取消\n");
            else
                printf("对不起，没有这个选项，操作终止\n");

            break;

        case QUIT:
            break;

        default:
            printf("\n对不起，没有这个选项\n");
        }

        if (optional != QUIT) {
            printf("\n请按任意键继续...\n");
            printf("%s%s%s\n", STARS, STARS, STARS);
            getchar();
            system("cls");
        }

        if (optional == QUIT) {
            printf("正在退出，请再次确认 (Y/N)：");
            set = getchar();
            flash_buff();

            if (set == 'N' || set == 'n') {
                system("cls");
                continue;
            } else if (set == 'Y' || set == 'y') {
                break;
            } else {
                printf("对不起，没有这个选项.中断操作\n\n");
                printf("请按任意键继续...\n");
                printf("%s%s%s\n", STARS, STARS, STARS);
                getchar();
                system("cls");
            }
        }

    } while (true);

    EmptyUsersList(&users_plist);
    return exits;
}

//比对用户信息
static bool user_info_cmp(char users_id[], char users_psaawd[],
                          const UsersNodePtr *user_plist, Users *cur_user) {

    UsersNode *pstart = *user_plist;
    bool sign = false;

    while (pstart != NULL) {
        if (!(strcmp(pstart->user.usr_id, users_id)) &&
            !(strcmp(pstart->user.usr_passwd, users_psaawd))) {
            sign = true;
            *cur_user = pstart->user;
            break;
        }
        pstart = pstart->next;
    }

    return sign;
}

//控制密码输入非明文显示
static void passwd_get(char users_passwd[]) {
    //密码字符计数
    int count = 0;
    char ch;

    //对临时存放密码的字符串清零
    memset(users_passwd, 0, sizeof(char));

    //当用户没有按下回车'\r'及字符数小于字符长度时进入循环
    while ('\r' != (ch = _getch()) && count < USR_PASSWD_LENGTH) {
        if (ch != '\b') {
            //当用户没有按下退格时的操作
            users_passwd[count] = ch;
            putchar('*');
            count++;

        } else {
            //当用户按下退格时的操作
            printf("\b \b");
            count--;
        }

        //当用户退格到最顶端时的操作
        if (count < 0) {
            count = 0;
            putchar('\0');
        }
    }

    //给字符串末尾加上空字符结尾
    users_passwd[count] = '\0';
    printf("\n");
}