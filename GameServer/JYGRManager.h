#pragma once

/*
�� �� ã�Ƽ� roomŬ���� �Ҵ�
*/

class JYGRManager : public JYMultiThreadSync<JYGRManager>
{
public:
	JYGRManager(VOID);
	virtual ~JYGRManager(VOID);

private:
	DWORD				mMaxRoomCount;   //�ִ� ���
	DWORD				mCurrentRoomCount;  //���� �� ��

	std::vector<JYGameRoom*>	mRoomVector;  //�� ��ü ������ �����ϴ� ����

public:
	BOOL	Begin(DWORD maxRoomCount);
	BOOL	End(VOID);
	

		//������ ����������~
	JYGameRoom*	QuickJoin(JYConnectedUser *connectedUser, USHORT &slotIndex);

};
