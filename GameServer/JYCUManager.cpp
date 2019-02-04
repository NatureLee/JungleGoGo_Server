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

	//�Ķ���Ͱ� üũ
	if (maxUserCount <= 0 || !listenSocket)
		return FALSE;

	//�Ķ���ͷ� ���� �ִ� �ο� ����
	mMaxUserCount = maxUserCount;
	mCurrentUserCount = 0;  //���� �ο� �ʱ�ȭ

							//listen ���� �ڵ� ����
	mListenSocket = listenSocket;

	//�ִ� �ο��� ��ŭ ������ü ����
	for (DWORD i = 0; i<mMaxUserCount; i++)
	{
		//��ü �����ؼ� ���Ϳ� �Է�(������ ����)
		JYConnectedUser *ConnectedUser = new JYConnectedUser();

		//���� ����,�ʱ�ȭ
		if (ConnectedUser->Begin())
			mConnectedUserVector.push_back(ConnectedUser);
		else
		{
			//���н� ��
			End();

			return FALSE;
		}
	}

	return TRUE;
}

//��ü ����
BOOL JYCUManager::End(VOID)
{
	JYThreadSync Sync;

	//���� ���� ũ�⸸ŭ �ݺ�
	for (DWORD i = 0; i<mConnectedUserVector.size(); i++)
	{
		JYConnectedUser *ConnectedUser = mConnectedUserVector[i];  //���Ϳ��� ��ü �����

		ConnectedUser->End();

		delete ConnectedUser; //���ĸ� ����
	}

	//���� ����
	mConnectedUserVector.clear();

	return TRUE;
}

//������ ��� ���� ��ü�� ��� ���·� ����
BOOL JYCUManager::AcceptAll(VOID)
{
	JYThreadSync Sync;

	for (DWORD i = 0; i<mConnectedUserVector.size(); i++)
	{
		JYConnectedUser *ConnectedUser = mConnectedUserVector[i];

		//Accept ����
		if (!ConnectedUser->Accept(mListenSocket))
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

//�������� ��� Ŭ������ ��Ŷ ����
BOOL JYCUManager::WriteAll(DWORD protocol, BYTE *data, DWORD dataLength)
{
	JYThreadSync Sync;

	//������ �߸��Ǿ����� ����
	if (!data)
		return FALSE;

	for (DWORD i = 0; i<mConnectedUserVector.size(); i++)
	{
		JYConnectedUser *ConnectedUser = mConnectedUserVector[i];

		//���� �������� ��ü��� (�������� �������Ը� �Ф�Ŷ����)
		if (ConnectedUser->GetIsConnected())
		{
			//��Ŷ ����
			if (!ConnectedUser->WritePacket(protocol, data, dataLength))
				ConnectedUser->End();
		}
	}

	return TRUE;
}