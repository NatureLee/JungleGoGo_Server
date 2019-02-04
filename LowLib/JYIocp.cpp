#include "stdafx.h"
#include "JYIocp.h"

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	JYIocp *Owner = (JYIocp*)parameter;
	Owner->WorkerThreadCallback();

	return 0;
}

JYIocp::JYIocp(VOID)
{
	mIocpHandle = NULL;
	mWorkerThreadCount = 0;

	mStartupEventHandle = NULL;
}


JYIocp::~JYIocp(VOID)
{
}

BOOL JYIocp::Begin(VOID)
{
	mIocpHandle = NULL;

	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	mWorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;
	//iocp 핸들 생성
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!mIocpHandle)
		return FALSE;

	//시작관리 이벤트 생성
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	//cpu 2배만큼 workerThread생성
	for (DWORD i = 0; i<mWorkerThreadCount; i++)
	{
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mWorkerThreadVector.push_back(WorkerThread);

		WaitForSingleObject(mStartupEventHandle, INFINITE);
	}

	return TRUE;
}

BOOL JYIocp::End(VOID)
{
	//workerThread마다 종료 요청
	for (DWORD i = 0; i<mWorkerThreadVector.size(); i++)
		//큐에 강제로 신호보냄
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);

	for (DWORD i = 0; i<mWorkerThreadVector.size(); i++)
	{
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);

		CloseHandle(mWorkerThreadVector[i]);
	}

	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	mWorkerThreadVector.clear();

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

//IOCP에 소켓 등록
BOOL JYIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	if (!socket || !completionKey)
		return FALSE;

	//소켓등록
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return FALSE;

	return TRUE;
}

//워커스레드
VOID JYIocp::WorkerThreadCallback(VOID)
{
	BOOL			Successed = FALSE;  //성공,실패 유무 판단할 변수
	DWORD			NumberOfByteTransfered = 0; //발생한 IO크기
	VOID			*CompletionKey = NULL;  //키값
	OVERLAPPED		*Overlapped = NULL;  //오버랩값
	OVERLAPPED_EX	*OverlappedEx = NULL;
	VOID			*Object = NULL;

	while (TRUE)
	{
		//workerThread시작할 수 있다고 알려주는 이벤트
		//이 이벤트가 발생해야 다음으로ㅓ 넘어감
		SetEvent(mStartupEventHandle);

		//IOCP에 등록된 소켓 및 파일의 Read,Write관련 함수의 상태를 받아옴
		Successed = GetQueuedCompletionStatus(
			mIocpHandle,
			&NumberOfByteTransfered,
			(PULONG_PTR)&CompletionKey,
			&Overlapped,
			INFINITE);

		if (!CompletionKey)
			return;

		OverlappedEx = (OVERLAPPED_EX*)Overlapped;
		Object = OverlappedEx->Object;

		if (!Successed || (Successed && !NumberOfByteTransfered))
		{
			if (OverlappedEx->IoType == IO_ACCEPT)
				OnIoConnected(Object);
			else
				OnIoDisconnected(Object);

			continue;
		}

		switch (OverlappedEx->IoType)
		{
		case IO_READ:
			OnIoRead(Object, NumberOfByteTransfered);
			break;

		case IO_WRITE:
			OnIoWrote(Object, NumberOfByteTransfered);
			break;
		}
	}
}
