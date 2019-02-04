#include "stdafx.h"


JYGameRoom::JYGameRoom(VOID)
{
	mIndex = 0;
	mCurrentUserCount = 0;
	mRootUser = NULL;
	mIsGameStarting = FALSE;

	ZeroMemory(mRoomUser, sizeof(mRoomUser));
}

JYGameRoom::~JYGameRoom(VOID)
{

}

//룸 시작
BOOL JYGameRoom::Begin(DWORD index)
{
	JYThreadSync Sync;

	mIndex = index;
	mCurrentUserCount = 0;
	mRootUser = NULL;
	mIsGameStarting = FALSE;
	ZeroMemory(mRoomUser, sizeof(mRoomUser));

	return TRUE;
}

//개체 종료
BOOL JYGameRoom::End(VOID)
{
	JYThreadSync Sync;

	mIndex = 0;
	mCurrentUserCount = 0;
	mRootUser = NULL;   //방장 초기화
	mIsGameStarting = FALSE;

	ZeroMemory(mRoomUser, sizeof(mRoomUser));

	return TRUE;
}

//유저가 방에 들어왓을 때 처리
BOOL JYGameRoom::JoinUser(JYConnectedUser *connectedUser, USHORT &slotIndex)
{
	JYThreadSync Sync;

	if (!connectedUser)
		return FALSE;


	for (USHORT i = 0; i<2; i++)
	{
		// 빈자리를 찾아준다.
		if (mRoomUser[i] == NULL)
		{
			mRoomUser[i] = connectedUser;
			connectedUser->SetEnteredRoom(this);

			//ㅇ인원수 증가
			mCurrentUserCount = min(SHORT(mCurrentUserCount++), 2);

			slotIndex = i;

			// 방 처음 생성
			if (mCurrentUserCount == 1)
			{
				mRootUser = connectedUser;
			}

			return TRUE;
		}
	}

	return FALSE;
}

//유저가 방 나갈때 호출
BOOL JYGameRoom::LeaveUser(BOOL isDisconnected, JYGServerIocp *iocp, JYConnectedUser *connectedUser)
{
	JYThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	if (!connectedUser)
		return FALSE;


	// 해당 사용자를 찾는다.
	if (mRoomUser[0] == connectedUser)
	{
		mRoomUser[0] = NULL;
		connectedUser->SetEnteredRoom(NULL);  //방 정보 초기화

		mCurrentUserCount -= 1;
	}

	if (mRoomUser[1] == connectedUser)
	{
		mRoomUser[1] = NULL;
		connectedUser->SetEnteredRoom(NULL);  //방 정보 초기화

		mCurrentUserCount -= 1;
	}

	if (mRoomUser[0] == NULL && mRoomUser[1] == NULL && GetIsGameStarting())
	{
		SetIsGameStarting(FALSE);
	}

	return FALSE;
}

//모든 사용자한테 패킷 전송
BOOL JYGameRoom::WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength)
{
	JYThreadSync Sync;

	if (protocol <= 0 || !packet)
		return FALSE;

	for (USHORT i = 0; i<2; i++)
	{
		// 해당 사용자를 찾는다.
		if (mRoomUser[i])
			mRoomUser[i]->WritePacket(protocol, packet, packetLength);
	}

	return TRUE;
}

