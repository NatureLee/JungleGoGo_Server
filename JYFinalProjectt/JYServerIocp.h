#pragma once


// CIocp�� ��� �޽��ϴ�.
class JYServerIocp : public JYIocp
{
public:
	JYServerIocp(VOID);
	virtual ~JYServerIocp(VOID);

	// ��ü�� �ʱ�ȭ �ϰ� �����ϱ� ���� �Լ�
	BOOL Begin(VOID);

	// ��ü�� �����ϱ� ���� �Լ�
	VOID End(VOID);

	// KeepAlive Thread Callback �Լ�
	VOID KeepThreadCallback(VOID);

protected:
	// CIocp ��� �����Լ����� �缱�� �մϴ�.
	VOID OnIoRead(VOID *pObject, DWORD dwDataLength);
	VOID OnIoWrote(VOID *pObject, DWORD dwDataLength);
	VOID OnIoConnected(VOID *pObject);
	VOID OnIoDisconnected(VOID *pObject);

private:
	// ������ Listen�� ����� ��ü
	JYNetwork * m_pListen;
	JYCSManager m_oConnectedSessionManager;

	// KeepAlive Thread�� �����ϴ� �ڵ鰪
	HANDLE m_hKeepThread;
	// KeepAlive Thread�� �����Ҷ� ����ϴ� �̺�Ʈ
	HANDLE m_hKeepThreadDestroyEvent;

private:
	// ��Ŷ ó���� �� �Լ���
	VOID PROC_PT_LOGIN(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_MAKING_USER(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_MAKING_SUCC(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
};
