#pragma once
/*
��ü ����ڵ ���� �۾�
��ü ����� ���� Ȯ��
*/
class JYCUManager : public JYMultiThreadSync<JYCUManager>
{
public:
	JYCUManager(VOID);
	virtual ~JYCUManager(VOID);

private:

	// ConnectedUser ��ü �����ϴ� ����
	std::vector<JYConnectedUser*>	mConnectedUserVector;
	DWORD							mMaxUserCount;      //�ִ� �ο� ����

	DWORD							mCurrentUserCount;  //���� ����� ��

	SOCKET							mListenSocket;

public:
	BOOL	Begin(DWORD maxUserCount, SOCKET listenSocket);
	BOOL	End(VOID);

	//��� ���� ��ü�� accept���·� ����
	BOOL	AcceptAll(VOID);
	//��ü���� ��Ŷ ����
	BOOL	WriteAll(DWORD protocol, BYTE *data, DWORD dataLength); 

	//���� ����� �� ��Ʈ��
	inline VOID	IncreaseConnectedUserCount(VOID) { InterlockedIncrement((LONG*)&mCurrentUserCount); }
	inline VOID	DecreaseConnectedUserCount(VOID) { InterlockedDecrement((LONG*)&mCurrentUserCount); }
};
