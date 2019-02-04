#pragma once


// CIocp를 상속 받습니다.
class JYServerIocp : public JYIocp
{
public:
	JYServerIocp(VOID);
	virtual ~JYServerIocp(VOID);

	// 개체를 초기화 하고 시작하기 위한 함수
	BOOL Begin(VOID);

	// 개체를 종료하기 위한 함수
	VOID End(VOID);

	// KeepAlive Thread Callback 함수
	VOID KeepThreadCallback(VOID);

protected:
	// CIocp 상속 가상함수들을 재선언 합니다.
	VOID OnIoRead(VOID *pObject, DWORD dwDataLength);
	VOID OnIoWrote(VOID *pObject, DWORD dwDataLength);
	VOID OnIoConnected(VOID *pObject);
	VOID OnIoDisconnected(VOID *pObject);

private:
	// 서버의 Listen을 담당할 개체
	JYNetwork * m_pListen;
	JYCSManager m_oConnectedSessionManager;

	// KeepAlive Thread를 관리하는 핸들값
	HANDLE m_hKeepThread;
	// KeepAlive Thread를 종료할때 사용하는 이벤트
	HANDLE m_hKeepThreadDestroyEvent;

private:
	// 패킷 처리를 할 함수들
	VOID PROC_PT_LOGIN(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_MAKING_USER(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
	VOID PROC_PT_MAKING_SUCC(JYConnectedSession *pConnectedSession, DWORD dwProtocol, BYTE *pPacket, DWORD dwPacketLength);
};
