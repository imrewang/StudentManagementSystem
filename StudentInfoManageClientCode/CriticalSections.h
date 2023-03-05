#pragma once
#include "CommonHeadFile.h"

class CCriticalSections
{
public:
    // 构造
    CCriticalSections();

    // 析构
    ~CCriticalSections();

    // 进入临界区
    void Enter();

    // 离开临界区
    void Leave();

private:
    // 初始化临界区
    void Initialize();

    // 释放临界区
    void Delete();

private:
    CRITICAL_SECTION m_CS;
};
