#pragma once
#include "StudentDataBase.h"
#include "SocketTcp.h"
#include "CriticalSections.h"
#include "protocol.h"

class CMyTask
{
public:
    //构造
    CMyTask();

    //有参构造
    CMyTask(SOCKET ClientSocket,
        CSocketTcp* pSockTcp,
        CStudentDataBase* pStudentDatabase,
        list<pair<SOCKET, WSAEVENT>>* pListToSockEvent,
        CCriticalSections* pCriticalSection
    );

    //析构
    ~CMyTask();

    //执行任务
    void Execute();

private:
    //匹配数据
    void Matching(HEAD_INFO head);

    //匹配到增加
    void MatchingInsert(HEAD_INFO head);

    //匹配到删除
    void MatchingDelete(HEAD_INFO head);

    //匹配到修改
    void MatchingUpdate(HEAD_INFO head);

    //匹配到查询
    void MatchingQuery(HEAD_INFO head);

    //当数据增加、删除、修改的时候，通知所有客户端缓存失效
    void NoticeClientCache();

    //释放
    void Relese();
private:
    SOCKET m_ClientSocket;
    CSocketTcp* m_pSockTcp;
    CStudentDataBase* m_pStudentDatabase;
    list<pair<SOCKET, WSAEVENT>>* m_pListToSockEvent;
    CCriticalSections* m_pCriticalSection;
};



