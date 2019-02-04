#pragma once

// CPacketSession�� ��ӹ��� CConnectedSession
class JYConnectedSession : public JYPacketSession
{
public:
	JYConnectedSession(VOID);
	virtual ~JYConnectedSession(VOID);

	// ��ü�� �����ϰ� �ٽ� Accept���·� ������ִ� �Լ�
	// CServerIocp���� OnIoDisconnected�� �Ǿ����� ��ü�� �����ϰ� �ٽ� �ʱ�ȭ �Ҷ� ����ϰ� �˴ϴ�.
	BOOL Restart(SOCKET hListenSocket);

	// m_bConnected�� ���� �����ϰ� �������� �Լ�
	VOID SetConnected(BOOL bConnected);
	BOOL GetConnected(VOID);

private:
	// ���ӵǾ����� �Ǻ��ϴ� ����
	BOOL m_bConnected;

	char userID[32];

public:
	VOID SetUserID(string id);
	string GetUserID(VOID);
};
