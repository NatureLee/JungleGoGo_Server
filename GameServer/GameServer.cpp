#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	// Winsock�� ����ϱ� ���� DLL �ε�
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);	// 2.2 ������ �ε��մϴ�.

											// ������ �����ϴ°�
	JYGServerIocp ServerIocp;

	if (ServerIocp.Begin())
	{
		printf("GameServer Start \n");
		getchar();
		ServerIocp.End();
	}

	// ����� DLL�� ��ε� ��ŵ�ϴ�.
	WSACleanup();
	CoUninitialize();

	return 0;
}