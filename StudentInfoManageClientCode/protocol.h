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

    OP_CACHEFAILED            //����ʧЧ
};


//Э��ͷ��Ϣ
typedef struct _HEAD_INFO
{
    _HEAD_INFO()
    {
        m_Cmd = -1;
        m_Len = 0;
    }

    int m_Cmd;      //����
    int m_Len;      //����
}HEAD_INFO, * P_HEAD_INFO;

//��ѯ��Ϣ�ṹ��
typedef struct _QUERY_INFO
{
    char m_StuId[45] = { 0 };
    char m_StuName[45] = { 0 };
    char m_StuSex[4] = { 0 };
    char m_StuData[12] = { 0 };
}QUERY_INFO, * P_QUERY_INFO;