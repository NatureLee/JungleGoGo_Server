#pragma once

#include "stdafx.h"


JYGameDataBase *GDBSession = new JYGameDataBase();

JYGameDataBase::JYGameDataBase(VOID)
{
	DB_HOST = "127.0.0.1";
	DB_USER = "root";
	DB_SOCK = NULL;
	DB_NAME = "userdb";
	DB_PASS = "wkdus83400";
}

JYGameDataBase::~JYGameDataBase(VOID)
{
	mysql_free_result(res);
	mysql_close(conn);
}

BOOL JYGameDataBase::InitDB()
{
	conn = mysql_init(NULL);

	//DB¿¡ ¿¬°á.
	if (!mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_PORT, DB_SOCK, DB_OPT))
	{
		cout << "DB Connect Error : " << mysql_error(conn) << endl;
		return false;
	}
	//DBÀÇ Table È®ÀÎ.
	if (mysql_query(conn, "SHOW TABLES"))
	{
		printf("DB Query Error : %s\n", mysql_error(conn));
		//cout << "DB Query Error : " << mysql_error(conn) << endl;
		return false;
	}
	//Äõ¸®°á°ú ÀúÀå.
	res = mysql_use_result(conn);

	printf("DB show Tables in %s \n", DB_NAME);
	//cout << "** DB Show Tables in " << DB_NAME << " **" << endl;

	//Äõ¸® °á°ú È®ÀÎ.
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		cout << row[0] << endl;
	}
	return TRUE;
}

BOOL JYGameDataBase::UpdateDCheck(string Nick) 
{
	string insertQuery = "UPDATE USERINFO SET connect = '" + to_string(1) + "' WHERE userNICK = '" + Nick + "'";

	//Äõ¸® Àü´Þ
	if (mysql_query(conn, insertQuery.c_str()) != NULL) {

		printf("UPDATE Error : %s \n", mysql_error(conn));

		return FALSE;
	}

	return TRUE;
}

BOOL JYGameDataBase::ChangeRate(string winner, string loser)
{
	int winRate;
	int loseRate;

	string insertQuery1 = "select win from userinfo where usernick = '" + winner +"'";

	//Äõ¸® Àü´Þ
	if (mysql_query(conn, insertQuery1.c_str()) != NULL) {
		JYLog::WriteLog(LPTSTR(L"ChangeRate Error: %S\n"), mysql_error(conn));

		return FALSE;
	}
	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL) {
		winRate = atoi(row[0]);
	}

	winRate += 1;

    insertQuery1 = "UPDATE USERINFO SET win = '" + to_string(winRate) + "' WHERE userNICK = '" + winner + "'";

	//Äõ¸® Àü´Þ
	if (mysql_query(conn, insertQuery1.c_str()) != NULL) {

		printf("ChangeRate Error : %s \n", mysql_error(conn));

		return FALSE;
	}


	//Change loseRate//
	string insertQuery2 = "select lose from userinfo where usernick = '" + loser + "'";

	//Äõ¸® Àü´Þ
	if (mysql_query(conn, insertQuery2.c_str()) != NULL) {
		JYLog::WriteLog(LPTSTR(L"ChangeRate Error: %S\n"), mysql_error(conn));

		return FALSE;
	}
	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL) {
		loseRate = atoi(row[0]);
	}

	loseRate += 1;

	insertQuery2 = "UPDATE USERINFO SET lose = '" + to_string(loseRate) + "' WHERE userNICK = '" + loser + "'";

	//Äõ¸® Àü´Þ
	if (mysql_query(conn, insertQuery2.c_str()) != NULL) {

		printf("ChangeRate Error : %s \n", mysql_error(conn));

		return FALSE;
	}

	return TRUE;

}


BOOL JYGameDataBase::SelectRate(string player,int *winRate,int *loseRate)
{

	string insertQuery1 = "select win,lose from userinfo where usernick = '" + player + "'";

	//Äõ¸® Àü´Þ
	if (mysql_query(conn, insertQuery1.c_str()) != NULL) {
		JYLog::WriteLog(LPTSTR(L"SelectRate Error: %S\n"), mysql_error(conn));

		return FALSE;
	}
	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res)) != NULL) {
		*winRate = atoi(row[0]);
		*loseRate = atoi(row[1]);
	}

	return TRUE;
}



