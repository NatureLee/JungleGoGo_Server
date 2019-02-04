#include "stdafx.h"

DWORD WINAPI KeepThreadCallback(LPVOID parameter)
{
	JYGServerIocp *Owner = (JYGServerIocp*)parameter;
	Owner->KeepThreadCallback();

	return 0;
}

DWORD WINAPI GameThreadCallback(LPVOID parameter)
{
	JYGServerIocp *Owner = (JYGServerIocp*)parameter;
	Owner->GameThreadCallback();

	return 0;
}

JYGServerIocp::JYGServerIocp(VOID)
{
}

JYGServerIocp::~JYGServerIocp(VOID)
{
}

VOID JYGServerIocp::KeepThreadCallback(VOID)
{
	DWORD KeepAlive = 0xFFFF;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mKeepThreadDestroyEvent, 30000);

		if (Result == WAIT_OBJECT_0)
			return;

		//mConnectedUserManager.WriteAll(0x3000000, (BYTE*)&KeepAlive, sizeof(DWORD));
	}
}

VOID JYGServerIocp::GameThreadCallback(VOID)
{
	// NEW_UPDATE
	//return;

	while (TRUE)
	{
		DWORD Result = WaitForSingleObject(mGameThreadDestroyEvent, 1000);

		if (Result == WAIT_OBJECT_0)
			return;
	}
}

BOOL JYGServerIocp::Begin(VOID)
{

	mListenSession = new JYNetwork();

	if (!JYIocp::Begin())
	{
		//CLog::WriteLogNoDate(_T("CIocp::Begin"));

		End();

		return FALSE;
	}

	//printf("ListenSession BEGIN\n");
	if (!mListenSession->Begin())
	{
		//CLog::WriteLogNoDate(_T("ListenSession.Begin"));

		End();

		return FALSE;
	}

	//printf("Bind SUCCESS\n");
	if (!mListenSession->TcpBind())
	{
		//CLog::WriteLogNoDate(_T("ListenSession.TcpBind"));

		End();

		return FALSE;
	}

	//printf("Lisetn SUCCESS \n");
	if (!mListenSession->Listen(DEFAULT_PORT, MAX_USER))
	{
		//CLog::WriteLogNoDate(_T("ListenSession.Listen"));

		End();

		return FALSE;
	}

	// IOCP에 Listen 소켓을 등록시켜 줍니다. 그리고 키로는 해당 개체의 포인터를 설정합니다.
	//printf("RegisterSocket SUCCESS\n");
	if (!JYIocp::RegisterSocketToIocp(mListenSession->GetSocket(), (ULONG_PTR)&mListenSession))
	{
		//CLog::WriteLogNoDate(_T("CIocp::RegisterSocketToIocp"));

		End();

		return FALSE;
	}


	//데이터베이스
     //printf("InitDB SUCCESS\n");
     if (!GDBSession->InitDB())
     {
     	// 실패했을 경우 End함수를 호출하고 종료합니다.
     	JYGServerIocp::End();
     
     	return FALSE;
     }

	//printf("ConnectedUser Manager Start\n");
	if (!mConnectedUserManager.Begin(MAX_USER, mListenSession->GetSocket()))
	{
		//CLog::WriteLogNoDate(_T("ConnectedUserManager.Begin"));

		End();

		return FALSE;
	}

	//printf("Room Manager Start\n");
	if (!mRoomManager.Begin(MAX_USER))
	{
		//CLog::WriteLogNoDate(_T("RoomManager.Begin"));

		End();

		return FALSE;
	}

	//printf("ConnectedUser Accept START\n");
	if (!mConnectedUserManager.AcceptAll())
	{
		//CLog::WriteLogNoDate(_T("ConnectedUserManager.AllAccept"));

		End();

		return FALSE;
	}

	mKeepThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!mKeepThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	mKeepThreadHandle = CreateThread(NULL, 0, ::KeepThreadCallback, this, 0, NULL);
	if (!mKeepThreadHandle)
	{
		End();

		return FALSE;
	}

	mGameThreadDestroyEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!mGameThreadDestroyEvent)
	{
		End();

		return FALSE;
	}

	mGameThreadHandle = CreateThread(NULL, 0, ::GameThreadCallback, this, 0, NULL);
	if (!mGameThreadHandle)
	{
		End();

		return FALSE;
	}


	return TRUE;
}

BOOL JYGServerIocp::End(VOID)
{
	if (mKeepThreadDestroyEvent && mKeepThreadHandle)
	{
		SetEvent(mKeepThreadDestroyEvent);

		WaitForSingleObject(mKeepThreadHandle, INFINITE);

		CloseHandle(mKeepThreadDestroyEvent);
		CloseHandle(mKeepThreadHandle);
	}

	if (mGameThreadDestroyEvent && mGameThreadHandle)
	{
		SetEvent(mGameThreadDestroyEvent);

		WaitForSingleObject(mGameThreadHandle, INFINITE);

		CloseHandle(mGameThreadDestroyEvent);
		CloseHandle(mGameThreadHandle);
	}

	JYIocp::End();

	mRoomManager.End();

	mConnectedUserManager.End();

	mListenSession->End();

	// 데이터베이스는 가장 마지막에 종료
	//mDataBase.End();

	return TRUE;
}


VOID JYGServerIocp::OnIoConnected(VOID *object)
{
	JYConnectedUser *ConnectedUser = (JYConnectedUser*)object;

	printf("---------Connected GameServer--------\n");


	//ConnectedUser->GetRemoteAddressAfterAccept(RemoteAddress, RemotePort);
	//ConnectedUser->SetRealInformation(RemoteAddress, RemotePort);

	if (!JYIocp::RegisterSocketToIocp(ConnectedUser->GetSocket(), (ULONG_PTR)ConnectedUser))
	{
		//CLog::WriteLog(_T("! OnIoConnected : CIocp::RegisterSocketToIocp"));

		End();

		return;
	}

	ConnectedUser->SetIsConnected(TRUE);

	if (!ConnectedUser->InitializeReadForIocp())
	{
		if (!ConnectedUser->Reload(mListenSession->GetSocket()))
		{
			//CLog::WriteLog(_T("! OnIoConnected : ConnectedUser->Reload"));

			End();

			return;
		}
	}

	mConnectedUserManager.IncreaseConnectedUserCount();

	//CLog::WriteLog(_T("# New client connected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

VOID JYGServerIocp::OnIoDisconnected(VOID *object)
{
	JYConnectedUser *ConnectedUser = (JYConnectedUser*)object;
	printf("---------DisConnected GameServer--------\n");

	JYGameRoom *room = ConnectedUser->GetEnteredRoom();

	if (room != NULL)
	{
		BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
		if (room->GetIsFull())
		{
			if (room->mRoomUser[0] == ConnectedUser) {
				room->mRoomUser[1]->WritePacket(PT_S_ROOMLEAVE_SUCC, WriteBuffer, WRITE_PT_S_ROOMLEAVE_SUCC(WriteBuffer));
			}
			else if (room->mRoomUser[1] == ConnectedUser) {
				room->mRoomUser[0]->WritePacket(PT_S_ROOMLEAVE_SUCC, WriteBuffer, WRITE_PT_S_ROOMLEAVE_SUCC(WriteBuffer));
			}
		}

		room->LeaveUser(TRUE, this, ConnectedUser);
	}

	//disconnected
	if (!ConnectedUser->Reload(mListenSession->GetSocket()))
	{
		//CLog::WriteLog(_T("! OnIoDisconnected : ConnectedUser->Reload"));

		End();

		return;
	}

	mConnectedUserManager.DecreaseConnectedUserCount();

	//CLog::WriteLog(_T("# Client disconnected : 0x%x(0x%x)\n"), ConnectedUser, ConnectedUser->GetSocket());
}

VOID JYGServerIocp::OnIoRead(VOID *object, DWORD dataLength)
{
	JYConnectedUser *ConnectedUser = (JYConnectedUser*)object;

	DWORD	Protocol = 0;
	BYTE	Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD	PacketLength = 0;

	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	if (ConnectedUser->ReadPacketForIocp(dataLength))
	{
		//printf("Read Packet SUCCESS\n");
		//cout << "DATA LENGTH : " << dataLength << endl;

		// 패킷을 뽑아 냅니다.
		while (ConnectedUser->GetPacket(Protocol, Packet, PacketLength))
		{
			//printf("GetPAcket SUCCESS\n");
			////프로토콜 뭔지
			cout << "PROTOCOL	:	" << Protocol << endl;

			// 프로토콜에 따른 switch 문
			switch (Protocol)
			{
			case PT_C_INLOBBY:
				PROC_PT_INLOBBY(ConnectedUser, Packet);
				break;
				// 게임 시작 왔을때
			case PT_C_GAMESTART:
				PROC_PT_GAMESTART(ConnectedUser, Packet);
				break;

			case PT_C_ROOMLEAVE:
				PROC_PT_ROOMLEAVE(ConnectedUser, Packet);
				break;

			case PT_C_ROOMOUT:
				PROC_PT_ROOMOUT(ConnectedUser, Packet);
				break;

			case PT_C_MOVETURN:
				PROC_PT_MOVETURN(ConnectedUser, Packet);
				break;

			case PT_C_MOVE:
				PROC_PT_MOVE(ConnectedUser, Packet);
				break;

			case PT_C_PLAYERNICK:
				PROC_PT_PLAYERNICK(ConnectedUser, Packet);
				break;

			case PT_C_WINNER:
				PROC_PT_WIN(ConnectedUser, Packet);
				break;
			}
		}
	}

	if (!ConnectedUser->InitializeReadForIocp())
		ConnectedUser->End();
	// TODO : 강제 종료부
	// End는 꼭 이후에
	// ConnectedUser->End();
}

VOID JYGServerIocp::OnIoWrote(VOID *object, DWORD dataLength)
{
	JYConnectedUser *ConnectedUser = (JYConnectedUser*)object;

	ConnectedUser->WriteComplete();

#ifdef _FULL_DEBUG
	CLog::WriteLog(_T("# Client data wrote : 0x%x(0x%x)(%d)\n"), ConnectedUser, ConnectedUser->GetSocket(), dataLength);
#endif
}