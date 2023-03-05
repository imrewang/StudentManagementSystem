#include "stdafx.h"
#include "SocketInit.h"
#include < Winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

//构造
CSocketInit::CSocketInit()
{
}

//析构
CSocketInit::~CSocketInit()
{
    Cleanup();
}

//打开网络库
bool CSocketInit::Startup()
{
    //启动网络库
    WSADATA wsaData = { 0 };
    int nRet = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (nRet != 0)
    {
        return false;
    }

    //版本校验
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        Cleanup();
        return false;
    }

    return true;
}

//关闭网络库
bool CSocketInit::Cleanup()
{
    int nRet = ::WSACleanup();

    if (nRet != 0)
    {
        return false;
    }

    return true;
}
