#pragma once

// ConnectedSession�� �����ϴ� Ŭ����
// ���� ������ ȯ�濡�� �����ϰ� ����ϱ� ���ؼ� CMultiThreadSync�� ��ӹ޽��ϴ�.
class JYCSManager : public JYMultiThreadSync<JYCSManager>
{
public:
	JYCSManager(VOID);
	virtual ~JYCSManager(VOID);

	// �����ϴ� �Լ�
	BOOL Begin(SOCKET hListenSocket);
	// �����ϴ� �Լ�
	VOID End(VOID);

	// ���ӵ� ��� ConnectedSession�� ��Ŷ�� �����ϴ� �Լ�
	VOID WriteAll(DWORD dwProtocol, BYTE *pData, DWORD dwLength);

public:
	// ������ ConnectedSession��ü�� �����͸� �����ϴ� Vector
	std::vector<JYConnectedSession*> m_vConnectedSessions;
};
