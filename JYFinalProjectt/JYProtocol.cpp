// ��Ŷ ó���� ���ؼ� ���� �ۼ��� ProcProtocol.cpp
#include "stdafx.h"
#include "JYServerIocp.h"


// �α��� ó��
VOID JYServerIocp::PROC_PT_LOGIN(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	S_PT_LOGIN data;
	READ_PT_LOGIN(pPacket, data);
	
	S_PT_LOGIN_ERROR errData;
	errData.error = 1;

	//id pw üũ
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

// ȸ������ ó��
VOID JYServerIocp::PROC_PT_MAKING_USER(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	S_PT_MAKING data;
	READ_PT_MAKING(pPacket, data);

	//id,pw,nick ����
	if (GDBSession->InsertQuery(data.USER_ID, data.USER_PW, data.NICKNAME))
	{
		printf("PROC_PT_MAKING_USER SUCCESS\n");
		cout << "ID : " << data.USER_ID << endl;
		cout << "PW : " << data.USER_PW << endl;
		cout << "NICKNAME : " << data.NICKNAME << endl;

		//���� ��Ŷ �����ֱ�
		S_PT_MAKING_SUCC msdata;
		//���� Ŭ�󿡰Ը� writePacket�ϱ�
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


	//���� ��Ŷ �����ֱ�
	S_PT_MAKING_SUCC msdata;
	//���� Ŭ�󿡰Ը� writePacket�ϱ�
	pConnectedSession->WritePacket(PT_MAKING_SUCC, WriteBuffer, WRITE_PT_MAKING_SUCC(WriteBuffer, msdata));
	printf("send MAKING_SUCC\n");
}

