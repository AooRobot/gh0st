#pragma once

#include <mysql.h>  //����MySql��ͷ�ļ�
#include <string>

typedef struct
{
	std::string host;     //IP
	std::string port;     //�˿�
	std::string Db;       //���ݿ�����
	std::string user;     //�˺�
	std::string passwd;   //����
	std::string charset;  //�ֽڼ�
	std::string Msg;      //��Ϣ
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

	//����
	int Connect(CONNECT_INFO& connectinfo)  //��ֹ�������� 
	{
		int value = 0;
		do 
		{
			//��ʼ��
			if (mysql_init(&m_mysql) == NULL)
			{
				connectinfo.Msg = "��ʼ������";
				value = 1;
				break;
			}
			//�������ݿ�
			if (mysql_real_connect(&m_mysql,
				connectinfo.host.c_str(),
				connectinfo.user.c_str(),
				connectinfo.passwd.c_str(),
				connectinfo.Db.c_str(),0,NULL,0) == NULL)
			{
				connectinfo.Msg = "�������ݿ�ʧ��";
				value = 1;
				break;
			}
			//�ֽڼ�
			if (mysql_set_character_set(&m_mysql,connectinfo.charset.c_str()) != 0)
			{
				connectinfo.Msg = "�����ֽڼ�ʧ��";
				value = 1;
				break;
			}

			m_bIsConnected = true;

		} while (0); //ִֻ��һ��

		return value;
	}

	//�Ͽ������ݿ������
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

	//ע��
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

	//��ѯ���ݿ�
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
	MYSQL m_mysql;  //��Ա����

	bool m_bIsConnected;
	
};

