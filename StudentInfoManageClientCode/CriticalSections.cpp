#include "CriticalSections.h"

// 构造
CCriticalSections::CCriticalSections()
{
    Initialize();
}

// 析构
CCriticalSections::~CCriticalSections()
{
    Delete();
}

// 进入临界区
void CCriticalSections::Enter()
{
    ::EnterCriticalSection(&m_CS);
}

// 离开临界区
void CCriticalSections::Leave()
{
    ::LeaveCriticalSection(&m_CS);
}

// 初始化临界区
void CCriticalSections::Initialize()
{
    ::InitializeCriticalSection(&m_CS);
}

// 释放临界区
void CCriticalSections::Delete()
{
    ::DeleteCriticalSection(&m_CS);
}
