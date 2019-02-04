#pragma once


class JYGameDataBase
{
public:
	JYGameDataBase(VOID);
	~JYGameDataBase(VOID);

	BOOL InitDB();

	//��ġ üũ ����
	BOOL CheckQuery(string ID, string PW);
	////�Է� ����
	//BOOL InsertQuery(string ID, string PW, string NICK);

	BOOL UpdateDCheck(string Nick);
	BOOL ChangeRate(string winner, string loser);
	BOOL SelectRate(string player1, int* winRate,int *loseRate);


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

extern JYGameDataBase * GDBSession;
