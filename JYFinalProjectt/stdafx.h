// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

//#include <stdio.h>
//#include <tchar.h>
#include <mysql.h>

#include <iostream>
using namespace std;

#pragma once

#define MAX_USER 1000
#define DEFAULT_PORT 6535

#include "../LowLib/Global.h"

#include "../LowLib/JYCriticalSection.h"
#include "../LowLib/JYMultiThreadSync.h"
#include "../LowLib/JYCircularQueue.h"
#include "../LowLib/JYNetwork.h"
#include "../LowLib/JYPacketSession.h"
#include "../LowLib/JYIocp.h"
#include "../LowLib/JYStream.h"
#include "../LowLib/JYLog.h"
#include "../LowLib/JYMemoryPool.h"

#include "JYProtocol.h"
#include "JYConnectedSession.h"
#include "JYCSManager.h"
#include "JYServerIocp.h"
#include "JYDataBase.h"

