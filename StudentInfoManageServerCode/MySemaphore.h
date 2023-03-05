#pragma once
#include "CommonHeadFile.h"

class CMySemaphore
{
public:
    // 构造
    CMySemaphore();

    // 析构
    ~CMySemaphore();

    // 创建信号量
    bool Create(LONG lInitialCount = 1, LONG lMaximumCount = 1);

    // 释放信号量
    bool Release(LONG lReleaseCount = 1);

    // 等待信号
    DWORD WaitForSemaphore();

    // 关闭句柄
    void Close();

private:
    HANDLE m_hSemaphore;
};
