#pragma once

// CPacketSession을 상속받은 CConnectedSession
class JYConnectedSession : public JYPacketSession
{
public:
	JYConnectedSession(VOID);
	virtual ~JYConnectedSession(VOID);

	// 개체를 종료하고 다시 Accept상태로 만들어주는 함수
	// CServerIocp에서 OnIoDisconnected가 되었을때 개체를 종료하고 다시 초기화 할때 사용하게 됩니다.
	BOOL Restart(SOCKET hListenSocket);

	// m_bConnected의 값을 설정하고 가져오는 함수
	VOID SetConnected(BOOL bConnected);
	BOOL GetConnected(VOID);

private:
	// 접속되었는지 판별하는 변수
	BOOL m_bConnected;

	char userID[32];

public:
	VOID SetUserID(string id);
	string GetUserID(VOID);
};
