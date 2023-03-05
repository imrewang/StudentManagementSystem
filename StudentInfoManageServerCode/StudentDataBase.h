#pragma once
#include "CommonHeadFile.h"
#include "SocketTcp.h"

class CStudentDataBase
{
public:
    // 构造
    CStudentDataBase();

    // 析构
    ~CStudentDataBase();

    // 初始化，连接数据库
    bool InitDatabase(const char *account, const char *password);

    // 执行数据库指令
    unsigned int ExecutesInstructions(const char *szData);

    // 获取查询信息
    void GetQueryInfo(CSocketTcp *pSockTcp, SOCKET sock);

    // 获取错误信息
    const char *GetErrorInfo();

private:
    // 创建数据库
    bool CreateDatabase(const char *account, const char *password);

    // 关闭数据库
    void Close();

private:
    MYSQL m_Mysql; // MYSQL
    // 该结构代表1个数据库连接的句柄。几乎所有的MySQL函数均使用它。不应尝试拷贝MYSQL结构。不保证这类拷贝结果会有用。
};
