#include "MyTask.h"

CMyTask::CMyTask()
{
}

CMyTask::CMyTask(
    SOCKET ClientSocket,
    CSocketTcp *pSockTcp,
    CStudentDataBase *pStudentDatabase,
    list<pair<SOCKET, WSAEVENT>> *pListToSockEvent,
    CCriticalSections *pCriticalSection)
{
    m_ClientSocket = ClientSocket;
    m_pSockTcp = pSockTcp;
    m_pStudentDatabase = pStudentDatabase;
    m_pListToSockEvent = pListToSockEvent;
    m_pCriticalSection = pCriticalSection;
}

CMyTask::~CMyTask()
{
}

void CMyTask::Execute()
{
    HEAD_INFO head;

    // 接收头数据
    int nRet = m_pSockTcp->Recv(m_ClientSocket, (char *)&head, sizeof(head));
    // printf("接收头数据\n");
    //  printf("接收头数据m_Cmd:%d\n", head.m_Cmd);
    //  printf("接收头数据m_Len:%d\n", head.m_Len);

    if (nRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() == 10035)
        {
            return;
        }
        printf("接收数据错误\r\n");
        return;
    }

    // 匹配数据
    Matching(head);

    // 释放
    Relese();
}

// 匹配数据
void CMyTask::Matching(HEAD_INFO head)
{
    switch (head.m_Cmd)
    {
    case OP_INSERT:
    {
        MatchingInsert(head);
        // printf("MatchingInsert\n");
        break;
    }
    case OP_DELETE:
    {
        MatchingDelete(head);
        // printf("MatchingDelete\n");
        break;
    }
    case OP_UPDATE:
    {
        MatchingUpdate(head);
        // printf("MatchingUpdate\n");
        break;
    }
    case OP_QUERY:
    {
        MatchingQuery(head);
        // printf("MatchingQuery\n");
        break;
    }
    }
}

// 匹配到增加
void CMyTask::MatchingInsert(HEAD_INFO head)
{
    // 申请空间
    char *pBuff = new char[head.m_Len];
    // 初始化申请的空间
    ZeroMemory(pBuff, head.m_Len);

    // 接收数据
    int nRet = m_pSockTcp->Recv(m_ClientSocket, pBuff, head.m_Len);
    // printf("接收数据\n");
    //  printf("接收数据:%d\n", head.m_Cmd);
    //  printf("接收数据:%d\n", head.m_Len);

    if (nRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() == 10035)
        {
            delete[] pBuff;
            return;
        }
        printf("接收数据错误\r\n");
        delete[] pBuff;
        return;
    }

    HEAD_INFO SendHead;

    // 数据库执行操作
    nRet = m_pStudentDatabase->ExecutesInstructions(pBuff);
    if (nRet != 0)
    {
        if (WSAGetLastError() == 10035)
        {
            delete[] pBuff;
            return;
        }

        // 发送操作没有成功
        SendHead.m_Cmd = OP_INSERTERROR;
        SendHead.m_Len = 0;

        m_pSockTcp->Send(m_ClientSocket, (char *)&SendHead, sizeof(HEAD_INFO));

        delete[] pBuff;

        return;
    }

    // 发送操作成功
    SendHead.m_Cmd = OP_INSERTOK;
    SendHead.m_Len = 0;

    m_pSockTcp->Send(m_ClientSocket, (char *)&SendHead, sizeof(HEAD_INFO));

    delete[] pBuff;

    // 通知所有客户端，缓存失效
    NoticeClientCache();
}

// 匹配到删除
void CMyTask::MatchingDelete(HEAD_INFO head)
{
    // 申请空间
    char *pBuff = new char[head.m_Len];
    // 初始化申请的空间
    ZeroMemory(pBuff, head.m_Len);

    // 接收数据
    int nRet = m_pSockTcp->Recv(m_ClientSocket, pBuff, head.m_Len);

    if (nRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() == 10035)
        {
            delete[] pBuff;
            return;
        }
        printf("接收数据错误\r\n");
        delete[] pBuff;
        return;
    }

    HEAD_INFO SendHead;

    // 数据库执行操作
    nRet = m_pStudentDatabase->ExecutesInstructions(pBuff);
    if (nRet != 0)
    {
        if (WSAGetLastError() == 10035)
        {
            delete[] pBuff;
            return;
        }

        // 发送操作没有成功
        SendHead.m_Cmd = OP_DELETEERROR;
        SendHead.m_Len = 0;

        m_pSockTcp->Send(m_ClientSocket, (char *)&SendHead, sizeof(HEAD_INFO));

        delete[] pBuff;

        return;
    }

    // 发送操作成功
    SendHead.m_Cmd = OP_DELETEOK;
    SendHead.m_Len = 0;

    m_pSockTcp->Send(m_ClientSocket, (char *)&SendHead, sizeof(HEAD_INFO));

    delete[] pBuff;

    // 通知所有客户端，缓存失效
    NoticeClientCache();
}

// 匹配到修改
void CMyTask::MatchingUpdate(HEAD_INFO head)
{
    // 申请空间
    char *pBuff = new char[head.m_Len];
    // 初始化申请的空间
    ZeroMemory(pBuff, head.m_Len);

    // 接收数据
    int nRet = m_pSockTcp->Recv(m_ClientSocket, pBuff, head.m_Len);

    if (nRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() == 10035)
        {
            delete[] pBuff;
            return;
        }
        printf("接收数据错误\r\n");
        delete[] pBuff;
        return;
    }

    HEAD_INFO SendHead;

    // 数据库执行操作
    nRet = m_pStudentDatabase->ExecutesInstructions(pBuff);
    if (nRet != 0)
    {
        if (WSAGetLastError() == 10035)
        {
            delete[] pBuff;
            return;
        }

        // 发送操作没有成功
        SendHead.m_Cmd = OP_UPDATEERROR;
        SendHead.m_Len = 0;

        m_pSockTcp->Send(m_ClientSocket, (char *)&SendHead, sizeof(HEAD_INFO));

        delete[] pBuff;

        return;
    }

    // 发送操作成功
    SendHead.m_Cmd = OP_UPDATEOK;
    SendHead.m_Len = 0;

    m_pSockTcp->Send(m_ClientSocket, (char *)&SendHead, sizeof(HEAD_INFO));

    delete[] pBuff;

    // 通知所有客户端，缓存失效
    NoticeClientCache();
}

// 匹配到查询
void CMyTask::MatchingQuery(HEAD_INFO head)
{
    // 申请空间
    char *pBuff = new char[head.m_Len];
    // 初始化申请的空间
    ZeroMemory(pBuff, head.m_Len);

    // 接收数据
    int nRet = m_pSockTcp->Recv(m_ClientSocket, pBuff, head.m_Len);

    if (nRet == SOCKET_ERROR)
    {
        if (WSAGetLastError() == 10035)
        {
            delete[] pBuff;
            return;
        }
        printf("接收数据错误\r\n");
        delete[] pBuff;
        return;
    }

    HEAD_INFO SendHead;

    // 数据库执行操作
    nRet = m_pStudentDatabase->ExecutesInstructions(pBuff);
    if (nRet != 0)
    {
        // 发送操作没有成功
        SendHead.m_Cmd = OP_QUERYERROR;
        SendHead.m_Len = 0;

        m_pSockTcp->Send(m_ClientSocket, (char *)&SendHead, sizeof(HEAD_INFO));

        delete[] pBuff;

        return;
    }

    m_pStudentDatabase->GetQueryInfo(m_pSockTcp, m_ClientSocket);

    delete[] pBuff;
}

// 当数据增加、删除、修改的时候，通知所有客户端缓存失效
void CMyTask::NoticeClientCache()
{
    HEAD_INFO head;
    head.m_Cmd = OP_CACHEFAILED; // cache failed
    head.m_Len = 0;

    m_pCriticalSection->Enter();

    // 遍历链表向所有客户端发送缓存失效消息
    for (auto ListData : *m_pListToSockEvent)
    {
        m_pSockTcp->Send(ListData.first, (char *)&head, sizeof(HEAD_INFO));
    }

    m_pCriticalSection->Leave();
}

// 释放
void CMyTask::Relese()
{
    delete this;
}
