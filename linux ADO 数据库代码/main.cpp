/************************************************************************* 
	> File Name: main.cpp 
	> Author: SongLee 
	> E-mail: lisong.shine@qq.com 
	> Created Time: 2014��05��05�� ����һ 00ʱ30��45�� 
	> Personal Blog: http://songlee24.github.io 
	> ԭ�ĳ�����http://blog.csdn.net/lisonglisonglisong/article/details/25071793
 ************************************************************************/  
#include<iostream>  
#include "MyDB.h"  
using namespace std;  
  
int main()  
{  
	MyDB db;  
	db.initDB("localhost", "root", "songlee", "student");  
	db.exeSQL("select * from student_info");  
	return 0;  
}  
