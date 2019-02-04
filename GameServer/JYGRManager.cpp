#include "stdafx.h"


JYGRManager::JYGRManager(VOID)
{
}

JYGRManager::~JYGRManager(VOID)
{
}

BOOL JYGRManager::Begin(DWORD maxRoomCount)
{
	if (maxRoomCount <= 0)
		return FALSE;

	mMaxRoomCount = maxRoomCount;

	for (DWORD i = 0; i<mMaxRoomCount; i++)
	{
		JYGameRoom *Room = new JYGameRoom();

		//초인터 벡터에 값 넣기
		if (Room->Begin(i))

			mRoomVector.push_back(Room);
		else
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

BOOL JYGRManager::End(VOID)
{
	JYThreadSync Sync;

	for (DWORD i = 0; i<mRoomVector.size(); i++)
	{
		JYGameRoom *Room = mRoomVector[i];

		Room->End();

		delete Room;
	}

	mRoomVector.clear();

	return TRUE;
}

//입장 퀵조인
//output 파라미터를 이용해 어느 슬롯으로 들어갈지 정함
JYGameRoom* JYGRManager::QuickJoin(JYConnectedUser *connectedUser, USHORT &slotIndex)
{
	JYThreadSync Sync;

	if (!connectedUser)
		return NULL;

	for (DWORD i = 0; i<mRoomVector.size(); i++)
	{
		JYGameRoom *Room = mRoomVector[i];

		// 사람이 있고 꽉차지 않은 방으로~ 시작도 하지 않았고..
		if (!Room->GetIsFull() && !Room->GetIsEmpty() && !Room->GetIsGameStarting())
		{
			//실패시 계속 방 찾고 성공시 방 개체 포인터 반환
			if (!Room->JoinUser(connectedUser, slotIndex))  //실패
				continue;
			else  //성공
				return Room;
		}
	}

	// 사람있는 방이 없거나 모두 꽉 찾을 경우
	// 우선 빈 방 찾기
	for (DWORD i = 0; i<mRoomVector.size(); i++)
	{
		JYGameRoom *Room = mRoomVector[i];

		if (Room->GetIsEmpty())
		{
			if (!Room->JoinUser(connectedUser, slotIndex))
				continue;
			else
				return Room;
		}
	}

	// 방 꽉 차있으면 걍 널
	return NULL;
}


