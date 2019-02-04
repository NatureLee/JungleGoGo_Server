#pragma once

class JYGameRoom;

class JYConnectedUser : public JYPacketSession
{
public:
	JYConnectedUser(VOID);
	virtual ~JYConnectedUser(VOID);

private:
	JYGameRoom			*mEnteredRoom;  //���� ������ ���� ��ü

	BOOL				mIsConnected;   //���� ���� ������ Ȯ���ϴ� FLAG

public:
	BOOL				Begin(VOID);
	BOOL				End(VOID);

	BOOL				Reload(SOCKET listenSocket);

	inline BOOL			SetIsConnected(BOOL isConnected) { JYThreadSync Sync; mIsConnected = isConnected; return TRUE; }
	inline BOOL			GetIsConnected(VOID) { JYThreadSync Sync; return mIsConnected; }


	inline BOOL			SetEnteredRoom(JYGameRoom *room) { JYThreadSync Sync; mEnteredRoom = room; return TRUE; }
	inline JYGameRoom*	GetEnteredRoom(VOID) { JYThreadSync Sync; return mEnteredRoom; }


	//////////////////////////////////////////////////////////////////////////
	// ���Ӱ��� �Լ���
	inline BOOL			InitializeForGameStart(VOID) { JYThreadSync Sync;  return TRUE; }


	//////////////////////////////////////////////////////////////////////////
};
