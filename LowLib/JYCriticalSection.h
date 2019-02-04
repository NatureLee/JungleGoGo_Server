#pragma once

class JYCriticalSection
{
public:
	JYCriticalSection(VOID)
	{
		InitializeCriticalSection(&mSync);
	}

	~JYCriticalSection(VOID)
	{
		DeleteCriticalSection(&mSync);
	}

	inline VOID Enter(VOID)
	{
		EnterCriticalSection(&mSync);
	}

	inline VOID Leave(VOID)
	{
		LeaveCriticalSection(&mSync);
	}

private:
	CRITICAL_SECTION	mSync;
};