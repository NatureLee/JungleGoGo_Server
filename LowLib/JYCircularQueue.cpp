#include "stdafx.h"
#include "JYCriticalSection.h"
#include "JYMultiThreadSync.h"
#include "JYCircularQueue.h"


JYCircularQueue::JYCircularQueue(VOID)
{
	ZeroMemory(cQueue, sizeof(cQueue));

	cQueueHead = cQueueTail = 0;
}

JYCircularQueue::~JYCircularQueue(VOID)
{
}

BOOL JYCircularQueue::Begin(VOID)
{
	ZeroMemory(cQueue, sizeof(cQueue));

	cQueueHead = cQueueTail = 0;

	return TRUE;
}

BOOL JYCircularQueue::End(VOID)
{
	return TRUE;
}

BYTE* JYCircularQueue::Push(VOID *object, BYTE *data, DWORD dataLength)
{
	JYThreadSync Sync; //동기화 개체

					   //CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (cQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == cQueueHead)
		return NULL;

	cQueue[TempTail].Object = object;
	cQueue[TempTail].DataLength = dataLength;

	memcpy(cQueue[TempTail].Data, data, dataLength);

	cQueueTail = TempTail;

	return cQueue[TempTail].Data;
}

BYTE* JYCircularQueue::Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength)
{
	//JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (cQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == cQueueHead)
		return NULL;

	cQueue[TempTail].Object = object;
	cQueue[TempTail].DataLength = dataLength;
	cQueue[TempTail].Protocol = protocol;

	memset(cQueue[TempTail].Data, 0, sizeof(cQueue[TempTail].Data));
	memcpy(cQueue[TempTail].Data, data, dataLength);

	cQueueTail = TempTail;

	return cQueue[TempTail].Data;
}

BYTE *JYCircularQueue::Push(VOID *object, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (cQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == cQueueHead)
		return NULL;

	cQueue[TempTail].Object = object;
	cQueue[TempTail].DataLength = dataLength;
	cQueue[TempTail].RemotePort = remotePort;

	memset(cQueue[TempTail].RemoteAddress, 0, sizeof(cQueue[TempTail].RemoteAddress));
	strncpy(cQueue[TempTail].RemoteAddress, remoteAddress, sizeof(cQueue[TempTail].RemoteAddress));

	memset(cQueue[TempTail].Data, 0, sizeof(cQueue[TempTail].Data));
	memcpy(cQueue[TempTail].Data, data, dataLength);

	cQueueTail = TempTail;

	return cQueue[TempTail].Data;
}

BYTE *JYCircularQueue::Push(VOID *object, DWORD protocol, BYTE *data, DWORD dataLength, LPCSTR remoteAddress, USHORT remotePort)
{
	JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return NULL;

	DWORD TempTail = (cQueueTail + 1) % MAX_QUEUE_LENGTH;

	if (TempTail == cQueueHead)
		return NULL;

	cQueue[TempTail].Object = object;
	cQueue[TempTail].DataLength = dataLength;
	cQueue[TempTail].RemotePort = remotePort;
	cQueue[TempTail].Protocol = protocol;

	memset(cQueue[TempTail].RemoteAddress, 0, sizeof(cQueue[TempTail].RemoteAddress));
	strncpy(cQueue[TempTail].RemoteAddress, remoteAddress, sizeof(cQueue[TempTail].RemoteAddress));

	memset(cQueue[TempTail].Data, 0, sizeof(cQueue[TempTail].Data));
	memcpy(cQueue[TempTail].Data, data, dataLength);

	cQueueTail = TempTail;

	return cQueue[TempTail].Data;
}

BOOL JYCircularQueue::Pop(VOID)
{
	JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (cQueueHead == cQueueTail)
		return FALSE;

	DWORD TempHead = (cQueueHead + 1) % MAX_QUEUE_LENGTH;

	cQueueHead = TempHead;

	return TRUE;
}

BOOL JYCircularQueue::Pop(VOID **object, BYTE *data, DWORD &dataLength)
{
	JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (cQueueHead == cQueueTail)
		return FALSE;

	DWORD TempHead = (cQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = cQueue[TempHead].DataLength;
	*object = cQueue[TempHead].Object;

	memcpy(data, cQueue[TempHead].Data, cQueue[TempHead].DataLength);

	cQueueHead = TempHead;

	return TRUE;
}

BOOL JYCircularQueue::Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength)
{
	JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (cQueueHead == cQueueTail)
		return FALSE;

	DWORD TempHead = (cQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = cQueue[TempHead].DataLength;
	*object = cQueue[TempHead].Object;
	protocol = cQueue[TempHead].Protocol;

	memcpy(data, cQueue[TempHead].Data, cQueue[TempHead].DataLength);

	cQueueHead = TempHead;

	return TRUE;
}

BOOL JYCircularQueue::Pop(VOID **object, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort)
{
	JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (cQueueHead == cQueueTail)
		return FALSE;

	DWORD TempHead = (cQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = cQueue[TempHead].DataLength;
	*object = cQueue[TempHead].Object;
	remotePort = cQueue[TempHead].RemotePort;

	strncpy(cQueue[TempHead].RemoteAddress, remoteAddress, sizeof(cQueue[TempHead].RemoteAddress));
	memcpy(data, cQueue[TempHead].Data, cQueue[TempHead].DataLength);

	cQueueHead = TempHead;

	return TRUE;
}

BOOL JYCircularQueue::Pop(VOID **object, DWORD &protocol, BYTE *data, DWORD &dataLength, LPSTR remoteAddress, USHORT &remotePort)
{
	JYThreadSync Sync;

	//CLog::WriteLog(_T("Queue Status  : Head (%d) Tail (%d)\n"), mQueueHead, mQueueTail);

	if (!object || !data)
		return FALSE;

	if (cQueueHead == cQueueTail)
		return FALSE;

	DWORD TempHead = (cQueueHead + 1) % MAX_QUEUE_LENGTH;

	dataLength = cQueue[TempHead].DataLength;
	*object = cQueue[TempHead].Object;
	remotePort = cQueue[TempHead].RemotePort;
	protocol = cQueue[TempHead].Protocol;

	strncpy_s(remoteAddress, sizeof(LPSTR), cQueue[TempHead].RemoteAddress, sizeof(cQueue[TempHead].RemoteAddress));
	memcpy(data, cQueue[TempHead].Data, cQueue[TempHead].DataLength);

	cQueueHead = TempHead;

	return TRUE;
}

BOOL JYCircularQueue::IsEmpty(VOID)
{
	JYThreadSync Sync;

	if (cQueueHead == cQueueTail)
		return TRUE;

	return FALSE;
}


