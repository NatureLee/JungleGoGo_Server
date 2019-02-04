#pragma once
/*
로비에서 방 관리, 게임 내에서는 룰 관리
*/


class JYConnectedUser;
class JYGServerIocp;


class JYGameRoom : public JYMultiThreadSync<JYGameRoom>
{
public:
	JYGameRoom(VOID);
	virtual ~JYGameRoom(VOID);

public:
	DWORD					mIndex;  // 방 고유 번호


	JYConnectedUser*	    mRoomUser[2];  //최대 인원
	USHORT					mCurrentUserCount = 1; //현재 인원

	JYConnectedUser		    *mRootUser;  //방장 개체 포인터

	BOOL					mIsGameStarting;

public:
	BOOL					Begin(DWORD index);
	BOOL					End(VOID);

	BOOL					JoinUser(JYConnectedUser *connectedUser, USHORT &slotIndex);
	BOOL					LeaveUser(BOOL isDisconnected, JYGServerIocp *iocp, JYConnectedUser *connectedUser);

	BOOL					WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength);


	inline BOOL				GetIsEmpty(VOID) { JYThreadSync Sync; return mCurrentUserCount ? FALSE : TRUE; }  //현재 방 비엇는지 확인
	inline BOOL				GetIsFull(VOID) { JYThreadSync Sync; return mCurrentUserCount == 2 ? TRUE : FALSE; }  //현재 방 다 찼는지 확인
	inline JYConnectedUser** GetRoomUser(VOID) { JYThreadSync Sync; return mRoomUser; }  //방안 사용자 관리 배열 포인터 반환
	inline BOOL				GetIsGameStarting(VOID) { JYThreadSync Sync; return mIsGameStarting; }
	inline VOID				SetIsGameStarting(BOOL isGameStarting) { JYThreadSync Sync; mIsGameStarting = isGameStarting; }																						 //멤버 변수의 Set, get 함수들
	inline DWORD			GetIndex(VOID) { JYThreadSync Sync; return mIndex; }
	inline USHORT			GetCurrentUserCount(VOID) { JYThreadSync Sync; return mCurrentUserCount; }
	inline JYConnectedUser*	GetRootUser(VOID) { JYThreadSync Sync; return mRootUser; }

};
