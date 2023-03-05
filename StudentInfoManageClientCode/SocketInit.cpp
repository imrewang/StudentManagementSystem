#include "SocketInit.h"

// 构造
CSocketInit::CSocketInit()
{
}

// 析构
CSocketInit::~CSocketInit()
{
    Cleanup();
}

// 打开网络库
bool CSocketInit::Startup()
{
    // 启动网络库
    WSADATA wsaData = {0};

    int nRet = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
    // 使用Socket的程序在使用Socket之前必须调用WSAStartup函数。以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。

    if (nRet != 0)
    {
        return FALSE;
    }

    // 版本校验
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        Cleanup(); // 关闭网络库
        return FALSE;
    }

    return TRUE;
}

// 关闭网络库
bool CSocketInit::Cleanup()
{
    int nRet = ::WSACleanup();

    if (nRet != 0)
    {
        return FALSE;
    }

    return TRUE;
}
