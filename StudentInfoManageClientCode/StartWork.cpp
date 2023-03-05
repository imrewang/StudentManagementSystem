#include "StartWork.h"
#include <stdio.h>
#include <string>

CStartWork::CStartWork()
{
}

CStartWork::~CStartWork()
{
}

// 启动数据库
bool CStartWork::StartDatabase()
{
    char szUser[32] = {0};
    char szPassword[32] = {0};

    printf("请输入数据库账号：");
    scanf("%s", szUser);

    printf("请输入数据库密码：");
    scanf("%s", szPassword);

    return m_StudentDatabase.InitDatabase(szUser, szPassword);
}

// 启动(创建、绑定、监听)Socket
bool CStartWork::StartSocket()
{
    char szIPAddr[32] = {0};
    USHORT sProt = 0;

    printf("请输入IP地址：");
    scanf("%s", szIPAddr);

    printf("请输入端口号：");
    scanf("%hd", &sProt);

    // 创建线程池
    m_MyThreadPool.CreateThreadPool();

    // 打开网络库
    if (!m_SockInit.Startup())
    {
        printf("打开网络库失败\r\n");
        return false;
    }

    // 创建socket
    if (!m_SockTcp.CreateSocket())
    {
        printf("创建socket失败\r\n");
        return false;
    }

    // 绑定socket
    if (!m_SockTcp.BindSocket(szIPAddr, sProt))
    {
        printf("绑定socket失败\r\n");
        return false;
    }

    // 监听socket
    if (!m_SockTcp.ListenSocket())
    {
        // 发送消息告诉界面
        printf("监听socket失败\r\n");
        return false;
    }

    // 创建线程，处理客户端数据
    HANDLE hThread = CreateThread(NULL, 0, ClentDataThread, this, 0, NULL);
    // CreateThread 第三个参数是函数指针, 新线程建立后将立即执行该函数, 函数执行完毕, 系统将销毁此线程从而结束多线程的故事.
    CloseHandle(hThread);

    printf("等待客户端连接\n");
    // 等待客户端连接
    while (true)
    {
        SOCKADDR_IN si;
        ZeroMemory(&si, sizeof(SOCKADDR_IN));
        SOCKET sClient = m_SockTcp.AcceptSocket(si);

        if (sClient == INVALID_SOCKET)
        {
            printf("客户端连接失败\r\n");
            return false;
        }

        m_CriticalSection.Enter();
        // 创建事件对象
        WSAEVENT hEvent = WSACreateEvent();

        m_ListToSockEvent.push_back(make_pair(sClient, hEvent));

        // 关联网络事件
        WSAEventSelect(sClient, hEvent, FD_READ | FD_CLOSE);
        m_CriticalSection.Leave();
    }
    printf("~~~~~~~等待客户端连接");
    return true;
}

// 处理客户端数据线程
DWORD WINAPI CStartWork::ClentDataThread(LPVOID lpParameter)
{
    CStartWork *pThis = (CStartWork *)lpParameter;

    while (true)
    {
        WSAEVENT AryEvent[WSA_MAXIMUM_WAIT_EVENTS] = {0};

        // 将vector中的数据放入AryEvent中
        pThis->m_CriticalSection.Enter();
        int nCount = 0;
        for (auto ListData : pThis->m_ListToSockEvent)
        {
            AryEvent[nCount] = ListData.second;
            nCount++;
        }
        pThis->m_CriticalSection.Leave();

        // 返回导致返回的事件对象
        DWORD dwRet = WSAWaitForMultipleEvents(nCount, AryEvent, FALSE, 1000, FALSE);

        if (dwRet == WSA_WAIT_FAILED || dwRet == WSA_WAIT_TIMEOUT)
        {
            continue;
        }

        // 找到对应的socket
        pThis->m_CriticalSection.Enter();
        for (list<pair<SOCKET, WSAEVENT>>::iterator it = pThis->m_ListToSockEvent.begin(); it != pThis->m_ListToSockEvent.end();)
        {
            if (it->second == AryEvent[dwRet - WSA_WAIT_EVENT_0])
            {
                WSANETWORKEVENTS NetWorkEvents;
                int nRet = WSAEnumNetworkEvents(it->first, it->second, &NetWorkEvents);
                if (nRet == SOCKET_ERROR)
                {
                    printf("WSAEnumNetworkEvents 失败\r\n");
                    return 0;
                }

                // 如果是可读事件发生
                if (NetWorkEvents.lNetworkEvents & FD_READ)
                { // 这里跳转void CMyThreadPool::InsertTask(CMyTask *pTask)进行m_Semaphore.Release(1);
                    pThis->m_MyThreadPool.InsertTask(new CMyTask(it->first, &pThis->m_SockTcp,
                                                                 &pThis->m_StudentDatabase,
                                                                 &pThis->m_ListToSockEvent,
                                                                 &pThis->m_CriticalSection));
                    it++;
                }
                // 如果是关闭事件发生
                else if (NetWorkEvents.lNetworkEvents & FD_CLOSE)
                {
                    it = pThis->m_ListToSockEvent.erase(it);
                }
                else
                {
                    it++;
                }
            }
            else
            {
                it++;
            }
        }
        pThis->m_CriticalSection.Leave();
    }

    return 0;
}