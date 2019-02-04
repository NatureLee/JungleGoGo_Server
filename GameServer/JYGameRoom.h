#pragma once
/*
�κ񿡼� �� ����, ���� �������� �� ����
*/


class JYConnectedUser;
class JYGServerIocp;


class JYGameRoom : public JYMultiThreadSync<JYGameRoom>
{
public:
	JYGameRoom(VOID);
	virtual ~JYGameRoom(VOID);

public:
	DWORD					mIndex;  // �� ���� ��ȣ


	JYConnectedUser*	    mRoomUser[2];  //�ִ� �ο�
	USHORT					mCurrentUserCount = 1; //���� �ο�

	JYConnectedUser		    *mRootUser;  //���� ��ü ������

	BOOL					mIsGameStarting;

public:
	BOOL					Begin(DWORD index);
	BOOL					End(VOID);

	BOOL					JoinUser(JYConnectedUser *connectedUser, USHORT &slotIndex);
	BOOL					LeaveUser(BOOL isDisconnected, JYGServerIocp *iocp, JYConnectedUser *connectedUser);

	BOOL					WriteAll(DWORD protocol, BYTE *packet, DWORD packetLength);


	inline BOOL				GetIsEmpty(VOID) { JYThreadSync Sync; return mCurrentUserCount ? FALSE : TRUE; }  //���� �� ������� Ȯ��
	inline BOOL				GetIsFull(VOID) { JYThreadSync Sync; return mCurrentUserCount == 2 ? TRUE : FALSE; }  //���� �� �� á���� Ȯ��
	inline JYConnectedUser** GetRoomUser(VOID) { JYThreadSync Sync; return mRoomUser; }  //��� ����� ���� �迭 ������ ��ȯ
	inline BOOL				GetIsGameStarting(VOID) { JYThreadSync Sync; return mIsGameStarting; }
	inline VOID				SetIsGameStarting(BOOL isGameStarting) { JYThreadSync Sync; mIsGameStarting = isGameStarting; }																						 //��� ������ Set, get �Լ���
	inline DWORD			GetIndex(VOID) { JYThreadSync Sync; return mIndex; }
	inline USHORT			GetCurrentUserCount(VOID) { JYThreadSync Sync; return mCurrentUserCount; }
	inline JYConnectedUser*	GetRootUser(VOID) { JYThreadSync Sync; return mRootUser; }

};
