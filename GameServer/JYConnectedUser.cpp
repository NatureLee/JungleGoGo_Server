#include "stdafx.h"


JYConnectedUser::JYConnectedUser(VOID)
{
	mEnteredRoom = NULL;  // 현재 입장 중인 방 없음

	mIsConnected = FALSE; // 접속 되어있지 않음
}

JYConnectedUser::~JYConnectedUser(VOID)
{

}

BOOL JYConnectedUser::Begin(VOID)
{
	//스레드 동기화
	JYThreadSync Sync;

	mEnteredRoom = NULL;

	mIsConnected = FALSE;


	//패킷세션 종료
	return JYPacketSession::Begin();
}

BOOL JYConnectedUser::End(VOID)
{
	JYThreadSync Sync;

	mEnteredRoom = NULL;

	mIsConnected = FALSE;

	return JYPacketSession::End();
}

//접속 종료되었을 때 개체 재구성
//Accept 위해서 listensocket 파라미터
BOOL JYConnectedUser::Reload(SOCKET listenSocket)
{
	JYThreadSync Sync;

	End();

	//개체 재시작
	if (!Begin())
		return FALSE;

	//최상위 부모 NetWork의 Accept 호출 -> accept상태 되게 함
	if (!JYNetwork::Accept(listenSocket))
		return FALSE;

	return TRUE;
}