#pragma once

enum OperatorProtocol
{
    OP_INSERT,
    OP_DELETE,
    OP_UPDATE,
    OP_QUERY,

    OP_INSERTOK,
    OP_INSERTERROR,
    OP_DELETEOK,
    OP_DELETEERROR,
    OP_UPDATEOK,
    OP_UPDATEERROR,
    OP_QUERYOK,
    OP_QUERYERROR,

    OP_CACHEFAILED            //缓存失效
};


//协议头信息
typedef struct _HEAD_INFO
{
    _HEAD_INFO()
    {
        m_Cmd = -1;
        m_Len = 0;
    }

    int m_Cmd;      //命令
    int m_Len;      //长度
}HEAD_INFO, * P_HEAD_INFO;

//查询信息结构体
typedef struct _QUERY_INFO
{
    char m_StuId[45] = { 0 };
    char m_StuName[45] = { 0 };
    char m_StuSex[4] = { 0 };
    char m_StuData[12] = { 0 };
}QUERY_INFO, * P_QUERY_INFO;