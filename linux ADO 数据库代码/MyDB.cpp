/************************************************************************* 
	> File Name: MyDB.cpp 
	> Author: SongLee 
	> E-mail: lisong.shine@qq.com 
	> Created Time: 2014��05��04�� ������ 23ʱ37��08�� 
	> Personal Blog: http://songlee24.github.io 
 ************************************************************************/  
#include<iostream>  
#include<cstdlib>  
#include "MyDB.h"  
using namespace std;  
  
MyDB::MyDB()  
{  
	connection = mysql_init(NULL); // ��ʼ�����ݿ����ӱ���  
	if(connection == NULL)  
	{  
		cout << "Error:" << mysql_error(connection);  
		exit(1);  
	}  
}  
  
MyDB::~MyDB()  
{  
	if(connection != NULL)  // �ر����ݿ�����  
	{  
		mysql_close(connection);  
	}  
}  
  
bool MyDB::initDB(string host, string user, string pwd, string db_name)  
{  
	// ����mysql_real_connect����һ�����ݿ�����  
	// �ɹ�����MYSQL*���Ӿ����ʧ�ܷ���NULL  
	connection = mysql_real_connect(connection, host.c_str(),  
			user.c_str(), pwd.c_str(), db_name.c_str(), 0, NULL, 0);  
	if(connection == NULL)  
	{  
		cout << "Error:" << mysql_error(connection);  
		exit(1);  
	}  
	return true;  
}  
  
bool MyDB::exeSQL(string sql)  
{  
	// mysql_query()ִ�гɹ�����0��ʧ�ܷ��ط�0ֵ����PHP�в�һ��  
	if(mysql_query(connection, sql.c_str()))  
	{  
		cout << "Query Error:" << mysql_error(connection);  
		exit(1);  
	}  
	else  
	{  
		result = mysql_use_result(connection); // ��ȡ�����  
		// mysql_field_count()����connection��ѯ������  
		for(int i=0; i < mysql_field_count(connection); ++i)  
		{  
			// ��ȡ��һ��  
			row = mysql_fetch_row(result);  
			if(row <= 0)  
			{  
				break;  
			}  
			// mysql_num_fields()���ؽ�����е��ֶ���  
			for(int j=0; j < mysql_num_fields(result); ++j)  
			{  
				cout << row[j] << " ";  
			}  
			cout << endl;  
		}  
		// �ͷŽ�������ڴ�  
		mysql_free_result(result);  
	}  
	return true;  
}  
