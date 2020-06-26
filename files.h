/* 定义数据库文件读写操作函数原型 */

#ifndef _FILES_H_
#define _FILES_H_

#include "database.h"
#include <stdbool.h>
#include <stdio.h>

/* 操作：判断文件是否存在
   后置条件：调用_access()判断，存在返回true，否则返回false */
bool IsBooksDataExist(void);
bool IsUsersDataExist(void);

/* 操作：打开数据库文件并将信息载入链表
   前置条件：1. 参数为一个文件类型的指针
             2. 参数为一个指向链表指针类型的指针
   后置条件：以二进制只读方式打开目标数据库，
             读取文件信息并载入链表中，
             完成后关闭文件（清空文件指针）*/
bool BooksDataOpen(FILE *fp, BooksNodePtr *books_list);
bool UsersDataOpen(FILE *fp, UsersNodePtr *users_list);

/* 操作：将已经修改的数据写入数据库中
   前置条件：1. 参数为一个文件类型的指针
             2. 参数为一个指向链表指针类型的指针，标识为只读
   后置条件：以二进制只写方式打开并清空目标数据库，
             读取链表信息并写入到文件中，
             完成后关闭文件 */
bool BooksDataWrite(FILE *fp, const BooksNodePtr *books_list);
bool UsersDataWrite(FILE *fp, const UsersNodePtr *users_list);

#endif