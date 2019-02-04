#pragma once
/*
전체 사용자등레 대한 작업
전체 사용자 상태 확인
*/
class JYCUManager : public JYMultiThreadSync<JYCUManager>
{
public:
	JYCUManager(VOID);
	virtual ~JYCUManager(VOID);

private:

	// ConnectedUser 개체 관리하는 벡터
	std::vector<JYConnectedUser*>	mConnectedUserVector;
	DWORD							mMaxUserCount;      //최대 인원 설정

	DWORD							mCurrentUserCount;  //현재 사용자 수

	SOCKET							mListenSocket;

public:
	BOOL	Begin(DWORD maxUserCount, SOCKET listenSocket);
	BOOL	End(VOID);

	//모든 유저 개체를 accept상태로 만듬
	BOOL	AcceptAll(VOID);
	//전체한테 패킷 보냄
	BOOL	WriteAll(DWORD protocol, BYTE *data, DWORD dataLength); 

	//현재 사용자 수 컨트롤
	inline VOID	IncreaseConnectedUserCount(VOID) { InterlockedIncrement((LONG*)&mCurrentUserCount); }
	inline VOID	DecreaseConnectedUserCount(VOID) { InterlockedDecrement((LONG*)&mCurrentUserCount); }
};
