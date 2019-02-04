#include "stdafx.h"
#include "JYServerIocp.h"

// KeepAlive Thread Callback 함수
DWORD WINAPI KeepThreadCallback(LPVOID pParameter)
{
	JYServerIocp *pOwner = (JYServerIocp*)pParameter;

	pOwner->KeepThreadCallback();

	return 0;
}

JYServerIocp::JYServerIocp(VOID)
{
}

JYServerIocp::~JYServerIocp(VOID)
{
}

VOID JYServerIocp::KeepThreadCallback(VOID)
{
	// KeepAlive 신호
	DWORD dwKeepAlive = 0xFFFF;

	while (TRUE)
	{
		// 30초동안 종료 이벤트가 발생하지 않으면 넘어갑니다.
		DWORD dwResult = WaitForSingleObject(m_hKeepThreadDestroyEvent, 30000);

		// 종료 이벤트가 발생했을경우 Thread를 종료합니다.
		if (dwResult == WAIT_OBJECT_0) return;

		// 접속해 있는 모든 Session에 패킷을 전송합니다.
		//m_oConnectedSessionManager.WriteAll(0x3000000, (BYTE*)&dwKeepAlive, sizeof(DWORD));
	}
}

// CIocp의 가상함수들
VOID JYServerIocp::OnIoConnected(VOID *pObject)
{
	// 여기 가상함수에서 넘어온 pObject는 접속을 담당한 개체가 넘어오게 됩니다.
	// 이것을 여기서 클라이언트를 관리할 CConnectedSession으로 형변환을 해 주어 받게 됩니다.
	// 형변환은 reinterpret_cast를 사용합니다.
	JYConnectedSession *pConnectedSession = reinterpret_cast<JYConnectedSession*>(pObject);
	JYLog::WriteLog(LPTSTR(L"\n---- Connected LoginServer ----\n"));


	// 접속한 개체의 IO를 IOCP를 통해서 받기위해서 IOCP에 등록하는 과정을 거치게 됩니다.
	// 여기서 GetSocket을 이용해서 소켓을 등록하고 해당 키는 개체의 포인터를 이용하게 됩니다.
	if (!JYIocp::RegisterSocketToIocp(pConnectedSession->GetSocket(), reinterpret_cast<ULONG_PTR>(pConnectedSession)))
		return;

	// IOCP 초기 받기를 실행해 줍니다.
	if (!pConnectedSession->InitializeReadForIocp())
	{
		// 만일 실패했을 경우 개체를 재시작해 줍니다.
		pConnectedSession->Restart(m_pListen->GetSocket());
		return;
	}
	pConnectedSession->SetConnected(TRUE);
}

VOID JYServerIocp::OnIoDisconnected(VOID *pObject)
{
	// 여기 가상함수에서 넘어온 pObject는 OnIoConnected와 마찬가지로 접속을 담당한 개체가 넘어오게 됩니다.
	// 이것을 여기서 클라이언트를 관리할 CConnectedSession으로 형변환을 해 주어 받게 됩니다.
	// 형변환은 reinterpret_cast를 사용합니다.
	JYLog::WriteLog(LPTSTR(L"\n--------Disconnected Session--------\n"));

	JYConnectedSession *pConnectedSession = reinterpret_cast<JYConnectedSession*>(pObject);

	GDBSession->UpdateDCheck(pConnectedSession->GetUserID());

	// 접속을 종료하였기 때문에 개체를 재시작해 줍니다.
	pConnectedSession->Restart(m_pListen->GetSocket());

	pConnectedSession->SetConnected(FALSE);
}

VOID JYServerIocp::OnIoRead(VOID *pObject, DWORD dwDataLength)
{
	//printf("Read Packet SUCCESS\n");
	// 여기 가상함수에서 넘어온 pObject는 OnIoConnected와 마찬가지로 접속을 담당한 개체가 넘어오게 됩니다.
	// 이것을 여기서 클라이언트를 관리할 CConnectedSession으로 형변환을 해 주어 받게 됩니다.
	// 형변환은 reinterpret_cast를 사용합니다.
	JYConnectedSession *pConnectedSession = reinterpret_cast<JYConnectedSession*>(pObject);

	// 받은 프로토콜과 패킷 길이를 저장하는 변수
	DWORD dwProtocol = 0, dwPacketLength = 0;
	// 패킷을 저장하는 변수
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	// CNetworkSession에서 CPacketSession으로 데이터를 가져옵니다.
	if (pConnectedSession->ReadPacketForIocp(dwDataLength))
	{
		//printf("Read Packet Length SUCCESS\n");
		//cout <<"DATA LENGTH : " << dwDataLength <<endl;
		//cout<<"Packet Number :	"<< 

		// CPacketSession에서 패킷을 뽑아 냅니다.
		while (pConnectedSession->GetPacket(dwProtocol, Packet, dwPacketLength))
		{
			// 프로토콜에 따른 switch 문
			switch (dwProtocol)
			{
				// 사용자 등록 프로토콜일 경우
			case PT_LOGIN:
				PROC_PT_LOGIN(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
				// 사용자 검색 프로토콜일 경우
			case PT_MAKING:
				PROC_PT_MAKING_USER(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;

			case PT_MAKING_SUCC:
				break;
			}
		}
	}

	if (!pConnectedSession->InitializeReadForIocp())
		pConnectedSession->Restart(m_pListen->GetSocket());
}

VOID JYServerIocp::OnIoWrote(VOID *pObject, DWORD dwDataLength)
{
	JYConnectedSession *pConnectedSession = reinterpret_cast<JYConnectedSession*>(pObject);

}

BOOL JYServerIocp::Begin(VOID)
{
	// 상속 받은 IOCP 개체를 시작합니다.
	if (!JYIocp::Begin())
		return FALSE;

	// Listen에 사용할 개체를 생성합니다.
	m_pListen = new JYNetwork();

	//printf("JYNetwork SUCCESS\n");
	// 개체를 시작합니다.
	if (!m_pListen->Begin())
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		JYServerIocp::End();
		return FALSE;
	}
	// TCP로 사용할 것을 선언합니다.
	//printf("Bind SUCCESS\n");
	if (!m_pListen->TcpBind())
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		JYServerIocp::End();

		return FALSE;
	}

	// 포트 1820으로 Listen을 하고 최대 100명으로 설정합니다.
	//printf("Lisetn SUCCESS \n");
	if (!m_pListen->Listen(DEFAULT_PORT, MAX_USER))
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		JYServerIocp::End();

		return FALSE;
	}

	// IOCP에 Listen 소켓을 등록시켜 줍니다. 그리고 키로는 해당 개체의 포인터를 설정합니다.
	//printf("RegisterSocket SUCCESS\n");
	if (!JYIocp::RegisterSocketToIocp(m_pListen->GetSocket(), reinterpret_cast<ULONG_PTR>(m_pListen)))
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		JYServerIocp::End();

		return FALSE;
	}

	// CConnectedSessionManager를 시작합니다.
	// 시작 함수내에는 CConnectedSession을 생성하고 Accept상태로 만드는 코드가 추가되어 있습니다.
	//printf("ConnectedSession Manager Start\n");
	if (!m_oConnectedSessionManager.Begin(m_pListen->GetSocket()))
	{
		JYServerIocp::End();

		return FALSE;
	}

	// KeepAlive 종료 이벤트를 생성합니다.
	//printf("KeepAlive Event Create\n");
	m_hKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hKeepThreadDestroyEvent)
	{
		// 실패했을경우 종료합니다.
		JYServerIocp::End();
		return FALSE;
	}

	// KeepAlive Thread를 생성합니다.
	//printf("KeepAlive Thread Create\n");
	m_hKeepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!m_hKeepThread)
	{
		// 생성 실패했을 경우 종료합니다.
		JYServerIocp::End();
		return FALSE;
	}


	//printf("InitDB SUCCESS\n");
	if (!GDBSession->InitDB())
	{
		// 실패했을 경우 End함수를 호출하고 종료합니다.
		JYServerIocp::End();

		return FALSE;
	}


	return TRUE;
}

VOID JYServerIocp::End(VOID)
{
	// CServerIocp를 종료할때

	// m_hKeepThread가 NULL이 아니면 KeepAlive Thread를 종료해 줍니다.
	if (m_hKeepThread)
	{
		// 종료 이벤트를 발생시킵니다.
		SetEvent(m_hKeepThreadDestroyEvent);

		// Thread가 종료될 때까지 대기합니다.
		WaitForSingleObject(m_hKeepThread, INFINITE);

		// 핸들값을 닫아줍니다.
		CloseHandle(m_hKeepThread);
		m_hKeepThread = NULL;
	}

	// KeepAlive 종료 이벤트가 NULL이 아니면
	if (m_hKeepThreadDestroyEvent)
	{
		// 이벤트를 닫아줍니다.
		CloseHandle(m_hKeepThreadDestroyEvent);
		m_hKeepThreadDestroyEvent = NULL;
	}

	// IOCP를 종료합니다.
	JYIocp::End();

	// CConnectedSessionManager를 종료합니다.
	m_oConnectedSessionManager.End();

	// 생성했던 Listen용 개체를 종료하고 삭제해 줍니다.
	if (m_pListen)
	{
		m_pListen->End();

		delete m_pListen;
	}
}