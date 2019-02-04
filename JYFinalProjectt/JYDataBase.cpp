#pragma once


#include "stdafx.h"
#include "JYDataBase.h"


JYDataBase *GDBSession = new JYDataBase();

JYDataBase::JYDataBase(VOID)
{
	DB_HOST = "127.0.0.1";
	DB_USER = "root";
	DB_SOCK = NULL;
	DB_NAME = "userdb";
	DB_PASS = "wkdus83400";
}

JYDataBase::~JYDataBase(VOID)
{
	mysql_free_result(res);
	mysql_close(conn);
}

BOOL JYDataBase::InitDB()
{
	conn = mysql_init(NULL);

	//DB에 연결.
	if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, DB_SOCK, DB_OPT))
	{
		cout << "DB Connect Error : " << mysql_error(conn) << endl;
		return false;
	}
	//DB의 Table 확인.
	if (mysql_query(conn, "SHOW TABLES"))
	{
		printf("DB Query Error : %s\n", mysql_error(conn));
		//cout << "DB Query Error : " << mysql_error(conn) << endl;
		return false;
	}
	//쿼리결과 저장.
	res = mysql_use_result(conn);

	printf("DB show Tables in %s \n", DB_NAME);

	//쿼리 결과 확인.
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		cout << row[0] << endl;
	}
	return TRUE;
}

BOOL JYDataBase::CheckQuery(string ID, string PW, int * error)
{
	//사용할 쿼리를 문자열로 합성.
	string checkQuery = "SELECT * FROM USERINFO WHERE userID = '" + ID + "' AND userPW = '" + PW + "'";

	if (mysql_query(conn, checkQuery.c_str()) != NULL)
	{
		JYLog::WriteLog(LPTSTR(L"\nQuery Error : : %S\n"), mysql_error(conn));
		return FALSE;
	}

	//결과 저장
	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL) {
		if (row[0] == ID) {	//ID와 일치??
			if (row[1] == PW)	//PW와 일치??
			{
				if (row[5] == to_string(0))  //중복 로그인
					break;

				else 
				{
					*error = 2;
					return false;
				}
			}
			else 
				return false;
			
		}
		else 
     		return false;
		
		if (row == NULL)
			return false;
	}

	// 결과값 중복체크
	if (res->row_count == 0) {
		return false;
	}

	return TRUE;
}

//DB에 삽입
BOOL JYDataBase::InsertQuery(string ID, string PW, string NICK)
{
	//문자열 합성
	string query = "INSERT INTO USERDB.USERINFO (userID,userPW,userNICK) VALUES ('";
	string insertQuery = query + ID + "' , '" + PW + "' , '" + NICK + "')";

	//쿼리 전달
	if (mysql_query(conn, insertQuery.c_str()) != NULL) {

		JYLog::WriteLog(LPTSTR(L"InsertQuery Error : %S\n"), mysql_error(conn));


		return FALSE;
	}

	return TRUE;
}

BOOL JYDataBase::SelectQuery(string ID, string PW, char * NICK)
{
	//문자열 합성
	string insertQuery = "SELECT USERNICK FROM USERINFO WHERE USERID = '" + ID + "' AND USERPW = '" + PW + "'";

	//쿼리 전달
	if (mysql_query(conn, insertQuery.c_str()) != NULL) {
		JYLog::WriteLog(LPTSTR(L"Sign Up Error: %S\n"), mysql_error(conn));

		return FALSE;
	}

	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL) {
		strcpy(NICK, row[0]);
	}

	return TRUE;
}


BOOL JYDataBase::UpdateDCheck(string ID)
{
	string insertQuery = "UPDATE USERINFO SET connect = '" + to_string(0) + "' WHERE userID = '" + ID + "'";

	//쿼리 전달
	if (mysql_query(conn, insertQuery.c_str()) != NULL) {

		printf("UPDATE Error : %s \n", mysql_error(conn));

		return FALSE;
	}

	return TRUE;
}