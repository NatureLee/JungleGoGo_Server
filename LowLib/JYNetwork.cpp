#include "stdafx.h"
#include "JYCriticalSection.h"
#include "JYMultiThreadSync.h"
#include "JYCircularQueue.h"
#include "JYNetwork.h"

JYNetwork::JYNetwork(VOID)
{
	//overlapped 구조체 초기화
	ZeroMemory(&mAcceptOverlapped, sizeof(mAcceptOverlapped));
	ZeroMemory(&mReadOverlapped, sizeof(mReadOverlapped));
	ZeroMemory(&mWriteOverlapped, sizeof(mWriteOverlapped));
	//read에서 사용하는 버퍼
	ZeroMemory(mReadBuffer, sizeof(mReadBuffer));

	mSocket = NULL;

	//overlapped 구조체의 종류 정의
	mAcceptOverlapped.IoType = IO_ACCEPT;
	mReadOverlapped.IoType = IO_READ;
	mWriteOverlapped.IoType = IO_WRITE;

	//현재 개체의 포인터 가짐
	mAcceptOverlapped.Object = this;
	mReadOverlapped.Object = this;
	mWriteOverlapped.Object = this;
}

JYNetwork::~JYNetwork(VOID)
{

}

BOOL JYNetwork::Begin(VOID)
{
	JYThreadSync Sync;

	if (mSocket)
		return FALSE;

	ZeroMemory(mReadBuffer, sizeof(mReadBuffer));

	mSocket = NULL;

	return TRUE;
}

BOOL JYNetwork::End(VOID)
{
	JYThreadSync Sync;

	if (!mSocket)
		return FALSE;

	shutdown(mSocket, SD_BOTH);  //해당 소켓의 리드롸잇을 ㄷ멈춘다
	closesocket(mSocket);

	mSocket = NULL;

	return TRUE;
}

//tcp사용시 맨 처음 호출
BOOL JYNetwork::TcpBind(VOID)
{
	JYThreadSync Sync;

	if (mSocket)
		return FALSE;

	//소켓생성
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	//BOOL NoDelay = TRUE;
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));
	return TRUE;
}

//Listen래핑 함수 (여기서 bind & listen)
BOOL JYNetwork::Listen(USHORT port, INT backLog)
{
	JYThreadSync Sync;

	if (port <= 0 || backLog <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN ListenSocketInfo;

	ListenSocketInfo.sin_family = AF_INET;
	ListenSocketInfo.sin_port = htons(port);
	ListenSocketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	//Bind
	if (bind(mSocket, (struct sockaddr*) &ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	//Listen
	if (listen(mSocket, backLog) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	LINGER Linger;
	Linger.l_onoff = 1;
	Linger.l_linger = 0;

	if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

//서버간의 통신에 사용
BOOL JYNetwork::Connect(LPSTR address, USHORT port)
{
	JYThreadSync Sync;

	if (!address || port <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;


	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_port = htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(address);

	//위에서 설정된 주소로 접속 시도
	if (WSAConnect(mSocket, (LPSOCKADDR)&RemoteAddressInfo, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL JYNetwork::Accept(SOCKET listenSocket)
{
	JYThreadSync Sync;

	if (!listenSocket)
		return FALSE;

	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
	{
		End();

		return FALSE;
	}

	//BOOL NoDelay = TRUE;
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));

	//접속 몰릴때를 대비해 ex함수 사용
	if (!AcceptEx(listenSocket,
		mSocket,
		mReadBuffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		NULL,
		&mAcceptOverlapped.Overlapped))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

//초기 받기
BOOL JYNetwork::InitializeReadForIocp(VOID)
{
	JYThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes = 0;
	DWORD	ReadFlag = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer;
	WsaBuf.len = MAX_BUFFER_LENGTH;

	//Recv호출
	INT	ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

//초기받기 후 실제 데이터를 정해진 버퍼에 복사
//받은 데이터 가져오기
BOOL JYNetwork::ReadForIocp(BYTE *data, DWORD &dataLength)
{
	JYThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	//recv통해서 받아온 데이터 잇는 readbuffer에서 데이터 복사
	memcpy(data, mReadBuffer, dataLength);

	return TRUE;
}

BOOL JYNetwork::ReadForEventSelect(BYTE *data, DWORD &dataLength)
{
	JYThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes = 0;
	DWORD	ReadFlag = 0;

	WsaBuf.buf = (CHAR*)mReadBuffer;
	WsaBuf.len = MAX_BUFFER_LENGTH;

	INT	ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	memcpy(data, mReadBuffer, ReadBytes);
	dataLength = ReadBytes;

	return TRUE;
}

//write 함수
BOOL JYNetwork::Write(BYTE *data, DWORD dataLength)
{
	JYThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	WriteBytes = 0;
	DWORD	WriteFlag = 0;

	WsaBuf.buf = (CHAR*)data;
	WsaBuf.len = dataLength;

	//send 함수
	INT	ReturnValue = WSASend(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		&mWriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

SOCKET JYNetwork::GetSocket(VOID)
{
	JYThreadSync Sync;

	return mSocket;
}