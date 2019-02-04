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

		//������ ���Ϳ� �� �ֱ�
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

//���� ������
//output �Ķ���͸� �̿��� ��� �������� ���� ����
JYGameRoom* JYGRManager::QuickJoin(JYConnectedUser *connectedUser, USHORT &slotIndex)
{
	JYThreadSync Sync;

	if (!connectedUser)
		return NULL;

	for (DWORD i = 0; i<mRoomVector.size(); i++)
	{
		JYGameRoom *Room = mRoomVector[i];

		// ����� �ְ� ������ ���� ������~ ���۵� ���� �ʾҰ�..
		if (!Room->GetIsFull() && !Room->GetIsEmpty() && !Room->GetIsGameStarting())
		{
			//���н� ��� �� ã�� ������ �� ��ü ������ ��ȯ
			if (!Room->JoinUser(connectedUser, slotIndex))  //����
				continue;
			else  //����
				return Room;
		}
	}

	// ����ִ� ���� ���ų� ��� �� ã�� ���
	// �켱 �� �� ã��
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

	// �� �� �������� �� ��
	return NULL;
}


