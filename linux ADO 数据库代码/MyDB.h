/************************************************************************* 
    > File Name: MyDB.h 
    > Author: SongLee 
    > E-mail: lisong.shine@qq.com 
    > Created Time: 2014��05��04�� ������ 23ʱ25��50�� 
    > Personal Blog: http://songlee24.github.io 
 ************************************************************************/  
#ifndef __MYBD_H__  
#include<iostream>  
#include<string>  
#include<mysql/mysql.h>  
using namespace std;  
  
class MyDB  
{  
public:  
    MyDB();  
    ~MyDB();  
    bool initDB(string host, string user, string pwd, string db_name);  
    bool exeSQL(string sql);  
private:  
    MYSQL *connection;  
    MYSQL_RES *result;  
    MYSQL_ROW row;  
};  
  
#endif
