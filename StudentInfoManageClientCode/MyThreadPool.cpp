#include "MyThreadPool.h"

CMyThreadPool::CMyThreadPool()
{
}

CMyThreadPool::~CMyThreadPool()
{
}

// 创建等待线程
void CMyThreadPool::CreateThreadPool()
{
    SYSTEM_INFO SysInfo;

    // 获取cpu核心数
    ::GetSystemInfo(&SysInfo);

    // 创建对应的信号量
    m_Semaphore.Create(0, SysInfo.dwNumberOfProcessors); // 初始化为0，上限是dwNumberOfProcessors
    // 也可以和任务队列m_QueueTask最大长度一样

    // printf("%d", SysInfo.dwNumberOfProcessors); //输出8

    // 创建对应的线程
    for (int i = 0; i < SysInfo.dwNumberOfProcessors; i++)
    {
        HANDLE hThread = CreateThread(NULL, 0, WorkThread, this, 0, NULL);
        CloseHandle(hThread);
    }
}

// 向任务队列添加任务
void CMyThreadPool::InsertTask(CMyTask *pTask)
{
    // 添加到任务队列
    m_CS.Enter();
    m_QueueTask.push(pTask);
    m_CS.Leave();

    // 释放一个信号
    int ret = m_Semaphore.Release(1);
    printf("释放一个信号:%d\n", ret);
    // 这个十分之好，可以看客户端到底真正发起了哪些请求
}

// 工作线程
DWORD WINAPI CMyThreadPool::WorkThread(LPVOID lpParameter)
{
    CMyThreadPool *pThis = (CMyThreadPool *)lpParameter;

    while (true)
    {
        // 等待任务来临//先获取任务，没有任务挂起
        pThis->m_Semaphore.WaitForSemaphore();

        pThis->m_CS.Enter();
        // 从任务队列取出任务
        CMyTask *pTask = pThis->m_QueueTask.front();
        // 弹出任务队列中的任务
        pThis->m_QueueTask.pop();
        printf("从任务队列取出任务");
        pThis->m_CS.Leave();

        // 执行任务
        pTask->Execute();
    }
}
