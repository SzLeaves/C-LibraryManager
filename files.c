/* 定义数据库文件读写操作函数 */

#define _CRT_SECURE_NO_WARNINGS

#include "files.h"
#include "database.h"
#include <io.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//判断数据库文件是否存在
bool IsBooksDataExist(void) {
    //_access在文件存在时返回0，否则返回-1
    if (!(_access(_db_BOOKS, 00)))
        return true;
    return false;
}

bool IsUsersDataExist(void) {
    //_access在文件存在时返回0，否则返回-1
    if (!(_access(_db_USERS, 00)))
        return true;
    return false;
}

//打开数据库文件并将数据载入链表
bool BooksDataOpen(FILE *fp, BooksNodePtr *books_list) {
    unsigned int total = 0;

    //检测链表是否已满
    if (IsBooksListFull(&(*books_list)))
        return false;

    //检测数据库文件是否存在，不存在时自动新建数据库文件
    if (!(IsBooksDataExist())) {
        fp = fopen(_db_BOOKS, "wb");
        fclose(fp);
    }

    //打开数据库文件
    fp = fopen(_db_BOOKS, "rb");
    if (fp == NULL)
        return false;

    if ((fgetc(fp)) != EOF) {
        //定位文件起始位置
        rewind(fp);

        //读取头部图书信息总数量
        fread(&total, sizeof(unsigned int), 1, fp);
        if ((fgetc(fp)) != EOF) {
            fseek(fp, -1L, SEEK_CUR);
            //为链表分配数据
            for (int i = 0; i < total; i++) {
                //创建临时结构体以给链表分配数据
                Books books_temp;
                fread(&books_temp, sizeof(Books), 1, fp);
                AddBooksNode(books_temp, &(*books_list));
                memset(&books_temp, 0, sizeof(Books));
            }
        }
    }
    //关闭已经打开的文件数据库
    fclose(fp);
    return true;
}

bool UsersDataOpen(FILE *fp, UsersNodePtr *users_list) {
    unsigned int total = 0;

    //检测链表是否已满
    if (IsUsersListFull(&(*users_list)))
        return false;

    //检测数据库文件是否存在，不存在时自动新建数据库文件
    if (!(IsUsersDataExist())) {
        fp = fopen(_db_USERS, "wb");
        fclose(fp);
    }

    //打开数据库文件
    fp = fopen(_db_USERS, "rb");
    if (fp == NULL)
        return false;

    if ((fgetc(fp)) != EOF) {
        //定位文件起始位置
        rewind(fp);

        //读取头部用户信息总数量
        fread(&total, sizeof(unsigned int), 1, fp);
        if ((fgetc(fp)) != EOF) {
            fseek(fp, -1L, SEEK_CUR);
            //为链表分配数据
            for (int i = 0; i < total; i++) {
                //创建临时结构体以给链表分配数据
                Users users_temp;
                fread(&users_temp, sizeof(Users), 1, fp);
                AddUsersNode(users_temp, &(*users_list));
                memset(&users_temp, 0, sizeof(Users));
            }
        }
    }
    //关闭已经打开的文件数据库
    fclose(fp);
    return true;
}

//将链表信息写入数据库
bool BooksDataWrite(FILE *fp, const BooksNodePtr *books_list) {
    BooksNode *ptr = *books_list;
    unsigned int total = BooksNodeConut(&(*books_list));

    fp = fopen(_db_BOOKS, "wb");

    //写入头部图书数量信息
    fwrite(&total, sizeof(unsigned int), 1, fp);

    while (ptr != NULL) {
        fwrite(&ptr->book, sizeof(Books), 1, fp);
        ptr = ptr->next;
    }
    //刷新缓冲区，写入数据
    fflush(fp);
    //关闭已经打开的文件数据库
    fclose(fp);
    return true;
}

bool UsersDataWrite(FILE *fp, const UsersNodePtr *users_list) {
    UsersNode *ptr = *users_list;
    unsigned int total = UsersNodeConut(&(*users_list));

    fp = fopen(_db_USERS, "wb");

    //写入头部用户数量信息
    fwrite(&total, sizeof(unsigned int), 1, fp);

    while (ptr != NULL) {
        fwrite(&ptr->user, sizeof(Users), 1, fp);
        ptr = ptr->next;
    }
    //刷新缓冲区，写入数据
    fflush(fp);
    //关闭已经打开的文件数据库
    fclose(fp);
    return true;
}