/* 定义用户基本操作函数 */

#define _CRT_SECURE_NO_WARNINGS

#include "operation.h"
#include "database.h"
#include "files.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//系统主菜单操作流程
int MainMenu(int usr_type, Users *cur_user) {
    system("cls");
    int abnormal = 0;
    char optional, set;

    if (cur_user->usr_passwd[0] == '\0')
        printf("您当前密码为空\n"
               "为了您的账户安全，请尽快修改密码\n");

    do {
        switch (usr_type) {
        case nomal_user:
            optional = main_menu(nomal_user, cur_user->usr_names);

            if (optional != QUIT && optional != LOGOUT) {
                switch (optional) {
                case '1':
                    system("cls");
                    abnormal = CheckBooks();
                    break;

                case '2':
                    system("cls");
                    abnormal = BorrowBooks(cur_user);
                    break;

                case '3':
                    system("cls");
                    abnormal = ModifyUserInfo(cur_user);
                    break;

                default:
                    system("cls");
                    printf("对不起，没有这个选项.\n\n");
                    break;
                }
            }
            break;

        case admin_user:
            optional = main_menu(admin_user, cur_user->usr_names);

            if (optional != QUIT && optional != LOGOUT) {
                switch (optional) {
                case '1':
                    system("cls");
                    abnormal = CheckBooks();
                    break;

                case '2':
                    system("cls");
                    abnormal = BorrowBooks(cur_user);
                    break;

                case '3':
                    system("cls");
                    abnormal = ModifyUserInfo(cur_user);
                    break;

                case '4':
                    system("cls");
                    abnormal = AdminManagerUsers();
                    break;

                case '5':
                    system("cls");
                    abnormal = AdminManagerBooks();
                    break;

                default:
                    system("cls");
                    printf("对不起，没有这个选项.\n\n");
                    break;
                }
            }
            break;
        }

        //用户选择注销/退出系统时的操作
        if (optional == LOGOUT || optional == QUIT) {
            if (optional == LOGOUT)
                printf("正在注销，请再次确认 (Y/N)：");
            else
                printf("正在退出，请再次确认 (Y/N)：");

            set = getchar();
            flash_buff();

            if (set == 'N' || set == 'n') {
                system("cls");
                continue;
            } else if (set == 'Y' || set == 'y') {
                if (optional == LOGOUT) {
                    //注销状态
                    system("cls");
                    return logout;
                } else
                    //退出状态
                    return exits;
            } else {
                system("cls");
                printf("对不起，没有这个选项.中断操作\n\n");
            }
        }

        //模块状态返回异常时的处理
        if (abnormal == failure) {
            fprintf(stderr, "载入失败\n请检查.ldb数据库文件或内存空间\n");
            return exits;
        }

    } while (true);
}

//查找图书操作流程
int CheckBooks(void) {
    int total = 0;
    char optional;

    FILE *books_fp = NULL;
    BooksNodePtr books_list;

    printf("载入数据库...");
    InitBooksList(&books_list);
    if (BooksDataOpen(books_fp, &books_list) == false)
        return failure;
    printf("载入成功\n");

    do {
        char *str = (char *)calloc(NOMAL_LENGTH * 10, sizeof(char));
        optional = check_books_menu(str);

        if (optional != QUIT && optional != 'f') {
            printf("查找中...\n\n");
            switch (optional) {
            //按图书名称查找
            case '1': {
                total = BooksNodeCheck(&books_list, bookscheck_name, str);
                break;
            }

            //按图书类型查找
            case '2': {
                total = BooksNodeCheck(&books_list, bookscheck_type, str);
                break;
            }

            //按图书ID查找
            case '3': {
                total = BooksNodeCheck(&books_list, bookscheck_id, str);
                break;
            }

            //按图书作者查找
            case '4': {
                total = BooksNodeCheck(&books_list, bookscheck_author, str);
                break;
            }
            }
            printf("\n查找完成：共找到%d份相同信息\n\n", total);
        }

        if (optional != QUIT) {
            printf("请按回车键继续...\n%s%s%s", STARS, STARS, STARS);
            getchar();
        }
        system("cls");

        //释放临时分配的字符串存储空间
        free(str);

    } while (optional != QUIT);

    EmptyBooksList(&books_list);
    system("cls");
    return success;
}

//查找用户操作流程
int CheckUsers(void) {
    int total = 0, status;
    char optional;

    FILE *users_fp = NULL;
    UsersNodePtr users_list;

    printf("载入数据库...");
    InitUsersList(&users_list);
    if (UsersDataOpen(users_fp, &users_list) == false)
        return failure;
    printf("载入成功\n");

    do {
        char *str = (char *)calloc(NOMAL_LENGTH * 10, sizeof(char));
        optional = check_users_menu(str, &status);

        if (optional != QUIT && optional != 'f') {
            printf("查找中...\n\n");
            switch (optional) {
            //按用户名查找
            case '1': {
                total =
                    UsersNodeCheck(&users_list, userscheck_name, str, status);
                break;
            }

            //按用户ID查找
            case '2': {
                total = UsersNodeCheck(&users_list, userscheck_id, str, status);
                break;
            }

            //按用户类型查找
            case '3': {
                total =
                    UsersNodeCheck(&users_list, userscheck_staus, str, status);
                break;
            }
            }
            printf("\n查找完成：共找到%d份相同信息\n\n", total);
        }

        if (optional != QUIT) {
            printf("请按回车键继续...\n%s%s%s", STARS, STARS, STARS);
            getchar();
        }
        system("cls");

        //释放临时分配的字符串存储空间
        free(str);

    } while (optional != QUIT);

    EmptyUsersList(&users_list);
    system("cls");
    return success;
}

//借阅/归还图书操作流程
int BorrowBooks(Users *cur_user) {
    char set, optional;
    bool isborrow = false, isreturn = false, isfind = false;
    FILE *books_fp = NULL, *users_fp = NULL;
    BooksNodePtr books_list;
    UsersNodePtr users_list;

    printf("载入数据库...");
    InitBooksList(&books_list);
    InitUsersList(&users_list);
    if (BooksDataOpen(books_fp, &books_list) == false)
        return failure;
    if (UsersDataOpen(users_fp, &users_list) == false)
        return failure;

    printf("载入成功\n");

    do {
        int books_num = -1;

        char *str = (char *)calloc(NOMAL_LENGTH * 10, sizeof(char));
        optional = borrow_books_menu(str);

        if (optional != QUIT && optional != 'f') {

            switch (optional) {
            //借阅图书
            case '1': {
                isfind = BooksIDCheck(&books_list, str, &books_num);
                //判断查阅图书库存是否充足
                if (isfind && books_num > 0 &&
                    cur_user->borrow.borrow_num < USR_BORROW_MAX) {

                    printf("查找完成，是否借阅该图书？（Y/N）：");
                    set = getchar();
                    flash_buff();

                    if (set == 'Y' || set == 'y') {
                        BooksBorrow(&books_list, &users_list, cur_user, str);
                        isborrow = true;
                        printf("操作已完成\n");
                    } else if (set == 'N' || set == 'n') {
                        printf("操作已取消\n");
                    } else
                        printf("对不起，没有这个选项\n");

                } else if (books_num == 0) {
                    printf("该书馆藏数量为0，无法进行借阅！\n");
                } else if (cur_user->borrow.borrow_num > USR_BORROW_MAX) {
                    printf(
                        "您借阅的图书已经达到上限(10本)，请先归还再借阅！\n");
                } else
                    printf("没有找到相关的图书信息！\n");

                break;
            }

            //归还图书
            case '2': {
                isfind = UsersBooksCheck(cur_user, str);

                if (isfind) {
                    printf("查找完成，是否归还该图书？（Y/N）：");
                    set = getchar();
                    flash_buff();

                    if (set == 'Y' || set == 'y') {
                        BooksReturn(&books_list, &users_list, cur_user, str);
                        isreturn = true;
                        printf("操作已完成\n");
                    } else if (set == 'N' || set == 'n') {
                        printf("操作已取消\n");
                    } else
                        printf("对不起，没有这个选项\n");
                } else
                    printf("没有找到相关的图书信息！\n");

                break;
            }
            }
        }

        //检测到链表数据被修改时将数据写入
        if (isborrow == true || isreturn == true) {
            BooksDataWrite(books_fp, &books_list);
            UsersDataWrite(users_fp, &users_list);
            isborrow = isreturn = false;
        }

        if (optional != QUIT) {
            printf("请按回车键继续...\n%s%s%s", STARS, STARS, STARS);
            getchar();
        }
        system("cls");

        //释放临时分配的字符串存储空间
        free(str);

    } while (optional != QUIT);

    EmptyBooksList(&books_list);
    EmptyUsersList(&users_list);
    system("cls");

    return success;
}

//修改个人信息操作流程
int ModifyUserInfo(Users *cur_user) {
    bool ischange = false;
    char set, optional;
    FILE *users_fp = NULL;
    UsersNodePtr users_list;

    printf("载入数据库...");
    InitUsersList(&users_list);
    if (UsersDataOpen(users_fp, &users_list) == false)
        return failure;

    printf("载入成功\n");

    do {
        char *str = (char *)calloc(NOMAL_LENGTH * 10, sizeof(char));
        optional = modify_userinfo_menu(str);

        switch (optional) {
        //查看个人信息
        case '1': {
            UsersAllBooksCheck(cur_user);
            break;
        }

        //修改用户名
        case '2': {
            printf("\n原用户名：%s\n", cur_user->usr_names);
            printf("新用户名：%s\n", str);
            printf("确认更改吗？(Y/N)：");
            set = getchar();
            flash_buff();

            if (set == 'Y' || set == 'y') {
                UsersInfoModify(&users_list, cur_user, userscheck_name, str);
                ischange = true;
                printf("\n操作已完成\n");
            } else if (set == 'N' || set == 'n') {
                printf("\n操作已取消\n");
            } else {
                printf("对不起，没有这个选项\n");
            }
            break;
        }

        //修改用户密码
        case '3': {
            printf("\n原密码：%s\n", cur_user->usr_passwd);
            printf("新密码：%s\n", str);
            printf("确认更改吗？(Y/N)：");
            set = getchar();
            flash_buff();

            if (set == 'Y' || set == 'y') {
                UsersInfoModify(&users_list, cur_user, userscheck_passwd, str);
                ischange = true;
                printf("\n操作已完成\n");
            } else if (set == 'N' || set == 'n') {
                printf("\n操作已取消\n");
            } else {
                printf("对不起，没有这个选项\n");
            }
            break;
        }

        case QUIT:
            break;
        }

        //检测到链表数据被修改时将数据写入
        if (ischange) {
            UsersDataWrite(users_fp, &users_list);
            ischange = false;
        }

        if (optional != QUIT && optional != '1') {
            printf("请按回车键继续...\n%s%s%s", STARS, STARS, STARS);
            getchar();
        }
        system("cls");
        free(str);

    } while (optional != QUIT);

    EmptyUsersList(&users_list);
    system("cls");

    return success;
}

//管理图书操作流程
int AdminManagerBooks(void) {
    bool ischange = false, isfind;
    char op, set, optional;

    Books book_temp;
    FILE *books_fp = NULL;
    BooksNodePtr books_list;

    printf("载入数据库...");
    InitBooksList(&books_list);
    if (BooksDataOpen(books_fp, &books_list) == false)
        return failure;
    printf("载入成功\n");

    do {
        int books_num = 0;
        char *str = (char *)calloc(NOMAL_LENGTH * 10, sizeof(char));
        optional = admin_managebooks_menu(str);

        if (optional != QUIT && optional != 'f') {
            switch (optional) {
            //检索所有图书信息
            case '1': {
                BooksListCheck(&books_list);
                break;
            }

            //检索特定图书信息
            case '2': {
                system("cls");
                CheckBooks();
                break;
            }

            //添加图书信息
            case '3': {
                //输入信息
                scanf("%d", &book_temp.books_number);
                flash_buff();
                str_get(book_temp.books_names, BOOKS_NAMES_LENGTH);
                str_get(book_temp.books_types, BOOKS_TYPES_LENGTH);
                str_get(book_temp.books_id, BOOKS_ID_LENGTH);
                str_get(book_temp.authors_names, AUTHORS_NAMES_LENGTH);
                printf("%s%s\n", ULINE, ULINE);

                printf("需要添加的信息：\n\n");
                PrintBooksMembersInfo(&book_temp);

                printf("是否添加？(Y/N)：");
                set = getchar();
                flash_buff();
                if (set == 'Y' || set == 'y') {
                    if (AddBooksNode(book_temp, &books_list)) {
                        printf("操作成功完成\n");
                        ischange = true;
                    } else
                        printf("操作失败，请检查内存空间\n");
                } else
                    printf("操作已取消\n");
                break;
            }

            //删除图书信息
            case '4': {
                if (BooksIDCheck(&books_list, str, &books_num)) {
                    printf("是否删除该图书信息？(Y/N)\n"
                           "警告：该操作不可逆：");
                    set = getchar();
                    flash_buff();

                    if (set == 'Y' || set == 'y') {
                        if (DeleteBooksNode(&books_list, str)) {
                            printf("操作成功完成\n");
                            ischange = true;
                        } else
                            printf("操作失败，请检查内存空间\n");
                    } else
                        printf("操作已取消\n");
                } else
                    printf("没有找到相关的图书信息！\n");

                break;
            }

            //修改图书的特定信息
            case '5': {
                isfind = false;
                char *uid = (char *)calloc(BOOKS_ID_LENGTH, sizeof(char));
                op = modify_booksinfo_menu();

                switch (op) {
                //更改图书类型
                case '1': {
                    printf("请输入需要进行操作的图书ID：");
                    str_get(uid, BOOKS_ID_LENGTH);
                    printf("请输入更改后的图书类型：");
                    str_get(str, BOOKS_TYPES_LENGTH);

                    isfind = AdminModifyBooks(bookscheck_type, uid, str,
                                              books_num, &books_list);
                    if (isfind) {
                        ischange = true;
                        BooksIDCheck(&books_list, uid, &books_num);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的图书\n\n");

                    break;
                }

                //更改图书ID
                case '2': {
                    printf("请输入需要进行操作的图书ID：");
                    str_get(uid, BOOKS_ID_LENGTH);
                    printf("请输入更改后的图书ID，长度为13位数：");
                    str_get(str, BOOKS_ID_LENGTH);

                    isfind = AdminModifyBooks(bookscheck_id, uid, str,
                                              books_num, &books_list);
                    if (isfind) {
                        ischange = true;
                        BooksIDCheck(&books_list, uid, &books_num);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的图书\n\n");

                    break;
                }

                //更改图书名称
                case '3': {
                    printf("请输入需要进行操作的图书ID：");
                    str_get(uid, BOOKS_ID_LENGTH);
                    printf("请输入更改后的图书名称：");
                    str_get(str, BOOKS_NAMES_LENGTH);

                    isfind = AdminModifyBooks(bookscheck_name, uid, str,
                                              books_num, &books_list);
                    if (isfind) {
                        ischange = true;
                        BooksIDCheck(&books_list, uid, &books_num);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的图书\n\n");

                    break;
                }

                //更改图书作者
                case '4': {
                    printf("请输入需要进行操作的图书ID：");
                    str_get(uid, BOOKS_ID_LENGTH);
                    printf("请输入更改后的图书作者：");
                    str_get(str, AUTHORS_NAMES_LENGTH);

                    isfind = AdminModifyBooks(bookscheck_author, uid, str,
                                              books_num, &books_list);
                    if (isfind) {
                        ischange = true;
                        BooksIDCheck(&books_list, uid, &books_num);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的图书\n\n");

                    break;
                }

                //更改图书数量
                case '5': {
                    printf("请输入需要进行操作的图书ID：");
                    str_get(uid, BOOKS_ID_LENGTH);
                    printf("请输入更改后的图书数量：");
                    scanf("%d", &books_num);
                    flash_buff();

                    isfind = AdminModifyBooks(bookscheck_num, uid, str,
                                              books_num, &books_list);
                    if (isfind) {
                        ischange = true;
                        BooksIDCheck(&books_list, uid, &books_num);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的图书\n\n");

                    break;
                }

                default:
                    printf("对不起，没有这个选项\n");
                }

                free(uid);
                break;
            }

            }
        }

        //检测到链表数据被修改时将数据写入
        if (ischange) {
            BooksDataWrite(books_fp, &books_list);
            ischange = false;
        }

        if (optional != QUIT && optional != '1' && optional != '2') {
            printf("请按回车键继续...\n%s%s%s", STARS, STARS, STARS);
            getchar();
        }
        system("cls");

        //释放临时分配的字符串存储空间
        free(str);

    } while (optional != QUIT);

    EmptyBooksList(&books_list);
    system("cls");
    return success;
}

//管理用户操作流程
int AdminManagerUsers(void) {
    int status;
    bool ischange, isborrow, isadmin, isfind;
    char op, set, optional;

    Users users_temp;
    FILE *users_fp = NULL;
    UsersNodePtr users_list;

    printf("载入数据库...");
    InitUsersList(&users_list);
    if (UsersDataOpen(users_fp, &users_list) == false)
        return failure;
    printf("载入成功\n");

    do {
        char *str = (char *)calloc(NOMAL_LENGTH * 10, sizeof(char));
        ischange = isborrow = isadmin = isfind = false;
        optional = admin_manageusers_menu(str);

        if (optional != QUIT && optional != 'f') {
            switch (optional) {
            //检索所有用户信息
            case '1': {
                UsersListCheck(&users_list);
                break;
            }

            //检索特定用户信息
            case '2': {
                system("cls");
                CheckUsers();
                break;
            }

            //添加用户信息
            case '3': {
                //输入信息
                scanf("%d", &users_temp.status);
                flash_buff();
                str_get(users_temp.usr_id, USR_ID_LENGTH);
                str_get(users_temp.usr_names, USR_NAMES_LENGTH);
                memset(users_temp.usr_passwd, 0, sizeof(char));
                memset(&users_temp.borrow, 0, sizeof(UsersBooks));
                printf("%s%s\n", ULINE, ULINE);

                printf("需要添加的信息：\n\n");
                PrintUsersMembersInfo(&users_temp);

                printf("是否添加？(Y/N)：");
                set = getchar();
                flash_buff();

                if (set == 'Y' || set == 'y') {
                    if (AddUsersNode(users_temp, &users_list)) {
                        printf("\n操作成功完成\n");
                        ischange = true;
                    } else
                        printf("\n操作失败，请检查内存空间\n");
                } else
                    printf("\n操作已取消\n");
                break;
            }

            //删除用户信息
            case '4': {
                if (UsersIDCheck(&users_list, str, &isborrow, &isadmin)) {
                    if (isborrow == false && isadmin == false) {
                        printf("是否删除该用户？(Y/N)\n"
                               "警告：该操作不可逆：");
                        set = getchar();
                        flash_buff();

                        if (set == 'Y' || set == 'y') {
                            if (DeleteUsersNode(&users_list, str)) {
                                printf("\n操作成功完成\n");
                                ischange = true;
                            } else
                                printf("操作失败，请检查内存空间\n");
                        } else
                            printf("\n操作已取消\n");
                    } else if (isborrow) {
                        printf(
                            "该用户有借阅记录，请先进行还书操作后再删除！\n");
                    } else if (isadmin) {
                        printf("该用户为管理员，无法删除！\n");
                    }
                } else {
                    printf("没有找到相关的用户信息！\n");
                }
                break;
            }

            //修改用户的特定信息
            case '5': {
                char *uid = (char *)calloc(USR_ID_LENGTH, sizeof(char));
                op = modify_usersinfo_menu();

                switch (op) {
                //更改用户状态
                case '1': {
                    printf("请输入需要进行操作的用户ID：");
                    str_get(uid, USR_ID_LENGTH);
                    printf("请输入更改后的用户类型\n"
                           "管理员 = 1，普通用户 = 2，禁止登录 = 3\n请输入：");
                    scanf("%d", &status);
                    flash_buff();

                    isfind = AdminModifyUsers(userscheck_staus, uid, str,
                                              status, &users_list);
                    if (isfind) {
                        ischange = true;
                        UsersIDCheck(&users_list, uid, &isborrow, &isadmin);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的用户\n\n");

                    break;
                }

                //更改用户ID
                case '2': {
                    printf("请输入需要进行操作的用户ID：");
                    str_get(uid, USR_ID_LENGTH);
                    printf("请输入更改后的用户ID，长度为13位数：");
                    str_get(str, USR_ID_LENGTH);

                    isfind = AdminModifyUsers(userscheck_id, uid, str, status,
                                              &users_list);
                    if (isfind) {
                        ischange = true;
                        UsersIDCheck(&users_list, uid, &isborrow, &isadmin);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的用户\n\n");

                    break;
                }

                //更改用户名称
                case '3': {
                    printf("请输入需要进行操作的用户ID：");
                    str_get(uid, USR_ID_LENGTH);
                    printf("请输入更改后的用户名：");
                    str_get(str, USR_NAMES_LENGTH);

                    isfind = AdminModifyUsers(userscheck_name, uid, str, status,
                                              &users_list);
                    if (isfind) {
                        ischange = true;
                        UsersIDCheck(&users_list, uid, &isborrow, &isadmin);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的用户\n\n");

                    break;
                }

                //更改用户密码
                case '4': {
                    printf("请输入需要进行操作的用户ID：");
                    str_get(uid, USR_ID_LENGTH);
                    printf("请输入更改后的用户密码：");
                    str_get(str, USR_PASSWD_LENGTH);

                    isfind = AdminModifyUsers(userscheck_passwd, uid, str,
                                              status, &users_list);
                    if (isfind) {
                        ischange = true;
                        UsersIDCheck(&users_list, uid, &isborrow, &isadmin);
                        printf("\n操作成功完成\n");
                    } else
                        printf("\n没有找到需要操作的用户\n\n");

                    break;
                }

                default:
                    printf("对不起，没有这个选项\n");
                }

                free(uid);
                break;
            }
            }
        }

        //检测到链表数据被修改时将数据写入
        if (ischange)
            UsersDataWrite(users_fp, &users_list);

        if (optional != QUIT && optional != '1' && optional != '2') {
            printf("请按回车键继续...\n%s%s%s", STARS, STARS, STARS);
            getchar();
        }

        system("cls");

        //释放临时分配的字符串存储空间
        free(str);

    } while (optional != QUIT);

    EmptyUsersList(&users_list);
    system("cls");

    return success;
}

//读取用户基本输入函数
char *str_get(char str[], int len_str) {
    int i = 0;
    char *ptd = fgets(str, len_str, stdin);
    if (ptd) {
        while (ptd[i] != '\n' && ptd[i] != '\0')
            i++;

        if (ptd[i] == '\n')
            ptd[i] = '\0';
        else
            flash_buff();
    }

    return ptd;
}

//刷新缓冲区
void flash_buff(void) {
    while (getchar() != '\n')
        continue;
}