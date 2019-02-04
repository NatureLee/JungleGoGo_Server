#pragma once

#define MAX_QUEUE_LENGTH	500

typedef struct _QUEUE_DATA
{
	VOID	*Object;
	BYTE	Data[MAX_BUFFER_LENGTH];
	DWORD	DataLength;

	CHAR	RemoteAddress[32];
	USHORT	RemotePort;

	DWORD	Protocol;
} QUEUE_DATA;


class JYCircularQueue : public JYMultiThreadSync<JYCircularQueue>
{
public:
	JYCircularQueue(VOID);
	~JYCircularQueue(VOID);

private:
	QUEUE_DATA cQueue[MAX_QUEUE_LENGTH];  //실제 큐에 들어가는 데이터가 저장되는 메모리 영역
	DWORD cQueueHead;
	DWORD cQueueTail;

public:
	BOOL		Begin(VOID);
	BOOL		End(VOID);

	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength);
	BYTE*		Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort);

	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength);
	BOOL		Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort);
	BOOL		Pop(VOID);

	BOOL		IsEmpty(VOID);
};

