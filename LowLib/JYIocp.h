#pragma once

class JYIocp
{
public:
	JYIocp(VOID);
	virtual ~JYIocp(VOID);

private:
	HANDLE				mIocpHandle; //iocp관리하는 핸들 값
	DWORD				mWorkerThreadCount;

	std::vector<HANDLE>	mWorkerThreadVector;

	HANDLE				mStartupEventHandle; //시작 관리하는 이벤트 핸들

protected:
	virtual VOID		OnIoRead(VOID *object, DWORD dataLength) = 0;
	virtual VOID		OnIoWrote(VOID *object, DWORD dataLength) = 0;
	virtual VOID		OnIoConnected(VOID *object) = 0;
	virtual VOID		OnIoDisconnected(VOID *object) = 0;

public:
	BOOL				Begin(VOID);
	BOOL				End(VOID);

	BOOL				RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey);

	VOID				WorkerThreadCallback(VOID);
};

