#pragma once

class JYGameRoom;

class JYConnectedUser : public JYPacketSession
{
public:
	JYConnectedUser(VOID);
	virtual ~JYConnectedUser(VOID);

private:
	JYGameRoom			*mEnteredRoom;  //현재 입장한 방의 개체

	BOOL				mIsConnected;   //현재 접속 중인지 확인하는 FLAG

public:
	BOOL				Begin(VOID);
	BOOL				End(VOID);

	BOOL				Reload(SOCKET listenSocket);

	inline BOOL			SetIsConnected(BOOL isConnected) { JYThreadSync Sync; mIsConnected = isConnected; return TRUE; }
	inline BOOL			GetIsConnected(VOID) { JYThreadSync Sync; return mIsConnected; }


	inline BOOL			SetEnteredRoom(JYGameRoom *room) { JYThreadSync Sync; mEnteredRoom = room; return TRUE; }
	inline JYGameRoom*	GetEnteredRoom(VOID) { JYThreadSync Sync; return mEnteredRoom; }


	//////////////////////////////////////////////////////////////////////////
	// 게임관련 함수들
	inline BOOL			InitializeForGameStart(VOID) { JYThreadSync Sync;  return TRUE; }


	//////////////////////////////////////////////////////////////////////////
};
