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

//�� ����
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

//��ü ����
BOOL JYGameRoom::End(VOID)
{
	JYThreadSync Sync;

	mIndex = 0;
	mCurrentUserCount = 0;
	mRootUser = NULL;   //���� �ʱ�ȭ
	mIsGameStarting = FALSE;

	ZeroMemory(mRoomUser, sizeof(mRoomUser));

	return TRUE;
}

//������ �濡 ������ �� ó��
BOOL JYGameRoom::JoinUser(JYConnectedUser *connectedUser, USHORT &slotIndex)
{
	JYThreadSync Sync;

	if (!connectedUser)
		return FALSE;


	for (USHORT i = 0; i<2; i++)
	{
		// ���ڸ��� ã���ش�.
		if (mRoomUser[i] == NULL)
		{
			mRoomUser[i] = connectedUser;
			connectedUser->SetEnteredRoom(this);

			//���ο��� ����
			mCurrentUserCount = min(SHORT(mCurrentUserCount++), 2);

			slotIndex = i;

			// �� ó�� ����
			if (mCurrentUserCount == 1)
			{
				mRootUser = connectedUser;
			}

			return TRUE;
		}
	}

	return FALSE;
}

//������ �� ������ ȣ��
BOOL JYGameRoom::LeaveUser(BOOL isDisconnected, JYGServerIocp *iocp, JYConnectedUser *connectedUser)
{
	JYThreadSync Sync;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	if (!connectedUser)
		return FALSE;


	// �ش� ����ڸ� ã�´�.
	if (mRoomUser[0] == connectedUser)
	{
		mRoomUser[0] = NULL;
		connectedUser->SetEnteredRoom(NULL);  //�� ���� �ʱ�ȭ

		mCurrentUserCount -= 1;
	}

	if (mRoomUser[1] == connectedUser)
	{
		mRoomUser[1] = NULL;
		connectedUser->SetEnteredRoom(NULL);  //�� ���� �ʱ�ȭ

		mCurrentUserCount -= 1;
	}

	if (mRoomUser[0] == NULL && mRoomUser[1] == NULL && GetIsGameStarting())
	{
		SetIsGameStarting(FALSE);
	}

	return FALSE;
}

//��� ��������� ��Ŷ ����
BOOL JYGameRoom::WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength)
{
	JYThreadSync Sync;

	if (protocol <= 0 || !packet)
		return FALSE;

	for (USHORT i = 0; i<2; i++)
	{
		// �ش� ����ڸ� ã�´�.
		if (mRoomUser[i])
			mRoomUser[i]->WritePacket(protocol, packet, packetLength);
	}

	return TRUE;
}

