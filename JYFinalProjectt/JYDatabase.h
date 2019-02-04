#pragma once

class JYDataBase
{
public:
	JYDataBase(VOID);
	~JYDataBase(VOID);

	BOOL InitDB();

	//일치 체크 쿼리
	BOOL CheckQuery(string ID, string PW, int * error);
	//입력 쿼리
	BOOL InsertQuery(string ID, string PW, string NICK);

	BOOL SelectQuery(string ID, string PW, char * NICK);

	BOOL UpdateDCheck(string ID);

private:

	const char * DB_HOST; //DB 호스트
	const char * DB_NAME; //DB 이름.
	const char * DB_SOCK; //DB 소켓
	const char * DB_USER; //DB 사용자
	const char * DB_PASS; //DB 비밀번호

	enum {
		DB_PORT = 3306,	//DB 포트
		DB_OPT = 0		//DB 옵션
	};

	MYSQL * conn;	//DB와 연결하는 객체
	MYSQL_RES *res;	//쿼리 결과
	MYSQL_ROW row;	//결과값을 나타낼 문자열

};

extern JYDataBase * GDBSession;
