/* 定义数据库操作函数 */

#define _CRT_SECURE_NO_WARNINGS

#include "database.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//输出格式化定义
#define SPEA "==========="

/* database.c私有函数 */
/*  操作：将对应的结构拷贝至链表节点数据域中
   前置条件：1.参数为一个数据类型结构体
             2.参数为一个指向对应数据类型结构体链表的指针
    后置条件：函数拷贝对应的结构项至链表节点数据域中 */
static void CopyBooksToNode(Books book, BooksNode *books_pnode);
static void CopyUsersToNode(Users user, UsersNode *users_pnode);

/* 操作：计算用户的借阅时间
   前置条件：1.参数为两个time_t类型的指针，分别对应借出与归还时间
             2.参数为一个表示图书借阅操作的枚举量 */
static void CalBorrowTime(time_t *borrow_time, time_t *return_time);
static bool CalReturnTime(time_t *borrow_time, time_t *return_time);

/* 公用函数模块 */

//初始化链表函数
void InitBooksList(BooksNodePtr *books_plist) { *books_plist = NULL; }
void InitUsersList(UsersNodePtr *users_plist) { *users_plist = NULL; }

//判断已满链表函数
bool IsBooksListFull(const BooksNodePtr *books_plist) {
    bool isfull;
    BooksNode *temp_ptr;

    //使用malloc尝试给temp_ptr分配空间
    temp_ptr = (BooksNode *)malloc(sizeof(BooksNode));
    if (temp_ptr == NULL)
        isfull = true;
    else
        isfull = false;

    free(temp_ptr); //测试结束后释放内存空间
    return isfull;
}

bool IsUsersListFull(const UsersNodePtr *users_plist) {
    bool isfull;
    UsersNode *temp_ptr;

    //使用malloc尝试给temp_ptr分配空间
    temp_ptr = (UsersNode *)malloc(sizeof(UsersNode));
    if (temp_ptr == NULL)
        isfull = true;
    else
        isfull = false;

    free(temp_ptr); //测试结束后释放内存空间
    return isfull;
}

//链表节点数量计数函数
unsigned int BooksNodeConut(const BooksNodePtr *books_plist) {
    unsigned int count = 0;
    BooksNode *pstart = *books_plist;
    while (pstart != NULL) {
        pstart = pstart->next;
        count++;
    }

    return count;
}

unsigned int UsersNodeConut(const UsersNodePtr *users_plist) {
    unsigned int count = 0;
    UsersNode *pstart = *users_plist;
    while (pstart != NULL) {
        pstart = pstart->next;
        count++;
    }

    return count;
}

//遍历输出链表所有数据域成员信息
void BooksListCheck(const BooksNodePtr *books_plist) {
    int count;
    BooksNode *psave = *books_plist;

    do {
        system("cls");
        printf("%s%s%s\n\n", SPEA, SPEA, SPEA);

        for (count = 0; count < 5; count++) {
            if (psave == NULL)
                break;
            PrintBooksMembersInfo(&psave->book);
            psave = psave->next;
        }

        printf("本页共%d项，请按回车键继续...\n", count);
        printf("\n%s%s%s\n", SPEA, SPEA, SPEA);
        getchar();
    } while (psave != NULL);
}

void UsersListCheck(const UsersNodePtr *users_plist) {
    int count;
    UsersNode *psave = *users_plist;

    do {
        system("cls");
        printf("%s%s\n\n", SPEA, SPEA);

        for (count = 0; count < 3; count++) {
            if (psave == NULL)
                break;
            PrintUsersMembersInfo(&psave->user);
            psave = psave->next;
        }

        printf("本页共%d项，请按回车键继续...\n", count);
        printf("\n%s%s\n", SPEA, SPEA);
        getchar();
    } while (psave != NULL);
}

//查找/输出链表成员函数
unsigned int BooksNodeCheck(const BooksNodePtr *books_plist, int opt_type,
                            char str[]) {
    unsigned int count = 0;
    BooksNode *psave = *books_plist;

    while (psave != NULL) {
        switch (opt_type) {
        case bookscheck_name:
            if (strstr(psave->book.books_names, str)) {
                PrintBooksMembersInfo(&psave->book);
                count++;
            }
            break;

        case bookscheck_type:
            if (!(strcmp(psave->book.books_types, str))) {
                PrintBooksMembersInfo(&psave->book);
                count++;
            }
            break;

        case bookscheck_id:
            if (!(strcmp(psave->book.books_id, str))) {
                PrintBooksMembersInfo(&psave->book);
                count++;
            }
            break;

        case bookscheck_author:
            if (strstr(psave->book.authors_names, str)) {
                PrintBooksMembersInfo(&psave->book);
                count++;
            }
            break;
        }

        psave = psave->next;
    }

    return count;
}

unsigned int UsersNodeCheck(const UsersNodePtr *users_plist, int opt_type,
                            char str[], int status) {
    unsigned int count = 0;
    UsersNode *psave = *users_plist;

    while (psave != NULL) {
        switch (opt_type) {
        case userscheck_name:
            if (strstr(psave->user.usr_names, str)) {
                PrintUsersMembersInfo(&psave->user);
                count++;
            }
            break;

        case userscheck_id:
            if (strstr(psave->user.usr_id, str)) {
                PrintUsersMembersInfo(&psave->user);
                count++;
            }
            break;

        case userscheck_staus:
            if (status == psave->user.status) {
                PrintUsersMembersInfo(&psave->user);
                count++;
            }
            break;
        }

        psave = psave->next;
    }

    return count;
}

//查找图书ID
bool BooksIDCheck(const BooksNodePtr *books_plist, char str[], int *count) {
    bool isfind = false;
    BooksNode *psave = *books_plist;
    while (psave != NULL) {
        if (!(strcmp(psave->book.books_id, str))) {
            isfind = true;
            *count = psave->book.books_number;
            PrintBooksMembersInfo(&psave->book);
            break;
        }

        psave = psave->next;
    }

    return isfind;
}

//查找用户ID
bool UsersIDCheck(const UsersNodePtr *users_plist, char str[], bool *isborrow,
                  bool *isadmin) {
    bool isfind = false;
    UsersNode *psave = *users_plist;
    while (psave != NULL) {
        if (!(strcmp(psave->user.usr_id, str))) {
            isfind = true;
            //判断用户是否有借阅记录
            if (psave->user.borrow.borrow_num != 0)
                *isborrow = true;
            //判断用户是否为管理员
            if (psave->user.status == admin_user)
                *isadmin = true;

            PrintUsersMembersInfo(&psave->user);
            break;
        }

        psave = psave->next;
    }

    return isfind;
}

//查找用户已借阅图书
bool UsersBooksCheck(const Users *cur_user, char str[]) {
    bool isfind = false;
    for (int i = 0; i < USR_BORROW_MAX; i++) {
        if (!(strcmp(cur_user->borrow.borrow_books[i].books_id, str)) &&
            str[0] != '\0') {

            isfind = true;
            printf("图书名称：%s\n",
                   cur_user->borrow.borrow_books[i].books_names);
            printf("图书ID：%s\n", cur_user->borrow.borrow_books[i].books_id);
            printf("类别：%s\n", cur_user->borrow.borrow_books[i].books_types);
            printf("作者：%s\n\n",
                   cur_user->borrow.borrow_books[i].authors_names);
            break;
        }
    }
    return isfind;
}

//遍历用户所有图书信息
void UsersAllBooksCheck(const Users *cur_user) {
    system("cls");
    printf("%s%s%s\n\n", SPEA, SPEA, SPEA);

    for (int i = 0; i < USR_BORROW_MAX; i++) {
        if (cur_user->borrow.borrow_books[i].books_number != 0) {
            printf("图书名称：%s\n",
                   cur_user->borrow.borrow_books[i].books_names);
            printf("图书ID：%s\n", cur_user->borrow.borrow_books[i].books_id);
            printf("类别：%s\n", cur_user->borrow.borrow_books[i].books_types);
            printf("作者：%s\n\n",
                   cur_user->borrow.borrow_books[i].authors_names);
            printf("借阅时间：%s\n", ctime(cur_user->borrow.borrow_time));
            printf("归还时间：%s\n\n", ctime(cur_user->borrow.return_time));
        }
    }

    printf("共%d项，请按回车键继续...\n\n", cur_user->borrow.borrow_num);
    printf("%s%s%s\n\n", SPEA, SPEA, SPEA);
    getchar();
    system("cls");
}

//借阅功能--修改借阅后图书数量
void BooksBorrow(BooksNodePtr *books_plist, UsersNodePtr *users_plist,
                 Users *cur_user, char str[]) {
    unsigned int num = 0;
    BooksNode *books_save = *books_plist;
    UsersNode *users_save = *users_plist;

    //分别对用户与图书链表信息进行定位
    while (books_save != NULL) {
        if (!(strcmp(books_save->book.books_id, str)))
            break;
        books_save = books_save->next;
    }

    while (users_save != NULL) {
        if (!(strcmp(users_save->user.usr_id, cur_user->usr_id)))
            break;
        users_save = users_save->next;
    }

    //减少图书数据库中对应的图书数量
    books_save->book.books_number--;

    //定位用户借阅信息位置
    for (unsigned int i = 0; i < USR_BORROW_MAX; i++) {
        /* 检索当前用户借阅的图书数组borrow.borrow_books[i]中
           没有被分配数据的位置（即数组中所有元素均为0的数组下标 */
        if (cur_user->borrow.borrow_books[i].books_number == 0) {
            num = i;
            break;
        }
    }

    printf("\n-- 借阅成功 --\n\n");
    //修改用户借阅信息
    cur_user->borrow.borrow_books[num] = books_save->book;
    CalBorrowTime(&cur_user->borrow.borrow_time[num],
                  &cur_user->borrow.return_time[num]);
    cur_user->borrow.borrow_num++;

    //将修改信息写入链表
    users_save->user.borrow = cur_user->borrow;
}

//还书功能--修改归还后图书数量
void BooksReturn(BooksNodePtr *books_plist, UsersNodePtr *users_plist,
                 Users *cur_user, char str[]) {
    unsigned int num = 0;
    BooksNode *books_save = *books_plist;
    UsersNode *users_save = *users_plist;

    //分别对用户与图书链表信息进行定位
    while (books_save != NULL) {
        if (!(strcmp(books_save->book.books_id, str)))
            break;
        books_save = books_save->next;
    }

    while (users_save != NULL) {
        if (!(strcmp(users_save->user.usr_id, cur_user->usr_id)))
            break;
        users_save = users_save->next;
    }

    //增加图书数据库中对应的图书数量
    books_save->book.books_number++;

    //定位用户借阅信息位置
    for (unsigned int i = 0; i < USR_BORROW_MAX; i++) {
        /* 检索当前用户借阅的图书数组borrow.borrow_books[i]中
           没有被分配数据的位置（即数组中所有元素均为0的数组下标 */
        if (!(strcmp(books_save->book.books_id,
                     cur_user->borrow.borrow_books[i].books_id))) {
            num = i;
            break;
        }
    }

    //修改用户借阅信息
    printf("\n-- 返还成功 --\n\n");
    if (CalReturnTime(&cur_user->borrow.borrow_time[num],
                      &cur_user->borrow.return_time[num])) {
        printf("\n由于该书已逾期归还，您需要缴纳超期借阅金额\n");
    } else {
        printf("\n归期内返还，无需缴纳超期借阅金额\n");
    }

    memset(&cur_user->borrow.borrow_books[num], 0, sizeof(Books));
    cur_user->borrow.borrow_num--;

    //将修改信息写入链表
    users_save->user.borrow = cur_user->borrow;
}

//修改用户数据中的一项信息
void UsersInfoModify(UsersNodePtr *users_plist, Users *cur_user, int opt_type,
                     char str[]) {

    UsersNode *psave = *users_plist;

    //定位链表中的用户信息
    while (psave != NULL) {
        if (!(strcmp(psave->user.usr_id, cur_user->usr_id)))
            break;
        psave = psave->next;
    }

    switch (opt_type) {
    case userscheck_name:
        strncpy(cur_user->usr_names, str, USR_NAMES_LENGTH);
        strncpy(psave->user.usr_names, cur_user->usr_names, USR_NAMES_LENGTH);
        break;

    case userscheck_passwd:
        strncpy(cur_user->usr_passwd, str, USR_PASSWD_LENGTH);
        strncpy(psave->user.usr_passwd, cur_user->usr_passwd,
                USR_PASSWD_LENGTH);
        break;
    }
}

bool AdminModifyUsers(int opt_type, char usr_id[], char str[], int stat,
                      UsersNodePtr *users_plist) {

    bool isfind = false;
    UsersNode *psave = *users_plist;

    //定位链表中的用户信息
    while (psave != NULL) {
        if (!(strcmp(psave->user.usr_id, usr_id))) {
            isfind = true;
            break;
        }
        psave = psave->next;
    }

    if (isfind) {
        switch (opt_type) {
        case userscheck_staus:
            psave->user.status = stat;
            break;

        case userscheck_id:
            strncpy(psave->user.usr_id, str, USR_ID_LENGTH);
            break;

        case userscheck_name:
            strncpy(psave->user.usr_names, str, USR_NAMES_LENGTH);
            break;

        case userscheck_passwd:
            strncpy(psave->user.usr_passwd, str, USR_PASSWD_LENGTH);
            break;
        }
    }

    return isfind;
}

bool AdminModifyBooks(int opt_type, char book_id[], char str[], int num,
                      BooksNodePtr *books_plist) {
    bool isfind = false;
    BooksNode *psave = *books_plist;

    //定位链表中的图书信息
    while (psave != NULL) {
        if (!(strcmp(psave->book.books_id, book_id))) {
            isfind = true;
            break;
        }
        psave = psave->next;
    }

    if (isfind) {
        switch (opt_type) {
        case bookscheck_type:
            strncpy(psave->book.books_types, str, BOOKS_TYPES_LENGTH);
            break;

        case bookscheck_id:
            strncpy(psave->book.books_id, str, BOOKS_ID_LENGTH);
            break;

        case bookscheck_name:
            strncpy(psave->book.books_names, str, BOOKS_NAMES_LENGTH);
            break;

        case bookscheck_author:
            strncpy(psave->book.authors_names, str, AUTHORS_NAMES_LENGTH);
            break;

        case bookscheck_num:
            psave->book.books_number = num;
            break;
        }
    }

    return isfind;
}

//添加链表节点函数
bool AddBooksNode(Books books_item, BooksNodePtr *books_plist) {
    BooksNode *pnew = NULL;
    BooksNode *plast = *books_plist;

    //为新节点分配空间
    pnew = (BooksNode *)calloc(1, sizeof(BooksNode));

    //异常抛出：判断内存空间是否已满
    if (pnew == NULL)
        return false;

    //将结构数据写入节点中，并将指针域设为NULL
    CopyBooksToNode(books_item, pnew);
    pnew->next = NULL;

    //对已经填充数据的节点进行首尾连接
    //当没有头指针时，把book_plist设为头指针
    if (plast == NULL)
        *books_plist = pnew;
    //当头指针已经存在时，总是执行下面的连接规则，连接头指针之后的节点
    else {
        while (plast->next != NULL)
            plast = plast->next; //查找链表末尾
        plast->next = pnew;
    }

    return true;
}

bool AddUsersNode(Users users_item, UsersNodePtr *users_plist) {
    UsersNode *pnew = NULL;
    UsersNode *plast = *users_plist;

    //为新节点分配空间
    pnew = (UsersNode *)calloc(1, sizeof(UsersNode));

    //异常抛出：判断内存空间是否已满
    if (pnew == NULL)
        return false;

    //将结构数据写入节点中，并将指针域设为NULL
    CopyUsersToNode(users_item, pnew);
    pnew->next = NULL;

    //对已经填充数据的节点进行首尾连接
    //当没有头指针时，把book_plist设为头指针
    if (plast == NULL)
        *users_plist = pnew;
    //当头指针已经存在时，总是执行下面的连接规则，连接头指针之后的节点
    else {
        while (plast->next != NULL)
            plast = plast->next; //查找链表末尾
        plast->next = pnew;
    }

    return true;
}

//删除节点函数
bool DeleteBooksNode(BooksNodePtr *books_plist, char str[]) {
    bool isfind = false;
    BooksNode *psave = *books_plist;
    BooksNode *ptr_front = NULL, *ptr_remove = NULL;
    ptr_front = ptr_remove = *books_plist;

    //查找待删除的节点
    while (psave != NULL) {
        if (!(strcmp(psave->book.books_id, str))) {
            isfind = true;
            break;
        }
        //更新前置节点的地址，并指向下一节点查找
        ptr_front = psave;
        psave = psave->next;
    }

    if (isfind) {
        //当前节点为头指针时执行
        if (psave == *books_plist) {
            *books_plist = psave->next;
            ptr_remove = psave;
        }
        //当前节点为头指针之后的指针时执行
        else {
            ptr_front->next = psave->next;
            ptr_remove = psave;
        }

        ptr_remove->next = NULL;
        free(ptr_remove);
    }

    return isfind;
}

bool DeleteUsersNode(UsersNodePtr *users_plist, char str[]) {
    bool isfind = false;
    UsersNode *psave = *users_plist;
    UsersNode *ptr_front = NULL, *ptr_remove = NULL;
    ptr_front = ptr_remove = *users_plist;

    //查找待删除的节点
    while (psave != NULL) {
        if (!(strcmp(psave->user.usr_id, str))) {
            isfind = true;
            break;
        }
        //更新前置节点的地址，并指向下一节点查找
        ptr_front = psave;
        psave = psave->next;
    }

    if (isfind) {
        //当前节点为头指针时执行
        if (psave == *users_plist) {
            *users_plist = psave->next;
            ptr_remove = psave;
        }
        //当前节点为头指针之后的指针时执行
        else {
            ptr_front->next = psave->next;
            ptr_remove = psave;
        }

        ptr_remove->next = NULL;
        free(ptr_remove);
    }

    return isfind;
}

//清空链表函数
void EmptyBooksList(BooksNodePtr *books_plist) {
    //初始化临时存储指针域的指针
    BooksNode *books_list_save = NULL;

    while (*books_plist != NULL) {
        books_list_save = (*books_plist)->next; //保存下一节点地址
        free(*books_plist);                     //释放当前节点
        *books_plist = books_list_save; //将下一节点地址赋给下次释放的节点地址
    }
}

void EmptyUsersList(UsersNodePtr *users_plist) {
    //初始化临时存储指针域的指针
    UsersNode *users_list_save = NULL;

    while (*users_plist != NULL) {
        users_list_save = (*users_plist)->next; //保存下一节点地址
        free(*users_plist);                     //释放当前节点
        *users_plist = users_list_save; //将下一节点地址赋给下次释放的节点地址
    }
}

//输出图书节点数据域中对应成员的信息
void PrintBooksMembersInfo(const Books *ptr) {
    printf("\n-- %s ", ptr->books_names);
    printf("馆藏数：%d --\n", ptr->books_number);
    printf("图书ID：%s\n", ptr->books_id);
    printf("类别：%s\n", ptr->books_types);
    printf("作者：%s\n\n", ptr->authors_names);
}

//输出用户节点数据域中对应成员的信息
void PrintUsersMembersInfo(const Users *ptr) {
    printf("\n-- %s ", ptr->usr_names);
    printf("用户ID：%s --\n", ptr->usr_id);
    printf("用户类别：%d\n", ptr->status);
    printf("用户密码：%s\n", ptr->usr_passwd);

    if (ptr->borrow.borrow_num) {
        printf("用户所借图书：\n\n");
        for (int i = 0; i < USR_BORROW_MAX; i++)
            if (ptr->borrow.borrow_books[i].books_number != 0)
                PrintBooksMembersInfo(&ptr->borrow.borrow_books[i]);
    } else
        printf("用户暂无借阅记录\n\n");
    printf("<---------------------------------->\n\n");
}

/* database.c私有函数 */

//将指定结构拷贝至链表数据域中
static void CopyBooksToNode(Books book, BooksNode *books_pnode) {
    books_pnode->book = book;
}

static void CopyUsersToNode(Users user, UsersNode *users_pnode) {
    users_pnode->user = user;
}

// 用户借阅时间计算函数
static void CalBorrowTime(time_t *borrow_time, time_t *return_time) {
    // time()返回从1970-01-01 00:00:00 UTC起经过的时间（以秒为单位）
    *borrow_time = time(NULL);
    //对借阅天数进行换算（以秒为单位）
    *return_time = *borrow_time + (USR_BORROW_DAYS * 3600 * 24);

    printf("---> 借阅时间：%s\n", ctime(borrow_time));
    printf("---> 归还时间：%s\n", ctime(return_time));
}

// 用户返还时间计算函数
static bool CalReturnTime(time_t *borrow_time, time_t *return_time) {
    time_t cur_time = time(NULL);
    time_t result = cur_time - *return_time;

    printf("---> 当前时间：%s\n", ctime(&cur_time));
    printf("---> 借阅时间：%s\n", ctime(borrow_time));
    printf("---> 归还时间：%s\n", ctime(return_time));

    //重置用户借阅时间信息
    memset(borrow_time, 0, sizeof(time_t));
    memset(return_time, 0, sizeof(time_t));

    if (result > 0)
        return true;

    return false;
}