# 图书管理系统—单链表版

## 编译 && 运行  
```c
make
make clean
./LibraryManager.exe 
```
p.s. 在Windows 10下使用：  
```
gcc version 10.1.0 (Rev2, Built by MSYS2 project)
GNU Make 4.3 (Built for x86_64-pc-msys)
powershell
```
编译/测试运行通过  
## 测试数据

### 主程序：LibraryManager.exe 
### 用户信息:

```
-- Steven 用户ID：2019110300000 --         
用户类别：1(管理员)                                
用户密码：0000                          

-- Zhang San 用户ID：2019110300001 --      
用户类别：3(禁止登录)                            
用户密码：1234                                                  
  
-- Yang 用户ID：2019110300002 --           
用户类别：2(普通用户)                              
用户密码：4567
```

### 书籍信息 
按以下方式排列： 
图书数量--图书名称--图书类别--图书id--图书作者
```
<---------------------------------->  
5   
Wuthering Heights   
noval   
id00001   
Emily Bronte  
<---------------------------------->  
5  
The Pigeon Tunnel  
biography  
id00002  
John le Carre  
<---------------------------------->  
5   
Jane Eyre  
noval  
id00003  
Charlotte Bronte   
<---------------------------------->  
3  
Time Travel  
noval  
id00004   
James Gleick  
<---------------------------------->  
3  
To the Lighthouse  
noval   
id00005  
Virginia Woolf  
<---------------------------------->  
4  
The Alps  
travel notes  
id00006  
Stephen O'Shea  
<---------------------------------->   
5  
Surely You're Joking Mr Feynman  
biography  
id00007  
R.P.Feynman  
<---------------------------------->  
4  
Oracle Bones  
non-fiction  
id00008  
Peter Hessler  
<---------------------------------->  
2  
Science Readers  
science  
id00009  
Scholastic  
<---------------------------------->  
3  
Oxford Read and Discover  
learning  
id00010  
Oxford University Press  
<---------------------------------->  
```
