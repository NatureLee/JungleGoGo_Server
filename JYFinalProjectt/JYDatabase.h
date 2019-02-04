#pragma once

class JYDataBase
{
public:
	JYDataBase(VOID);
	~JYDataBase(VOID);

	BOOL InitDB();

	//��ġ üũ ����
	BOOL CheckQuery(string ID, string PW, int * error);
	//�Է� ����
	BOOL InsertQuery(string ID, string PW, string NICK);

	BOOL SelectQuery(string ID, string PW, char * NICK);

	BOOL UpdateDCheck(string ID);

private:

	const char * DB_HOST; //DB ȣ��Ʈ
	const char * DB_NAME; //DB �̸�.
	const char * DB_SOCK; //DB ����
	const char * DB_USER; //DB �����
	const char * DB_PASS; //DB ��й�ȣ

	enum {
		DB_PORT = 3306,	//DB ��Ʈ
		DB_OPT = 0		//DB �ɼ�
	};

	MYSQL * conn;	//DB�� �����ϴ� ��ü
	MYSQL_RES *res;	//���� ���
	MYSQL_ROW row;	//������� ��Ÿ�� ���ڿ�

};

extern JYDataBase * GDBSession;
