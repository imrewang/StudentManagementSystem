#include "MySemaphore.h"

// 构造
CMySemaphore::CMySemaphore()
{
    m_hSemaphore = NULL;
}

// 析构
CMySemaphore::~CMySemaphore()
{
    // 关闭句柄
    Close();
}

// 创建信号量
bool CMySemaphore::Create(LONG lInitialCount, LONG lMaximumCount)
{
    m_hSemaphore = ::CreateSemaphore(NULL, lInitialCount, lMaximumCount, NULL);

    if (m_hSemaphore == NULL) // 如果函数失败，则返回值为NULL。
    {
        return false;
    }

    return true;
}

// 释放信号量
bool CMySemaphore::Release(LONG lReleaseCount)
{
    BOOL bRet = ::ReleaseSemaphore(m_hSemaphore, lReleaseCount, NULL);
    if (bRet == 0)
    {
        return false;
    }
    return true;
}

// 等待信号
DWORD CMySemaphore::WaitForSemaphore()
{
    return ::WaitForSingleObject(m_hSemaphore, INFINITE);
}

// 关闭句柄
void CMySemaphore::Close()
{
    if (m_hSemaphore != NULL)
    {
        ::CloseHandle(m_hSemaphore);
        m_hSemaphore = NULL;
    }
}