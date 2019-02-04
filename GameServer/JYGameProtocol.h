#pragma once

typedef enum _GProtocol
{
	//게임
	PT_C_INLOBBY = 0,
	PT_S_RATE,
	PT_C_GAMESTART,
	PT_S_GAMESTART_SUCC,
	PT_S_GAMESTART_FAIL,
	PT_S_GAMESTARTALL,
	PT_C_ROOMLEAVE,
	PT_S_ROOMLEAVE_SUCC,
	PT_C_ROOMOUT,
	PT_S_ROOMOUT_SUCC,
	PT_C_PLAYERNICK,
	PT_S_PLAYERNICK,


	//인게임
	PT_C_MOVETURN = 100,
	PT_S_MOVETURN,
	PT_C_MOVE,
	PT_S_MOVE,
	PT_C_WINNER,
	PT_C_GETMYRATE

};


//클라에서 받는 게임 스타트 패킷

typedef struct _S_PT_C_INLOBBY
{
	char nick[32];
} S_PT_C_INLOBBY;

inline VOID READ_PT_C_INLOBBY(BYTE *buffer, S_PT_C_INLOBBY &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs((LPWSTR)parameter.nick, 32);
}

typedef struct _S_PT_S_RATE
{
	int win;
	int lose;
} S_PT_S_RATE;

inline DWORD WRITE_PT_S_RATE(BYTE *buffer,int win,int lose)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt32(win);
	Stream->WriteInt32(lose);

	return Stream->GetLength();
}

typedef struct _S_PT_C_GAMESTART
{
} S_PT_C_GAMESTART;

inline VOID READ_PT_C_GAMERESULT(BYTE *buffer, S_PT_C_GAMESTART &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);
}


typedef struct _S_PT_S_GAMESTART_SUCC
{
} S_PT_S_GAMESTART_SUCC;

inline DWORD WRITE_PT_S_GAMESTART_SUCC(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}

typedef struct _S_PT_S_GAMESTART_FAIL
{

} S_PT_S_GAMESTART_FAIL;

inline DWORD WRITE_PT_S_GAMESTART_FAIL(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}

//유저 모두 게임 스타트 시 시작 알리는 패킷
//턴을 정해줌
typedef struct _S_PT_S_GAMESTARTALL
{
	bool isturn;

} S_PT_S_GAMESTARTALL;

inline DWORD WRITE_PT_S_GAMESTARTALL(BYTE *buffer, BOOL turn)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteBOOL(turn);

	return Stream->GetLength();
}

typedef struct _S_PT_C_ROOMLEAVE
{

} S_PT_C_ROOMLEAVE;

inline VOID READ_PT_C_ROOMLEAVE(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);
}

typedef struct _S_PT_S_ROOMLEAVE_SUCC
{

} S_PT_S_ROOMLEAVE_SUCC;

inline DWORD WRITE_PT_S_ROOMLEAVE_SUCC(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}

typedef struct _S_PT_C_ROOMOUT
{

} S_PT_C_ROOMOUT;

inline VOID READ_PT_C_ROOMOUT(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);
}

typedef struct _S_PT_S_ROOMOUT_SUCC
{

} S_PT_S_ROOMOUT_SUCC;

inline DWORD WRITE_PT_S_ROOMOUT_SUCC(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}

typedef struct _S_PT_C_MOVETURN
{

} S_PT_C_MOVETURN;

inline VOID READ_PT_C_MOVETURN(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);
}

typedef struct _S_PT_S_MOVETURN
{

} S_PT_S_MOVETURN;

inline DWORD WRITE_PT_S_MOVETURN(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}

typedef struct _S_PT_S_MOVE
{
	int x1;
	int y1;
	int x2;
	int y2;

} S_PT_S_MOVE;

inline DWORD WRITE_PT_S_MOVE(BYTE *buffer, int x1, int y1, int x2, int y2)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt32(x1);
	Stream->WriteInt32(y1);
	Stream->WriteInt32(x2);
	Stream->WriteInt32(y2);

	return Stream->GetLength();
}

typedef struct _S_PT_C_MOVE
{
	int x1;
	int y1;
	int x2;
	int y2;

} S_PT_C_MOVE;

inline VOID READ_PT_C_MOVE(BYTE *buffer, S_PT_C_MOVE &parameter)
{
	JYStreamSP Stream;

	Stream->SetBuffer(buffer);

	Stream->ReadInt32(&parameter.x1);
	Stream->ReadInt32(&parameter.y1);
	Stream->ReadInt32(&parameter.x2);
	Stream->ReadInt32(&parameter.y2);
}

typedef struct _S_PT_C_PLAYERNICK
{
	char MyNick[32];
	float  MyRate;

} S_PT_C_PLAYERNICK;

inline VOID READ_PT_C_PLAYERNICK(BYTE *buffer, S_PT_C_PLAYERNICK &parameter)
{
	JYStreamSP Stream;

	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs((LPWSTR)parameter.MyNick, 32);
	Stream->ReadFloat(&parameter.MyRate);

}

//상대방 정보
typedef struct _S_PT_S_PLAYERNICK
{
	char OtherNick[32];
	float  OtherRate;

} S_PT_S_PLAYERNICK;

inline DWORD WRITE_PT_S_PLAYERNICK(BYTE *buffer, char * name,float rate)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs((LPWSTR)name, 32);
	Stream->WriteFloat(rate);

	return Stream->GetLength();
}

typedef struct _S_PT_C_GETMYRATE
{
	int winningRate;

} S_PT_S_GETMYRATE;

inline DWORD WRITE_PT_C_GETMYRATE(BYTE *buffer, int winningRate)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt32(winningRate);

	return Stream->GetLength();
}


typedef struct _S_PT_C_WINNER
{
	char winner[32];
	char loser[32];

} S_PT_C_WINNER;

inline VOID READ_PT_C_WINNER(BYTE *buffer, S_PT_C_WINNER &parameter)
{
	JYStreamSP Stream;

	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs((LPWSTR)parameter.winner, 32);
	Stream->ReadWCHARs((LPWSTR)parameter.loser, 32);
}

inline DWORD WRITE_PT_C_WINNER(BYTE *buffer, char * name1,char * name2)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs((LPWSTR)name1, 32);
	Stream->WriteWCHARs((LPWSTR)name2, 32);

	return Stream->GetLength();
}

typedef struct _S_PT_S_ROOMLEAVE
{

} S_PT_S_ROOMLEAVE;

inline DWORD WRITE_PT_S_ROOMLEAVE(BYTE *buffer)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}
