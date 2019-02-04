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
	//iocp �ڵ� ����
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (!mIocpHandle)
		return FALSE;

	//���۰��� �̺�Ʈ ����
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	//cpu 2�踸ŭ workerThread����
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
	//workerThread���� ���� ��û
	for (DWORD i = 0; i<mWorkerThreadVector.size(); i++)
		//ť�� ������ ��ȣ����
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

//IOCP�� ���� ���
BOOL JYIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	if (!socket || !completionKey)
		return FALSE;

	//���ϵ��
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return FALSE;

	return TRUE;
}

//��Ŀ������
VOID JYIocp::WorkerThreadCallback(VOID)
{
	BOOL			Successed = FALSE;  //����,���� ���� �Ǵ��� ����
	DWORD			NumberOfByteTransfered = 0; //�߻��� IOũ��
	VOID			*CompletionKey = NULL;  //Ű��
	OVERLAPPED		*Overlapped = NULL;  //��������
	OVERLAPPED_EX	*OverlappedEx = NULL;
	VOID			*Object = NULL;

	while (TRUE)
	{
		//workerThread������ �� �ִٰ� �˷��ִ� �̺�Ʈ
		//�� �̺�Ʈ�� �߻��ؾ� �������Τ� �Ѿ
		SetEvent(mStartupEventHandle);

		//IOCP�� ��ϵ� ���� �� ������ Read,Write���� �Լ��� ���¸� �޾ƿ�
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
