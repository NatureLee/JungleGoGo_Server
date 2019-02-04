#include "stdafx.h"
#include "JYCriticalSection.h"
#include "JYMultiThreadSync.h"
#include "JYCircularQueue.h"
#include "JYNetwork.h"
#include "JYPacketSession.h"


JYPacketSession::JYPacketSession(VOID)
{
	ZeroMemory(mPacketBuffer, sizeof(mPacketBuffer));
	//��Ʈ��ũ�� readBuffer���� �����͸� ������ ����

	//cPacketBuffer�� ��������
	mRemainLength = 0;
	mCurrentPacketNumber = 0;

	mLastReadPacketNumber = 0;
}

JYPacketSession::~JYPacketSession(VOID)
{
}

//��ü�� �����ϴ� �Լ�
BOOL JYPacketSession::Begin(VOID)
{
	JYThreadSync Sync;

	memset(mPacketBuffer, 0, sizeof(mPacketBuffer));

	mRemainLength = 0;
	mCurrentPacketNumber = 0;
	mLastReadPacketNumber = 0;

	//������ �����͸� ������ ť�� �ʱ�ȭ
	if (!WriteQueue.Begin())
		return FALSE;

	//��ӹ��� ��Ʈ��ũ �ʱ�ȭ
	return JYNetwork::Begin();
}

//��ü ���� �Լ�
BOOL JYPacketSession::End(VOID)
{
	JYThreadSync Sync;

	if (!WriteQueue.End())
		return FALSE;

	return JYNetwork::End();
}

//���� ��Ŷ �м�
BOOL JYPacketSession::GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength)
{
	JYThreadSync Sync;

	if (!packet)
		return FALSE;

	//4����Ʈ ���ϸ� false
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
		if (PacketNumber <= mLastReadPacketNumber) // ��Ŷ��ȣ�� �������̰ų� ���� ���(����) �ش� ��Ŷ�� �����Ѵ�.
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

//Network���� �����͸� �޾ƿ� (GetPacket������ ����)
// ReadPacketForIocp�� FALSE�� �������� ���� while���� ������.
BOOL JYPacketSession::ReadPacketForIocp(DWORD readLength)
{
	JYThreadSync Sync;

	if (!JYNetwork::ReadForIocp(mPacketBuffer + mRemainLength, readLength))
		return FALSE;

	mRemainLength += readLength;

	//return getPacket(protocol, packet, packetLength);
	return TRUE;
}

//��Ŷ�� ������ �Լ�
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

	//��Ŷ ����
	// ��Ŷ�� �ε����� �ٿ� ���� ������ �� �־�� �Ѵ�.
	memcpy(TempBuffer, &PacketLength, sizeof(DWORD));

	//��Ŷ ���� ���ֱ�
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/,
		&mCurrentPacketNumber, sizeof(DWORD));

	//��Ŷ ��ȣ ���ֱ�
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/,
		&protocol, sizeof(DWORD));

	//��������
	memcpy(TempBuffer +
		sizeof(DWORD)/*LENGTH*/ +
		sizeof(DWORD)/*PACKETNUMBER*/ +
		sizeof(DWORD)/*PROTOCOL*/,
		packet, packetLength);

	// WriteQueue�� �̿��ؼ� ��Ŷ�� ���� �Ϸᰡ �Ǿ������� �޸𸮸� ����д�.
	BYTE *WriteData = WriteQueue.Push(this, TempBuffer, PacketLength);

	return JYNetwork::Write(WriteData, PacketLength);
}

//������ �Ϸ� �� ť���� �����͸� ���ִ� �Լ�
BOOL JYPacketSession::WriteComplete(VOID)
{
	JYThreadSync Sync;

	// WriteQueue���� Pop�� �� �ָ� �ȴ�.
	return WriteQueue.Pop();
}