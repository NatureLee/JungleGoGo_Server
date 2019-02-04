#include "stdafx.h"




VOID JYGServerIocp::PROC_PT_INLOBBY(JYConnectedUser* connectedUser, BYTE *packet) 
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	S_PT_C_INLOBBY inLobby;
	READ_PT_C_INLOBBY(packet, inLobby);

	S_PT_S_RATE ratePack;

	string str(inLobby.nick);
	//printf("PROC_PT_INLOBBY");
	//printf("\n%s   ", inLobby.nick);

	//id pw 체크
	if (GDBSession->UpdateDCheck(str))
	{
		GDBSession->SelectRate(inLobby.nick, &ratePack.win, &ratePack.lose);
		connectedUser->WritePacket(PT_S_RATE, WriteBuffer, WRITE_PT_S_RATE(WriteBuffer, ratePack.win, ratePack.lose));
	}
	else
	{
		printf("UPDATEDOUBLECHECK ERROR\n");
	}

}

//게임 스타트 & 스타트 올
VOID JYGServerIocp::PROC_PT_GAMESTART(JYConnectedUser* connectedUser, BYTE *packet) 
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	USHORT SlotIndex = 0;
	JYGameRoom *Room = mRoomManager.QuickJoin(connectedUser, SlotIndex);

	//printf("PROC_PT_GAMESTART\n");

	if (Room) 
	{
		printf("In The Room\n");

		if (Room->GetIsFull())
		{
			//printf("SEND GAMESTARTALL\n");

			Room->mRoomUser[0]->WritePacket(PT_S_GAMESTARTALL,WriteBuffer,WRITE_PT_S_GAMESTARTALL(WriteBuffer,true));
			Room->mRoomUser[1]->WritePacket(PT_S_GAMESTARTALL, WriteBuffer, WRITE_PT_S_GAMESTARTALL(WriteBuffer,false)); //false를 줘서 드래곤팀

			Room->SetIsGameStarting(TRUE);

		}
		else {

			// 입장 성공 패킷을 보낸후에 
			//printf("SEND GAMESTART_SUCC\n");
			connectedUser->WritePacket(PT_S_GAMESTART_SUCC,
				WriteBuffer,
				WRITE_PT_S_GAMESTART_SUCC(WriteBuffer));
		}

	}
	else
	{
		connectedUser->WritePacket(PT_S_GAMESTART_FAIL,
			WriteBuffer,
			WRITE_PT_S_GAMESTART_FAIL(WriteBuffer));
	}

}

//정상종료 시
VOID JYGServerIocp::PROC_PT_ROOMOUT(JYConnectedUser* connectedUser, BYTE *packet) 
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };
	
	JYGameRoom *Room = connectedUser->GetEnteredRoom();

	if (Room)
	{
		Room->LeaveUser(FALSE, this, connectedUser);

		// 방 나가기 성공을 보내고
		connectedUser->WritePacket(PT_S_ROOMOUT_SUCC,
			WriteBuffer,
			WRITE_PT_S_ROOMLEAVE_SUCC(WriteBuffer));
	}

}

//기권 비정상 종료
VOID JYGServerIocp::PROC_PT_ROOMLEAVE(JYConnectedUser* connectedUser, BYTE *packet)
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	JYGameRoom *Room = connectedUser->GetEnteredRoom();

	if (Room)
	{
		if (Room->mRoomUser[0] == connectedUser) {
			Room->mRoomUser[1]->WritePacket(PT_S_ROOMLEAVE_SUCC, WriteBuffer, WRITE_PT_S_ROOMOUT_SUCC(WriteBuffer));
		}
		else if (Room->mRoomUser[1] == connectedUser) {
			Room->mRoomUser[0]->WritePacket(PT_S_ROOMLEAVE_SUCC, WriteBuffer, WRITE_PT_S_ROOMOUT_SUCC(WriteBuffer));
		}

		Room->LeaveUser(FALSE, this, connectedUser);
	}

}

//턴 바꿔주기
VOID JYGServerIocp::PROC_PT_MOVETURN(JYConnectedUser *connectedUser, BYTE *packet) 
{
	BYTE	WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	JYGameRoom *Room = connectedUser->GetEnteredRoom();

	if (Room) {

		if (Room->mRoomUser[0] == connectedUser) {
			Room->mRoomUser[1]->WritePacket(PT_S_MOVETURN, WriteBuffer, WRITE_PT_S_MOVETURN(WriteBuffer));
		}
		else if (Room->mRoomUser[1] == connectedUser) {
            Room->mRoomUser[0]->WritePacket(PT_S_MOVETURN, WriteBuffer, WRITE_PT_S_MOVETURN(WriteBuffer));
		}
	}

}


//위치 바꿔주기
VOID JYGServerIocp::PROC_PT_MOVE(JYConnectedUser *connectedUser, BYTE *packet)
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	S_PT_C_MOVE data;
	READ_PT_C_MOVE (packet, data);
	
	S_PT_S_MOVE data2;
	data2.x1 = data.x1; data2.y1 = data.y1; data2.x2 = data.x2; data2.y2 = data.y2;

	JYGameRoom *Room = connectedUser->GetEnteredRoom();
	if (Room) {

		if (Room->mRoomUser[0] == connectedUser) {

			Room->mRoomUser[1]->WritePacket(PT_S_MOVE, WriteBuffer, WRITE_PT_S_MOVE(WriteBuffer, data2.x1, data2.y1, data2.x2, data2.y2));
		}
		else if (Room->mRoomUser[1] == connectedUser) {

			Room->mRoomUser[0]->WritePacket(PT_S_MOVE, WriteBuffer, WRITE_PT_S_MOVE(WriteBuffer, data2.x1, data2.y1, data2.x2, data2.y2));
		}
	}
}

VOID JYGServerIocp::PROC_PT_PLAYERNICK(JYConnectedUser *connectedUser, BYTE *packet) 
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	S_PT_C_PLAYERNICK name1;
	READ_PT_C_PLAYERNICK(packet, name1);
	
	JYGameRoom *Room = connectedUser->GetEnteredRoom();

	if (Room) {

			if (Room->mRoomUser[0] == connectedUser) {
				Room->mRoomUser[1]->WritePacket(PT_S_PLAYERNICK, WriteBuffer, WRITE_PT_S_PLAYERNICK(WriteBuffer, name1.MyNick, name1.MyRate));
			}
			else if (Room->mRoomUser[1] == connectedUser) {
				Room->mRoomUser[0]->WritePacket(PT_S_PLAYERNICK, WriteBuffer, WRITE_PT_S_PLAYERNICK(WriteBuffer, name1.MyNick, name1.MyRate));
			}

	}
}

VOID JYGServerIocp::PROC_PT_WIN(JYConnectedUser *connectedUser, BYTE *packet) 
{
	BYTE WriteBuffer[MAX_BUFFER_LENGTH] = { 0, };

	S_PT_C_WINNER winner;
	READ_PT_C_WINNER(packet, winner);

	JYGameRoom *Room = connectedUser->GetEnteredRoom();

	if (Room)
	{
		Room->WriteAll(PT_C_WINNER, WriteBuffer, WRITE_PT_C_WINNER(WriteBuffer, winner.winner,winner.loser));
	}
	
	
	if (GDBSession->ChangeRate(winner.winner,winner.loser))
	{
		printf("ChangeRate Succ\n");
	}
	else
	{
		printf("ChangeRate Fail\n");
	}

}