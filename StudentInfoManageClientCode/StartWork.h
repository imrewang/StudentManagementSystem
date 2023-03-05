#pragma once
#include "StudentDataBase.h"
#include "SocketInit.h"
#include "SocketTcp.h"
#include "CriticalSections.h"
#include "MyThreadPool.h"

using namespace std;

class CStartWork
{
public:
    // 构造
    CStartWork();

    // 析构
    ~CStartWork();

    // 启动数据库
    bool StartDatabase();

    // 启动(创建、绑定、监听)Socket
    bool StartSocket();

private:
    // 处理客户端数据线程
    static DWORD WINAPI ClentDataThread(LPVOID lpParameter);

private:
    CStudentDataBase m_StudentDatabase; //
    CSocketInit m_SockInit;
    CSocketTcp m_SockTcp;
    CCriticalSections m_CriticalSection;
    CMyThreadPool m_MyThreadPool;
    list<pair<SOCKET, WSAEVENT>> m_ListToSockEvent;
};
