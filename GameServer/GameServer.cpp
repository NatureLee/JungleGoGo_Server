#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CoInitialize(NULL);

	// Winsock을 사용하기 위한 DLL 로드
	WSADATA WsaData;
	WSAStartup(MAKEWORD(2, 2), &WsaData);	// 2.2 버전을 로드합니다.

											// 서버를 시작하는곳
	JYGServerIocp ServerIocp;

	if (ServerIocp.Begin())
	{
		printf("GameServer Start \n");
		getchar();
		ServerIocp.End();
	}

	// 종료시 DLL을 언로드 시킵니다.
	WSACleanup();
	CoUninitialize();

	return 0;
}