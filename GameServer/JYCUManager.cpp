#include "stdafx.h"


JYCUManager::JYCUManager(VOID)
{
	mMaxUserCount = 0;
	mCurrentUserCount = 0;
}

JYCUManager::~JYCUManager(VOID)
{
}

BOOL JYCUManager::Begin(DWORD maxUserCount, SOCKET listenSocket)
{
	JYThreadSync Sync;

	//파라미터값 체크
	if (maxUserCount <= 0 || !listenSocket)
		return FALSE;

	//파라미터로 받은 최대 인원 설정
	mMaxUserCount = maxUserCount;
	mCurrentUserCount = 0;  //현재 인원 초기화

							//listen 소켓 핸들 저장
	mListenSocket = listenSocket;

	//최대 인원수 만큼 유저개체 생성
	for (DWORD i = 0; i<mMaxUserCount; i++)
	{
		//개체 생성해서 벡터에 입력(포인터 벡터)
		JYConnectedUser *ConnectedUser = new JYConnectedUser();

		//유저 생성,초기화
		if (ConnectedUser->Begin())
			mConnectedUserVector.push_back(ConnectedUser);
		else
		{
			//실패시 끝
			End();

			return FALSE;
		}
	}

	return TRUE;
}

//개체 종료
BOOL JYCUManager::End(VOID)
{
	JYThreadSync Sync;

	//현재 벡터 크기만큼 반복
	for (DWORD i = 0; i<mConnectedUserVector.size(); i++)
	{
		JYConnectedUser *ConnectedUser = mConnectedUserVector[i];  //벡터에서 개체 갖고옴

		ConnectedUser->End();

		delete ConnectedUser; //ㅁㅔ모리 삭제
	}

	//벡터 비우기
	mConnectedUserVector.clear();

	return TRUE;
}

//생성한 모든 유제 개체를 대기 상태로 만듬
BOOL JYCUManager::AcceptAll(VOID)
{
	JYThreadSync Sync;

	for (DWORD i = 0; i<mConnectedUserVector.size(); i++)
	{
		JYConnectedUser *ConnectedUser = mConnectedUserVector[i];

		//Accept 실행
		if (!ConnectedUser->Accept(mListenSocket))
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

//접속중인 모든 클라한테 패킷 전송
BOOL JYCUManager::WriteAll(DWORD protocol, BYTE *data, DWORD dataLength)
{
	JYThreadSync Sync;

	//데이터 잘못되어있음 종료
	if (!data)
		return FALSE;

	for (DWORD i = 0; i<mConnectedUserVector.size(); i++)
	{
		JYConnectedUser *ConnectedUser = mConnectedUserVector[i];

		//현재 접속중인 개체라면 (접속중인 유저에게만 패ㅑ킷전송)
		if (ConnectedUser->GetIsConnected())
		{
			//패킷 전송
			if (!ConnectedUser->WritePacket(protocol, data, dataLength))
				ConnectedUser->End();
		}
	}

	return TRUE;
}