#pragma once

//network 상속받아 패킷을 추가
class JYPacketSession : public JYNetwork
{
public:
	JYPacketSession(VOID);
	virtual ~JYPacketSession(VOID);

private:
	BYTE								mPacketBuffer[MAX_BUFFER_LENGTH * 3];
	INT									mRemainLength;
	DWORD								mCurrentPacketNumber;

	DWORD								mLastReadPacketNumber;

	JYCircularQueue						WriteQueue;


public:
	BOOL	Begin(VOID);
	BOOL	End(VOID);

	BOOL	ReadPacketForIocp(DWORD readLength);
	BOOL	ReadPacketForEventSelect(VOID);

	BOOL	ReadFromPacketForIocp(LPSTR remoteAddress, USHORT &remotePort, DWORD readLength);
	BOOL	ReadFromPacketForEventSelect(LPSTR remoteAddress, USHORT &remotePort);

	BOOL	WritePacket(DWORD protocol, const BYTE *packet, DWORD packetLength);
	BOOL	WriteToPacket(LPCSTR remoteAddress, USHORT remotePort, DWORD protocol, const BYTE *packet, DWORD packetLength);
	BOOL	WriteComplete(VOID);

	BOOL	GetPacket(DWORD &protocol, BYTE *packet, DWORD &packetLength);
	BOOL	GetPacket(LPSTR remoteAddress, USHORT remotePort, DWORD &protocol, BYTE *packet, DWORD &packetLength);
};


