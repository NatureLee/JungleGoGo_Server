#include "stdafx.h"
#include "JYCriticalSection.h"
#include "JYMultiThreadSync.h"
#include "JYMemoryPool.h"
#include "JYStream.h"

JYStream::JYStream()
{
	mBufferPointer = NULL;
	mLength = 0;
}

JYStream::~JYStream()
{
}

//외부의 버퍼를 지정해 포인터 넣기
BOOL JYStream::SetBuffer(BYTE *buffer)
{
	if (!buffer)
		return FALSE;

	mBufferPointer = buffer;
	mLength = 0;

	return TRUE;
}

BOOL JYStream::SetBuffer()
{
	if (!mBufferPointer)
		return FALSE;

	mLength = 0;

	return TRUE;
}

//INT32형 데이터를 읽을 때 사용
BOOL JYStream::ReadInt32(INT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(INT));

	mLength += sizeof(INT);

	return TRUE;
}

//DWORD형 읽을때
BOOL JYStream::ReadDWORD(DWORD *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return TRUE;
}


BOOL JYStream::ReadDWORD_PTR(DWORD_PTR *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL JYStream::ReadByte(BYTE *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(BYTE));

	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL JYStream::ReadBytes(BYTE *data, DWORD length)
{
	memcpy(data, mBufferPointer + mLength, length);

	mLength += length;

	return TRUE;
}

BOOL JYStream::ReadFloat(FLOAT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL JYStream::ReadInt64(INT64 *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL JYStream::ReadSHORT(SHORT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL JYStream::ReadUSHORT(USHORT *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(USHORT));

	mLength += sizeof(USHORT);

	return TRUE;
}

BOOL JYStream::ReadBOOL(BOOL *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

BOOL JYStream::ReadWCHAR(WCHAR *data)
{
	memcpy(data, mBufferPointer + mLength, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return TRUE;
}

BOOL JYStream::ReadWCHARs(LPWSTR data, DWORD length)
{
	memcpy(data, mBufferPointer + mLength, length * sizeof(WCHAR) / 2);

	mLength += length * sizeof(WCHAR) / 2;

	return TRUE;
}

BOOL JYStream::WriteInt32(INT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(INT));

	mLength += sizeof(INT);

	return TRUE;
}

BOOL JYStream::WriteDWORD(DWORD data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(DWORD));

	mLength += sizeof(DWORD);

	return TRUE;
}

BOOL JYStream::WriteDWORD_PTR(DWORD_PTR data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(DWORD_PTR));

	mLength += sizeof(DWORD_PTR);

	return TRUE;
}

BOOL JYStream::WriteByte(BYTE data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(BYTE));

	mLength += sizeof(BYTE);

	return TRUE;
}

BOOL JYStream::WriteBytes(BYTE *data, DWORD length)
{
	memcpy(mBufferPointer + mLength, data, length);

	mLength += length;

	return TRUE;
}

BOOL JYStream::WriteFloat(FLOAT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(FLOAT));

	mLength += sizeof(FLOAT);

	return TRUE;
}

BOOL JYStream::WriteInt64(INT64 data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(INT64));

	mLength += sizeof(INT64);

	return TRUE;
}

BOOL JYStream::WriteSHORT(SHORT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(SHORT));

	mLength += sizeof(SHORT);

	return TRUE;
}

BOOL JYStream::WriteUSHORT(USHORT data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(USHORT));

	mLength += sizeof(USHORT);

	return TRUE;
}

BOOL JYStream::WriteBOOL(BOOL data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(BOOL));

	mLength += sizeof(BOOL);

	return TRUE;
}

BOOL JYStream::WriteWCHAR(WCHAR data)
{
	memcpy(mBufferPointer + mLength, &data, sizeof(WCHAR));

	mLength += sizeof(WCHAR);

	return TRUE;
}

BOOL JYStream::WriteWCHARs(LPCWSTR data, DWORD length)
{
	memcpy(mBufferPointer + mLength, data, length * sizeof(WCHAR));

	mLength += length * sizeof(WCHAR);

	return TRUE;
}

DWORD JYStream::GetLength(VOID)
{
	return mLength;
}