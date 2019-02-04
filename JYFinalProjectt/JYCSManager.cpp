#include "stdafx.h"
#include "JYCSManager.h"

JYCSManager::JYCSManager(VOID)
{
}

JYCSManager::~JYCSManager(VOID)
{
}

// CConnectedSessionManager�� �����ϴ� �Լ�
BOOL JYCSManager::Begin(SOCKET hListenSocket)
{
	// ����ȭ ��ü
	JYThreadSync Sync;

	// �Ķ���ͷ� Listen��ü�� ������ �޾ƾ� �մϴ�.
	// ���� NULL�� ��� �����մϴ�.
	if (!hListenSocket)
		return FALSE;
	printf("ConnectedSession Create\n");
	// �ִ� �����ڸ�ŭ�� ��ü�� �̸� �����մϴ�.
	for (DWORD i = 0; i<MAX_USER; i++)
	{
		JYConnectedSession *pConnectedSession = new JYConnectedSession();
		// ������ ��ü�� ��ü�� �����ϴ� m_vConnectedSessions ���Ϳ� �Է��մϴ�.
		m_vConnectedSessions.push_back(pConnectedSession);
	}

	printf("ConnectedSession Accept state\n");
	// ������ ��ü���� �ʱ�ȭ�ϰ� Accept���·� ����� �ݴϴ�.
	for (DWORD i = 0; i<MAX_USER; i++)
	{
		// ��ü �ʱ�ȭ�� ������ ��� �����ŵ�ϴ�.
		if (!m_vConnectedSessions[i]->Begin())
		{
			JYCSManager::End();
			return FALSE;
		}

		// ��ü�� Accept���·� �����ϴ� ������ ������ �����ŵ�ϴ�.
		if (!m_vConnectedSessions[i]->Accept(hListenSocket))
		{
			JYCSManager::End();
			return FALSE;
		}
	}

	return TRUE;
}

// ConnectedSessionManager�� �����ϴ� �Լ�
VOID JYCSManager::End(VOID)
{
	// ����ȭ ��ü
	JYThreadSync Sync;

	// �ִ� �����ڸ�ŭ�� ��ü�� �����մϴ�.
	for (DWORD i = 0; i<m_vConnectedSessions.size(); i++)
	{
		// ��ü ����
		m_vConnectedSessions[i]->End();

		delete m_vConnectedSessions[i];
	}

	// �����ϴ� ������ ������ ��� �����ݴϴ�.
	m_vConnectedSessions.clear();
}

// ���ӵ� ��� Session�� �����͸� �����ϴ� �Լ�
VOID JYCSManager::WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength)
{
	// ����ȭ ��ü
	JYThreadSync Sync;

	// ���ӵ� ��� ��ü�� WritePacket�� �� �ݴϴ�.
	for (DWORD i = 0; i<m_vConnectedSessions.size(); i++)
	{
		// ���ӵǾ� �ִ� ��ü�� ��츸 WritePacket�� �� �ݴϴ�.
		if (m_vConnectedSessions[i]->GetConnected())
			m_vConnectedSessions[i]->WritePacket(dwProtocol, pData, dwLength);
	}
}