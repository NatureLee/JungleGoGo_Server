#include "stdafx.h"
#include "JYConnectedSession.h"

JYConnectedSession::JYConnectedSession(VOID)
{
	// ���� �Ǿ����� �Ǻ��ϴ� ����
	m_bConnected = FALSE;
}

JYConnectedSession::~JYConnectedSession(VOID)
{
}

BOOL JYConnectedSession::Restart(SOCKET hListenSocket)
{
	// ����ȭ ��ü
	JYThreadSync Sync;

	// ��ü�� ������ �ݴϴ�.
	End();

	// ��ü�� �ٽ� �����ϰ� Accept���·� ����� �ݴϴ�.
	// Accept�� �ϱ����ؼ��� �ݵ�� hListenSocket ���� �ʿ��մϴ�.
	// �� ���� CServerIocp�� ����� m_pListen�� GetSocket���� �޾ƿ� �� �ֽ��ϴ�.
	return Begin() && Accept(hListenSocket);
}

// m_bConnected�� ���� �����ϰ� �������� �Լ�
VOID JYConnectedSession::SetConnected(BOOL bConnected)
{
	JYThreadSync Sync; m_bConnected = bConnected;
}

BOOL JYConnectedSession::GetConnected(VOID)
{
	JYThreadSync Sync;

	return m_bConnected;
}


VOID JYConnectedSession::SetUserID(string id)
{
	strcpy(userID, id.c_str());
}

string JYConnectedSession::GetUserID(VOID)
{
	string str(userID);
	return str;
}