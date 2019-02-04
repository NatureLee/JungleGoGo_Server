#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>
#include <tchar.h>



#ifndef _WINSOCKAPI_
#define _WINSOCKAPI_

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#endif


#include <dbghelp.h>
#include <assert.h>
#include <time.h>

#include <vector>
#include <queue>
#include <list>
#include <map>
#include <string>
#include <utility>

#pragma warning(disable : 4996)

enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_WRITE
};

//오버랩 구조체
typedef struct _OVERLAPPED_EX
{
	OVERLAPPED	Overlapped;
	IO_TYPE		IoType;
	VOID		*Object;
} OVERLAPPED_EX;


#define MAX_BUFFER_LENGTH	4096

#include "JYLog.h"


//#import "C:\\Program Files\\Common Files\\System\\ADO\\msado15.dll" rename("EOF", "EndOfFile") no_namespace
