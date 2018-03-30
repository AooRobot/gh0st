#pragma once

#include <mysql.h>  //包含MySql的头文件
#include <string>

typedef struct
{
	std::string host;     //IP
	std::string port;     //端口
	std::string Db;       //数据库名称
	std::string user;     //账号
	std::string passwd;   //密码
	std::string charset;  //字节集
	std::string Msg;      //消息
}CONNECT_INFO;

class CMySqlMng
{
public:

	CMySqlMng()
		:m_bIsConnected(false)
	{
	}

	~CMySqlMng()
	{
	}

	//连接
	int Connect(CONNECT_INFO& connectinfo)  //防止拷贝构造 
	{
		int value = 0;
		do 
		{
			//初始化
			if (mysql_init(&m_mysql) == NULL)
			{
				connectinfo.Msg = "初始化错误";
				value = 1;
				break;
			}
			//连接数据库
			if (mysql_real_connect(&m_mysql,
				connectinfo.host.c_str(),
				connectinfo.user.c_str(),
				connectinfo.passwd.c_str(),
				connectinfo.Db.c_str(),0,NULL,0) == NULL)
			{
				connectinfo.Msg = "连接数据库失败";
				value = 1;
				break;
			}
			//字节集
			if (mysql_set_character_set(&m_mysql,connectinfo.charset.c_str()) != 0)
			{
				connectinfo.Msg = "设置字节集失败";
				value = 1;
				break;
			}

			m_bIsConnected = true;

		} while (0); //只执行一次

		return value;
	}

	//断开和数据库的连接
	void DisConnect()
	{
		if (m_bIsConnected)
		{
			mysql_close(&m_mysql);
			m_bIsConnected = false;
		}
	}

	bool IsConnect()
	{
		return m_bIsConnected;
	}

	//注册
	bool InsertUser(const char *username, const char *password)
	{
		CString sql;
		sql.Format("INSERT INTO user (username,password) values (%s,%s)", username, password);

		if (mysql_query(&m_mysql,sql.GetString()) == 0)
		{
			return true;
		}
		else
		{
			return  false;
		}
	}

	//查询数据库
	bool SelectUser(const char *username,const char *password)
	{
		CString sql;
		sql.Format("SELECT COUNT(*) FROM user WHERE username = '%s' AND password = '%s'", username, password);

		if (mysql_query(&m_mysql, sql.GetString()) == 0)
		{
			return true;
		}
		else
		{
			return  false;
		}
	}
private:
	MYSQL m_mysql;  //成员变量

	bool m_bIsConnected;
	
};

