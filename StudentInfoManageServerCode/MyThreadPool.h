#pragma once
#include "CriticalSections.h"
#include "MySemaphore.h"
#include "MyTask.h"

using namespace std;


class CMyThreadPool
{
public:
    //构造
    CMyThreadPool();

    //析构
    ~CMyThreadPool();

    //创建等待线程
    void CreateThreadPool();

    //向任务队列添加任务
    void InsertTask(CMyTask* pTask);

private:
    static DWORD WINAPI WorkThread(LPVOID lpParameter);

private:
    CCriticalSections m_CS;           //临界区
    CMySemaphore m_Semaphore;         //信号量
    queue<CMyTask*> m_QueueTask;       //任务队列
};


