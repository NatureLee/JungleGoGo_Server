#pragma once

typedef enum _Protocol
{
	PT_LOGIN = 0,
	PT_LOGIN_SUCC,
	PT_LOGIN_ERROR,

	PT_MAKING = 10,
	PT_MAKING_SUCC,
	PT_MAKING_ERROR
};

//LOGIN PACKET

//클라에서 받는 패킷
typedef struct _S_PT_LOGIN
{
	char USER_ID[32];
	char USER_PW[32];
} S_PT_LOGIN;

inline VOID READ_PT_LOGIN(BYTE *buffer, S_PT_LOGIN &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs((LPWSTR)parameter.USER_ID, 32);
	Stream->ReadWCHARs((LPWSTR)parameter.USER_PW, 32);
}

inline DWORD WRITE_PT_LOGIN(BYTE *buffer, S_PT_LOGIN &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs((LPWSTR)parameter.USER_ID, 32);
	Stream->WriteWCHARs((LPWSTR)parameter.USER_PW, 32);

	return Stream->GetLength();
}

//id,pw가 있을때 
typedef struct _S_PT_LOGIN_SUCC
{
	char NICKNAME[32];

} S_PT_LOGIN_SUCC;

inline DWORD WRITE_PT_LOGIN_SUCC(BYTE *buffer, S_PT_LOGIN_SUCC &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteWCHARs((LPWSTR)parameter.NICKNAME, 32);

	return Stream->GetLength();
}

//없을때 
typedef struct _S_PT_LOGIN_ERROR
{
	int error;

} S_PT_LOGIN_ERROR;

inline DWORD WRITE_PT_LOGIN_ERROR(BYTE *buffer, S_PT_LOGIN_ERROR &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt32((INT)parameter.error);

	return Stream->GetLength();
}

inline DWORD WRITE_PT_LOGIN_ERROR1 (BYTE *buffer, int error)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->WriteInt32(error);

	return Stream->GetLength();
}

//유저 정보 만들기
typedef struct _S_PT_MAKING
{
	char USER_ID[32];
	char USER_PW[32];
	char NICKNAME[32];
} S_PT_MAKING;

inline VOID READ_PT_MAKING(BYTE *buffer, S_PT_MAKING &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	Stream->ReadWCHARs((LPWSTR)parameter.USER_ID, 32);
	Stream->ReadWCHARs((LPWSTR)parameter.USER_PW, 32);
	Stream->ReadWCHARs((LPWSTR)parameter.NICKNAME, 32);
}

//만들기 성공시
typedef struct _S_PT_MAKING_SUCC
{
} S_PT_MAKING_SUCC;

inline DWORD WRITE_PT_MAKING_SUCC(BYTE *buffer, S_PT_MAKING_SUCC &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);

	return Stream->GetLength();
}

inline VOID READ_PT_MAKING_SUCC(BYTE *buffer, S_PT_MAKING &parameter)
{
	JYStreamSP Stream;
	Stream->SetBuffer(buffer);
}

//실패시
typedef struct _S_PT_MAKING_ERROR
{
} S_PT_MAKING_ERROR;

inline DWORD WRITE_PT_MAKING_ERROR(BYTE *buffer, S_PT_MAKING_ERROR &parameter)
{
	JYStreamSP Stream;

	return Stream->GetLength();
}