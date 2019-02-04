// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
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

