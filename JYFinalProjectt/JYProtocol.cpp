// 패킷 처리를 위해서 새로 작성한 ProcProtocol.cpp
#include "stdafx.h"
#include "JYServerIocp.h"


// 로그인 처리
VOID JYServerIocp::PROC_PT_LOGIN(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	S_PT_LOGIN data;
	READ_PT_LOGIN(pPacket, data);
	
	S_PT_LOGIN_ERROR errData;
	errData.error = 1;

	//id pw 체크
	if (GDBSession->CheckQuery(data.USER_ID, data.USER_PW,&errData.error))
	{
		S_PT_LOGIN_SUCC succData;

		GDBSession->SelectQuery(data.USER_ID, data.USER_PW, succData.NICKNAME);

		JYLog::WriteLog(LPTSTR(L"\nUSER ID : %S \n"), data.USER_ID);

		pConnectedSession->SetUserID(data.USER_ID);

		pConnectedSession->WritePacket(PT_LOGIN_SUCC, WriteBuffer, WRITE_PT_LOGIN_SUCC(WriteBuffer, succData));

		return;
	}
	else if (!GDBSession->CheckQuery(data.USER_ID, data.USER_PW, &errData.error))
	{
		printf("PROC_PT_LOGIN ERROR\n");

		printf("%d",errData.error);
		//S_PT_LOGIN_ERROR errData;
		pConnectedSession->WritePacket(PT_LOGIN_ERROR, WriteBuffer, WRITE_PT_LOGIN_ERROR1(WriteBuffer, errData.error));
		return;
	}
}

// 회원가입 처리
VOID JYServerIocp::PROC_PT_MAKING_USER(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	S_PT_MAKING data;
	READ_PT_MAKING(pPacket, data);

	//id,pw,nick 삽입
	if (GDBSession->InsertQuery(data.USER_ID, data.USER_PW, data.NICKNAME))
	{
		printf("PROC_PT_MAKING_USER SUCCESS\n");
		cout << "ID : " << data.USER_ID << endl;
		cout << "PW : " << data.USER_PW << endl;
		cout << "NICKNAME : " << data.NICKNAME << endl;

		//성공 패킷 보내주기
		S_PT_MAKING_SUCC msdata;
		//받은 클라에게만 writePacket하기
		pConnectedSession->WritePacket(PT_MAKING_SUCC, WriteBuffer, WRITE_PT_MAKING_SUCC(WriteBuffer, msdata));
		printf("send MAKING_SUCC\n");
	}
	else if (!GDBSession->InsertQuery(data.USER_ID, data.USER_PW, data.NICKNAME))
	{
		S_PT_MAKING_ERROR medata;
		pConnectedSession->WritePacket(PT_MAKING_ERROR, WriteBuffer, WRITE_PT_MAKING_ERROR(WriteBuffer, medata));
		printf("send MAKING_ERROR\n");

	}
}

VOID JYServerIocp::PROC_PT_MAKING_SUCC(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };


	//성공 패킷 보내주기
	S_PT_MAKING_SUCC msdata;
	//받은 클라에게만 writePacket하기
	pConnectedSession->WritePacket(PT_MAKING_SUCC, WriteBuffer, WRITE_PT_MAKING_SUCC(WriteBuffer, msdata));
	printf("send MAKING_SUCC\n");
}

