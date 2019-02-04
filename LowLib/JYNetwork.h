#pragma once

class JYNetwork : public JYMultiThreadSync<JYNetwork>
{
public:
	JYNetwork(VOID);
	virtual ~JYNetwork(VOID);

private:
	OVERLAPPED_EX	mAcceptOverlapped;
	OVERLAPPED_EX	mReadOverlapped;
	OVERLAPPED_EX	mWriteOverlapped;

	BYTE			mReadBuffer[MAX_BUFFER_LENGTH];

	SOCKET			mSocket;

public:
	BOOL			Begin(VOID);
	BOOL			End(VOID);

	BOOL			Listen(USHORT port, INT backLog);
	BOOL			Accept(SOCKET listenSocket);
	BOOL			TcpBind(VOID);

	BOOL			InitializeReadForIocp(VOID);
	BOOL			ReadForEventSelect(BYTE *data, DWORD &dataLength);
	BOOL			ReadForIocp(BYTE *data, DWORD &dataLength);

	BOOL			Write(BYTE *data, DWORD dataLength);
	BOOL			Connect(LPSTR address, USHORT port);

	SOCKET			GetSocket(VOID);
};
