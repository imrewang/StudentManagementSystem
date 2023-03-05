#include "stdafx.h"
#include "SocketInit.h"
#include < Winsock2.h>

#pragma comment(lib, "Ws2_32.lib")

//����
CSocketInit::CSocketInit()
{
}

//����
CSocketInit::~CSocketInit()
{
    Cleanup();
}

//�������
bool CSocketInit::Startup()
{
    //���������
    WSADATA wsaData = { 0 };
    int nRet = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (nRet != 0)
    {
        return false;
    }

    //�汾У��
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
    {
        Cleanup();
        return false;
    }

    return true;
}

//�ر������
bool CSocketInit::Cleanup()
{
    int nRet = ::WSACleanup();

    if (nRet != 0)
    {
        return false;
    }

    return true;
}
