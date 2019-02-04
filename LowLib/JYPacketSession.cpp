#include "stdafx.h"
#include "JYCriticalSection.h"
#include "JYMultiThreadSync.h"
#include "JYCircularQueue.h"
#include "JYNetwork.h"
#include "JYPacketSession.h"


JYPacketSession::JYPacketSession(VOID)
{
	ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));
	//네트워크의 readBuffer에서 데이터를 가져올 버퍼

	//cPacketBuffer에 남은길이
	mRemainLength = 0;
	mCurrentPacketNumber = 0;

	mLastReadPacketNumber = 0;
}

JYPacketSession::~JYPacketSession(VOID)
{
}

//개체를 시작하는 함수
BOOL JYPacketSession::Begin(VOID)
{
	JYThreadSync Sync;

	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;

	//보내는 데이터를 저장할 큐를 초기화
	if (!WriteQueue.Begin())
		return FALSE;

	//상속받은 네트워크 초기화
	return JYNetwork::Begin();
}

//개체 종료 함수
BOOL JYPacketSession::End(VOID)
{
	JYThreadSync Sync;

	if (!WriteQueue.End())
		return FALSE;

	return JYNetwork::End();
}

//받은 패킷 분석
BOOL JYPacketSession::GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength)
{
	JYThreadSync Sync;

	if (!packet)
		return FALSE;

	//4바이트 이하면 false
	if (mRemainLength < sizeof(DWORD))
		return FALSE;

	INT PacketLength = 0;
	memcpy(&PacketLength, mPacketBuffer, sizeof(INT));

	if (PacketLength > MAX_BUFFER_LENGTH || PacketLength <= 0) // Invalid Packet
	{
		mRemainLength = 0;
		return FALSE;
	}

	if (PacketLength <= mRemainLength)
	{
		DWORD PacketNumber = 0;
		DWORD Protocol = 0;

		memcpy(&PacketNumber, mPacketBuffer + sizeof(DWORD)/*LENGTH*/, sizeof(DWORD));
		memcpy(&Protocol, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/, sizeof(DWORD));

		protocol = Protocol;
		packetLength = PacketLength -
			sizeof(DWORD)/*LENGTH*/ -
			sizeof(DWORD)/*PACKETNUMBER*/ -
			sizeof(DWORD)/*PROTOCOL*/;

		memcpy(packet, mPacketBuffer +
			sizeof(DWORD)/*LENGTH*/ +
			sizeof(DWORD)/*PACKETNUMBER*/ +
			sizeof(DWORD)/*PROTOCOL*/, packetLength);

		if (mRemainLength - PacketLength > 0)
			memmove(mPacketBuffer, mPacketBuffer + PacketLength, mRemainLength - PacketLength);

		mRemainLength -= PacketLength;

		if (mRemainLength <= 0)
		{
			mRemainLength = 0;
			memset(mPacketBuffer, 0, sizeof(mPacketBuffer));
		}
		/*
		if (PacketNumber <= mLastReadPacketNumber) // 패킷번호가 이전것이거나 같을 경우(복사) 해당 패킷은 무시한다.
		return FALSE;
		else
		{
		//PacketNumber++;
		mLastReadPacketNumber = PacketNumber;
		return TRUE;
		}*/
	}

	return TRUE;
}

//Network에서 데이터를 받아옴 (GetPacket이전에 실행)
// ReadPacketForIocp는 FALSE가 떨어질때 까지 while문을 돌린다.
BOOL JYPacketSession::ReadPacketForIocp(DWORD readLength)
{
	JYThreadSync Sync;

	if (!JYNetwork::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	mRemainLength += readLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

//패킷을 보내는 함수
BOOL JYPacketSession::WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength)
{
	JYThreadSync Sync;

	if (!packet)
		return FALSE;

	// LENGTH(4) | PACKET_NUMBER(4) | PROTOCOL(4) | DATA (4084)
	DWORD PacketLength = sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/ +
		packetLength;

	if (PacketLength >= MAX_BUFFER_LENGTH)
		return FALSE;

	mCurrentPacketNumber++;

	BYTE TempBuffer[MAX_BUFFER_LENGTH] = { 0, };

	//패킷 만듬
	// 패킷에 인덱스를 붙여 순서 보정을 해 주어야 한다.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	//패킷 길이 써주기
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/,
		&mCurrentPacketNumber, sizeof(DWORD));

	//패킷 번호 써주기
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/,
		&protocol, sizeof(DWORD));

	//프로토콜
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	// WriteQueue를 이용해서 패킷이 전송 완료가 되었을까지 메모리를 살려둔다.
	BYTE *WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return JYNetwork::Write(WriteData, PacketLength);
}

//보내기 완료 시 큐에서 데이터를 빼주는 함수
BOOL JYPacketSession::WriteComplete(VOID)
{
	JYThreadSync Sync;

	// WriteQueue에서 Pop을 해 주면 된다.
	return WriteQueue.Pop();
}