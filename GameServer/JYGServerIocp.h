#pragma once


/*
�������� ����ϴ� ��� ������ �����ϴ� Ŭ����
*/


//���� ��ü ����̱� ������ IOCP��� ��
class JYGServerIocp : public JYIocp
{
public:
	JYGServerIocp(VOID);
	virtual ~JYGServerIocp(VOID);

private:
	JYNetwork	*        	mListenSession;
	JYCUManager	            mConnectedUserManager;
	JYGRManager 			mRoomManager;

	//������ ����� Ŭ�� �˻��ؼ� ó���ϴ� ������(keepalive)
	HANDLE					mKeepThreadHandle;
	HANDLE					mKeepThreadDestroyEvent;

	//���Ӱ��� �ð� �����ϴ� ������
	HANDLE					mGameThreadHandle;
	HANDLE					mGameThreadDestroyEvent;

	//CAdoSessionPool			mDataBase;

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);

	VOID	KeepThreadCallback(VOID);
	VOID	GameThreadCallback(VOID);

	//IOCP���� ��ӹ��� �����Լ�
protected:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);

private:
	//////////////////////////////////////////////////////////////////////////
	// �������� ó�� �Լ�
	// Lobby
	VOID PROC_PT_INLOBBY(JYConnectedUser *connectedUser, BYTE *packet);
	VOID PROC_PT_GAMESTART(JYConnectedUser *connectedUser, BYTE *packet);
	VOID PROC_PT_ROOMLEAVE(JYConnectedUser *connectedUser, BYTE *packet);
	VOID PROC_PT_PLAYERNICK(JYConnectedUser *connectedUser, BYTE *packet);



	//////////////////////////////////////////////////////////////////////////
	// Game

	VOID PROC_PT_MOVETURN(JYConnectedUser *connectedUser, BYTE *packet);
	VOID PROC_PT_MOVE(JYConnectedUser *connectedUser, BYTE *packet);
	VOID PROC_PT_WIN(JYConnectedUser *connectedUser, BYTE *packet);
	VOID PROC_PT_ROOMOUT(JYConnectedUser *connectedUser, BYTE *packet);


	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �����ͺ��̽� ó�� �Լ�

	//////////////////////////////////////////////////////////////////////////
};
