/* 定义数据结构类型及数据库操作函数原型 */

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdbool.h>
#include <time.h>

/* 数据库数据类型定义 */

//定义用户类型枚举标识符
enum users_type { admin_user = 1, nomal_user = 2, nologin = 3 };

//定义图书数据库操作枚举标识符
enum books_opt_type {
    bookscheck_name = 5,
    bookscheck_type = 6,
    bookscheck_id = 7,
    bookscheck_author = 8,
    bookscheck_num = 9
};

//定义用户数据库操作枚举标识符
enum users_opt_type {
    userscheck_name = 10,
    userscheck_id = 11,
    userscheck_passwd = 12,
    userscheck_staus = 13
};

//定义数据类型的通用长度
#define NOMAL_LENGTH 10

//定义用户数据使用的常量
#define USR_ID_LENGTH 14
#define USR_NAMES_LENGTH 20
#define USR_PASSWD_LENGTH 18
#define USR_BORROW_MAX 10
#define USR_BORROW_DAYS 30

//定义图书类型使用的常量
#define BOOKS_NAMES_LENGTH 50
#define BOOKS_TYPES_LENGTH 20
#define BOOKS_ID_LENGTH 13
#define AUTHORS_NAMES_LENGTH 30

//定义用户和图书数据库的文件名称
#define _db_BOOKS "books.ldb"
#define _db_USERS "users.ldb"

//定义图书数据结构，别名为Books
typedef struct books {
    int books_number;
    char books_names[BOOKS_NAMES_LENGTH];
    char books_types[BOOKS_TYPES_LENGTH];
    char books_id[BOOKS_ID_LENGTH];
    char authors_names[AUTHORS_NAMES_LENGTH];
} Books;

//定义包含图书数据的链表节点，别名为BooksNode
typedef struct books_list {
    Books book;
    struct books_list *next;
} BooksNode;

//定义BooksNode类型的指针（指向图书信息链表节点），别名为BooksNodePtr
//该关键字定义仅在主/被调函数间使用
typedef BooksNode *BooksNodePtr;

//定义用户所借阅图书信息
typedef struct users_borrow {
    unsigned int borrow_num;
    time_t borrow_time[USR_BORROW_MAX];
    time_t return_time[USR_BORROW_MAX];
    Books borrow_books[USR_BORROW_MAX];
} UsersBooks;

//定义用户数据结构，别名为Users
typedef struct users {
    int status;
    char usr_id[USR_ID_LENGTH];
    char usr_names[USR_NAMES_LENGTH];
    char usr_passwd[USR_PASSWD_LENGTH];
    UsersBooks borrow;
} Users;

//定义包含用户数据的链表节点，别名为UsersNode
typedef struct users_list {
    Users user;
    struct users_list *next;
} UsersNode;

//定义UserNode类型的指针（指向用户数据链表节点），别名为UsersNodePtr
//该关键字定义仅在主/被调函数间使用
typedef UsersNode *UsersNodePtr;

/* 数据库操作函数定义 */

/* 操作：初始化需要操作的链表
   前置条件：参数为一个指向需要初始化操作的链表的指针
   后置条件：指针指向的链表初始化为NULL
   注意：传递的参数是指针类型，所以形参使用二级指针 */
void InitBooksList(BooksNodePtr *books_plist); //初始化图书链表
void InitUsersList(UsersNodePtr *users_plist); //初始化用户链表

/* 操作：判断链表是否已满
   后置条件：若链表已满，则返回true，否则返回false */
bool IsBooksListFull(const BooksNodePtr *books_plist);
bool IsUsersListFull(const UsersNodePtr *users_plist);

/* 操作：确定链表节点数量
   前置条件：参数为一个指向需要计算节点数量的链表的指针
   后置条件：遍历节点计数后返回节点数量
   注意：该函数不需要改变指针变量，使用const限定符 */
unsigned int BooksNodeConut(const BooksNodePtr *books_plist);
unsigned int UsersNodeConut(const UsersNodePtr *users_plist);

/* 操作：遍历输出链表所有节点数据域成员
   前置条件：参数为一个指向需要遍历打印链表数据域成员的指针
   注意：该函数不需要改变指针变量，使用const限定符 */
void BooksListCheck(const BooksNodePtr *books_plist);
void UsersListCheck(const UsersNodePtr *users_plist);

/* 操作：遍历输出链表数据域中的一项成员
   前置条件：参数为一个指向需要遍历打印链表数据域成员的指针
   后置条件：遍历节点打印后返回查找项数量
   注意：该函数不需要改变指针变量，使用const限定符 */
unsigned int BooksNodeCheck(const BooksNodePtr *books_plist, int opt_type,
                            char str[]);
unsigned int UsersNodeCheck(const UsersNodePtr *users_plist, int opt_type,
                            char str[], int status);

/* 操作：查找ID并返回对应节点信息
   前置条件：参数为一个指向链表的指针
   后置条件：返回是否找到目标ID */
bool BooksIDCheck(const BooksNodePtr *books_plist, char str[], int *count);
bool UsersIDCheck(const UsersNodePtr *users_plist, char str[], bool *isborrow,
                  bool *isadmin);

/* 操作：查找用户所存储的图书信息
   前置条件：参数为一个指向链表的指针 */
bool UsersBooksCheck(const Users *cur_user, char str[]);
void UsersAllBooksCheck(const Users *cur_user);

/* 操作：修改图书与用户信息中对应借阅数据
   前置条件：1. 参数为一个指向用户链表的指针
             2. 参数为一个指向图书链表的指针
             3. 参数为一个指向当前用户的结构体指针
             4. 参数为一个记录借阅图书ID的字符串 */
void BooksBorrow(BooksNodePtr *books_plist, UsersNodePtr *users_plist,
                 Users *cur_user, char str[]);
void BooksReturn(BooksNodePtr *books_plist, UsersNodePtr *users_plist,
                 Users *cur_user, char str[]);

/* 操作：修改用户数据中的一项信息
   前置条件：参数为一个指向需要修改的链表的指针 */
void UsersInfoModify(UsersNodePtr *users_plist, Users *cur_user, int opt_type,
                     char str[]);
bool AdminModifyUsers(int opt_type, char usr_id[], char str[], int stat,
                      UsersNodePtr *users_plist);

/* 操作：修改图书数据中的一项信息
   前置条件：参数为一个指向需要修改的链表的指针 */
bool AdminModifyBooks(int opt_type, char book_id[], char str[], int num,
                      BooksNodePtr *books_plist);

/* 操作：添加链表节点
   前置条件：1.参数为一个待添加节点的结构类型
             2.参数为一个指向目标链表的指针
   后置条件：在内存空间允许的情况下，该函数在链表末尾添加一个项
             添加成功时返回true，遇到异常则返回false */
bool AddBooksNode(Books books_item, BooksNodePtr *books_plist);
bool AddUsersNode(Users users_item, UsersNodePtr *users_plist);

/* 操作：删除节点
   前置条件：1. 参数为一个待删除指定节点的链表指针
             2. 参数为一个对应删除项的ID
   后置条件：1. 查找对应的节点项
             2. 判断是否为头指针，使用不同的删除策略*/
bool DeleteBooksNode(BooksNodePtr *books_plist, char str[]);
bool DeleteUsersNode(UsersNodePtr *users_plist, char str[]);

/* 操作：清空已分配内存的链表
   前置条件：参数为一个已经初始化的链表
   后置条件：释放为链表分配的内存 */
void EmptyBooksList(BooksNodePtr *books_plist);
void EmptyUsersList(UsersNodePtr *users_plist);

/* 操作：输出图书信息链表中对应成员的信息 */
void PrintBooksMembersInfo(const Books *ptr);
void PrintUsersMembersInfo(const Users *ptr);

#endif