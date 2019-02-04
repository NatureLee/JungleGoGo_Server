// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <mysql.h>

#include <iostream>
using namespace std;


#include "../LowLib/Global.h"


#define DEFAULT_PORT					11550
#define MAX_USER						100

#define LOG_OK							_T("[ ok ]\n")
#define LOG_FAIL						_T("[ fail ]\n\n")


////유저 상태
   typedef enum USER_STATUS
   {
   	US_NONE = 0,
   	US_LOBBY_ENTER,
   	US_LOBBY_LEAVING,
   	US_GAME_START,
	US_GAME_LEAVING,
   	US_GAME_ENDING
   };

#define CONFIG_FILENAME								_T("./config.ini")

#include "../LowLib/JYCriticalSection.h"
#include "../LowLib/JYMultiThreadSync.h"
#include "../LowLib/JYCircularQueue.h"
#include "../LowLib/JYNetwork.h"
#include "../LowLib/JYPacketSession.h"
#include "../LowLib/JYIocp.h"
#include "../LowLib/JYStream.h"

#include "JYGameRoom.h"
#include "JYGRManager.h"
#include "JYConnectedUser.h"
#include "JYCUManager.h"
#include "JYGServerIocp.h"
#include "JYGameDatabase.h"
#include "JYGameProtocol.h"
