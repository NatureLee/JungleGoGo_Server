#pragma once


/*
서버에서 통신하는 모든 내용을 관리하는 클래스
*/


//서버 전체 통신이기 때문에 IOCP상속 받
class JYGServerIocp : public JYIocp
{
public:
	JYGServerIocp(VOID);
	virtual ~JYGServerIocp(VOID);

private:
	JYNetwork	*        	mListenSession;
	JYCUManager	            mConnectedUserManager;
	JYGRManager 			mRoomManager;

	//비정상 종료된 클라 검색해서 처리하는 스레드(keepalive)
	HANDLE					mKeepThreadHandle;
	HANDLE					mKeepThreadDestroyEvent;

	//게임관련 시간 관리하는 스레드
	HANDLE					mGameThreadHandle;
	HANDLE					mGameThreadDestroyEvent;

	//CAdoSessionPool			mDataBase;

public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);

	VOID	KeepThreadCallback(VOID);
	VOID	GameThreadCallback(VOID);

	//IOCP에서 상속받은 가상함수
protected:
	VOID OnIoRead(VOID *object, DWORD dataLength);
	VOID OnIoWrote(VOID *object, DWORD dataLength);
	VOID OnIoConnected(VOID *object);
	VOID OnIoDisconnected(VOID *object);

private:
	//////////////////////////////////////////////////////////////////////////
	// 프로토콜 처리 함수
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
	// 데이터베이스 처리 함수

	//////////////////////////////////////////////////////////////////////////
};
