#include "stdafx.h"
#include "JYConnectedSession.h"

JYConnectedSession::JYConnectedSession(VOID)
{
	// 접속 되었는지 판별하는 변수
	m_bConnected = FALSE;
}

JYConnectedSession::~JYConnectedSession(VOID)
{
}

BOOL JYConnectedSession::Restart(SOCKET hListenSocket)
{
	// 동기화 개체
	JYThreadSync Sync;

	// 개체를 종료해 줍니다.
	End();

	// 개체를 다시 시작하고 Accept상태로 만들어 줍니다.
	// Accept를 하기위해서는 반드시 hListenSocket 값이 필요합니다.
	// 이 값은 CServerIocp의 멤버인 m_pListen의 GetSocket으로 받아올 수 있습니다.
	return Begin() && Accept(hListenSocket);
}

// m_bConnected의 값을 설정하고 가져오는 함수
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