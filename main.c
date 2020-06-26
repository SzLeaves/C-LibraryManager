/* 定义程序主进程 */

#include "database.h"
#include "operation.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    //控制终端编码，统一以UTF-8编码显示
    system("chcp 65001");
    system("cls");

    int login_stat, logout_stat;
    Users cur_user;

    do {
        //检测登录模块
        login_stat = login(&cur_user);

        if (login_stat == exits)
            break;

        //进入普通用户界面
        if (login_stat == nomal_user)
            logout_stat = MainMenu(nomal_user, &cur_user);

        //进入系统管理员界面
        else if (login_stat == admin_user)
            logout_stat = MainMenu(admin_user, &cur_user);

    } while (logout_stat != exits);

    printf("\n系统已退出\n\n");
    system("pause");

    return 0;
}