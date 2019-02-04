#pragma once

/*
빈 방 찾아서 room클래스 할당
*/

class JYGRManager : public JYMultiThreadSync<JYGRManager>
{
public:
	JYGRManager(VOID);
	virtual ~JYGRManager(VOID);

private:
	DWORD				mMaxRoomCount;   //최대 방수
	DWORD				mCurrentRoomCount;  //현재 방 수

	std::vector<JYGameRoom*>	mRoomVector;  //룸 개체 포인터 저장하는 벡터

public:
	BOOL	Begin(DWORD maxRoomCount);
	BOOL	End(VOID);
	

		//입장은 퀵조인으로~
	JYGameRoom*	QuickJoin(JYConnectedUser *connectedUser, USHORT &slotIndex);

};
