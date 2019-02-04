#include "stdafx.h"


JYConnectedUser::JYConnectedUser(VOID)
{
	mEnteredRoom = NULL;  // ���� ���� ���� �� ����

	mIsConnected = FALSE; // ���� �Ǿ����� ����
}

JYConnectedUser::~JYConnectedUser(VOID)
{

}

BOOL JYConnectedUser::Begin(VOID)
{
	//������ ����ȭ
	JYThreadSync Sync;

	mEnteredRoom = NULL;

	mIsConnected = FALSE;


	//��Ŷ���� ����
	return JYPacketSession::Begin();
}

BOOL JYConnectedUser::End(VOID)
{
	JYThreadSync Sync;

	mEnteredRoom = NULL;

	mIsConnected = FALSE;

	return JYPacketSession::End();
}

//���� ����Ǿ��� �� ��ü �籸��
//Accept ���ؼ� listensocket �Ķ����
BOOL JYConnectedUser::Reload(SOCKET listenSocket)
{
	JYThreadSync Sync;

	End();

	//��ü �����
	if (!Begin())
		return FALSE;

	//�ֻ��� �θ� NetWork�� Accept ȣ�� -> accept���� �ǰ� ��
	if (!JYNetwork::Accept(listenSocket))
		return FALSE;

	return TRUE;
}