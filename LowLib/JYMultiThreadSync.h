#pragma once

template <class T>
class JYMultiThreadSync
{
	friend class JYThreadSync;

public:

	class JYThreadSync
	{
	public:
		JYThreadSync(VOID)
		{
			T::mSync.Enter();
		}

		~JYThreadSync(VOID)
		{
			T::mSync.Leave();
		}
	};

private:
	static JYCriticalSection mSync;
};

template <class T>
JYCriticalSection  JYMultiThreadSync<T>::mSync;