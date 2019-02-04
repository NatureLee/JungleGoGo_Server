#include "stdafx.h"
#include "JYServerIocp.h"

// KeepAlive Thread Callback �Լ�
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
	// KeepAlive ��ȣ
	DWORD dwKeepAlive = 0xFFFF;

	while (TRUE)
	{
		// 30�ʵ��� ���� �̺�Ʈ�� �߻����� ������ �Ѿ�ϴ�.
		DWORD dwResult = WaitForSingleObject(m_hKeepThreadDestroyEvent, 30000);

		// ���� �̺�Ʈ�� �߻�������� Thread�� �����մϴ�.
		if (dwResult == WAIT_OBJECT_0) return;

		// ������ �ִ� ��� Session�� ��Ŷ�� �����մϴ�.
		//m_oConnectedSessionManager.WriteAll(0x3000000, (BYTE*)&dwKeepAlive, sizeof(DWORD));
	}
}

// CIocp�� �����Լ���
VOID JYServerIocp::OnIoConnected(VOID *pObject)
{
	// ���� �����Լ����� �Ѿ�� pObject�� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedSession���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	JYConnectedSession *pConnectedSession = reinterpret_cast<JYConnectedSession*>(pObject);
	JYLog::WriteLog(LPTSTR(L"\n---- Connected LoginServer ----\n"));


	// ������ ��ü�� IO�� IOCP�� ���ؼ� �ޱ����ؼ� IOCP�� ����ϴ� ������ ��ġ�� �˴ϴ�.
	// ���⼭ GetSocket�� �̿��ؼ� ������ ����ϰ� �ش� Ű�� ��ü�� �����͸� �̿��ϰ� �˴ϴ�.
	if (!JYIocp::RegisterSocketToIocp(pConnectedSession->GetSocket(), reinterpret_cast<ULONG_PTR>(pConnectedSession)))
		return;

	// IOCP �ʱ� �ޱ⸦ ������ �ݴϴ�.
	if (!pConnectedSession->InitializeReadForIocp())
	{
		// ���� �������� ��� ��ü�� ������� �ݴϴ�.
		pConnectedSession->Restart(m_pListen->GetSocket());
		return;
	}
	pConnectedSession->SetConnected(TRUE);
}

VOID JYServerIocp::OnIoDisconnected(VOID *pObject)
{
	// ���� �����Լ����� �Ѿ�� pObject�� OnIoConnected�� ���������� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedSession���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	JYLog::WriteLog(LPTSTR(L"\n--------Disconnected Session--------\n"));

	JYConnectedSession *pConnectedSession = reinterpret_cast<JYConnectedSession*>(pObject);

	GDBSession->UpdateDCheck(pConnectedSession->GetUserID());

	// ������ �����Ͽ��� ������ ��ü�� ������� �ݴϴ�.
	pConnectedSession->Restart(m_pListen->GetSocket());

	pConnectedSession->SetConnected(FALSE);
}

VOID JYServerIocp::OnIoRead(VOID *pObject, DWORD dwDataLength)
{
	//printf("Read Packet SUCCESS\n");
	// ���� �����Լ����� �Ѿ�� pObject�� OnIoConnected�� ���������� ������ ����� ��ü�� �Ѿ���� �˴ϴ�.
	// �̰��� ���⼭ Ŭ���̾�Ʈ�� ������ CConnectedSession���� ����ȯ�� �� �־� �ް� �˴ϴ�.
	// ����ȯ�� reinterpret_cast�� ����մϴ�.
	JYConnectedSession *pConnectedSession = reinterpret_cast<JYConnectedSession*>(pObject);

	// ���� �������ݰ� ��Ŷ ���̸� �����ϴ� ����
	DWORD dwProtocol = 0, dwPacketLength = 0;
	// ��Ŷ�� �����ϴ� ����
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	// CNetworkSession���� CPacketSession���� �����͸� �����ɴϴ�.
	if (pConnectedSession->ReadPacketForIocp(dwDataLength))
	{
		//printf("Read Packet Length SUCCESS\n");
		//cout <<"DATA LENGTH : " << dwDataLength <<endl;
		//cout<<"Packet Number :	"<< 

		// CPacketSession���� ��Ŷ�� �̾� ���ϴ�.
		while (pConnectedSession->GetPacket(dwProtocol, Packet, dwPacketLength))
		{
			// �������ݿ� ���� switch ��
			switch (dwProtocol)
			{
				// ����� ��� ���������� ���
			case PT_LOGIN:
				PROC_PT_LOGIN(pConnectedSession, dwProtocol, Packet, dwPacketLength);
				break;
				// ����� �˻� ���������� ���
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
	// ��� ���� IOCP ��ü�� �����մϴ�.
	if (!JYIocp::Begin())
		return FALSE;

	// Listen�� ����� ��ü�� �����մϴ�.
	m_pListen = new JYNetwork();

	//printf("JYNetwork SUCCESS\n");
	// ��ü�� �����մϴ�.
	if (!m_pListen->Begin())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		JYServerIocp::End();
		return FALSE;
	}
	// TCP�� ����� ���� �����մϴ�.
	//printf("Bind SUCCESS\n");
	if (!m_pListen->TcpBind())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		JYServerIocp::End();

		return FALSE;
	}

	// ��Ʈ 1820���� Listen�� �ϰ� �ִ� 100������ �����մϴ�.
	//printf("Lisetn SUCCESS \n");
	if (!m_pListen->Listen(DEFAULT_PORT, MAX_USER))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		JYServerIocp::End();

		return FALSE;
	}

	// IOCP�� Listen ������ ��Ͻ��� �ݴϴ�. �׸��� Ű�δ� �ش� ��ü�� �����͸� �����մϴ�.
	//printf("RegisterSocket SUCCESS\n");
	if (!JYIocp::RegisterSocketToIocp(m_pListen->GetSocket(), reinterpret_cast<ULONG_PTR>(m_pListen)))
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		JYServerIocp::End();

		return FALSE;
	}

	// CConnectedSessionManager�� �����մϴ�.
	// ���� �Լ������� CConnectedSession�� �����ϰ� Accept���·� ����� �ڵ尡 �߰��Ǿ� �ֽ��ϴ�.
	//printf("ConnectedSession Manager Start\n");
	if (!m_oConnectedSessionManager.Begin(m_pListen->GetSocket()))
	{
		JYServerIocp::End();

		return FALSE;
	}

	// KeepAlive ���� �̺�Ʈ�� �����մϴ�.
	//printf("KeepAlive Event Create\n");
	m_hKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!m_hKeepThreadDestroyEvent)
	{
		// ����������� �����մϴ�.
		JYServerIocp::End();
		return FALSE;
	}

	// KeepAlive Thread�� �����մϴ�.
	//printf("KeepAlive Thread Create\n");
	m_hKeepThread = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!m_hKeepThread)
	{
		// ���� �������� ��� �����մϴ�.
		JYServerIocp::End();
		return FALSE;
	}


	//printf("InitDB SUCCESS\n");
	if (!GDBSession->InitDB())
	{
		// �������� ��� End�Լ��� ȣ���ϰ� �����մϴ�.
		JYServerIocp::End();

		return FALSE;
	}


	return TRUE;
}

VOID JYServerIocp::End(VOID)
{
	// CServerIocp�� �����Ҷ�

	// m_hKeepThread�� NULL�� �ƴϸ� KeepAlive Thread�� ������ �ݴϴ�.
	if (m_hKeepThread)
	{
		// ���� �̺�Ʈ�� �߻���ŵ�ϴ�.
		SetEvent(m_hKeepThreadDestroyEvent);

		// Thread�� ����� ������ ����մϴ�.
		WaitForSingleObject(m_hKeepThread, INFINITE);

		// �ڵ鰪�� �ݾ��ݴϴ�.
		CloseHandle(m_hKeepThread);
		m_hKeepThread = NULL;
	}

	// KeepAlive ���� �̺�Ʈ�� NULL�� �ƴϸ�
	if (m_hKeepThreadDestroyEvent)
	{
		// �̺�Ʈ�� �ݾ��ݴϴ�.
		CloseHandle(m_hKeepThreadDestroyEvent);
		m_hKeepThreadDestroyEvent = NULL;
	}

	// IOCP�� �����մϴ�.
	JYIocp::End();

	// CConnectedSessionManager�� �����մϴ�.
	m_oConnectedSessionManager.End();

	// �����ߴ� Listen�� ��ü�� �����ϰ� ������ �ݴϴ�.
	if (m_pListen)
	{
		m_pListen->End();

		delete m_pListen;
	}
}