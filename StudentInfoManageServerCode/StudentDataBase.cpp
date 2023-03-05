#include "StudentDataBase.h"
#include "protocol.h"

// 构造
CStudentDataBase::CStudentDataBase()
{
    memset(&m_Mysql, 0, sizeof(MYSQL));
}

// 析构
CStudentDataBase::~CStudentDataBase()
{
    Close();
}

// 初始化，连接数据库
bool CStudentDataBase::InitDatabase(const char *account, const char *password)
{
    // 初始化
    if (mysql_init(&m_Mysql) == NULL) // mysql_init()　　获取或初始化MYSQL结构。
    {
        printf("初始化问题：%s\r\n", mysql_error(&m_Mysql)); // mysql_error()　　返回上次调用的MySQL函数的错误消息。
        return false;
    }

    // 设置字符集
    if (mysql_set_character_set(&m_Mysql, "gbk") != 0)
    {
        printf("设置字符集失败！\r\n");
        mysql_close(&m_Mysql); // mysql_close()　　关闭服务器连接。
        return false;
    }

    // 连接数据库
    if (mysql_real_connect(&m_Mysql, "localhost", account, password, "MyStuManSys", 3306, NULL, 0) == NULL)
    { // mysql_real_connect()　　连接到MySQL服务器。
        // 如果没有此数据库，则创建
        if (mysql_errno(&m_Mysql) == 1049)
        { // mysql_errno()　　返回上次调用的MySQL函数的错误编号。
            mysql_close(&m_Mysql);
            return CreateDatabase(account, password);
        }
        else
        {
            printf("连接数据库问题：%s\r\n", mysql_error(&m_Mysql));
            mysql_close(&m_Mysql);
            return false;
        }
    }

    return true;
}

// 执行数据库指令
unsigned int CStudentDataBase::ExecutesInstructions(const char *szData)
{
    // 数据库执行操作
    if (mysql_query(&m_Mysql, szData) != 0)
    {
        return mysql_errno(&m_Mysql);
    }

    return 0;
}

// 获取查询信息
void CStudentDataBase::GetQueryInfo(CSocketTcp *pSockTcp, SOCKET sock)
{
    // 保存结果
    MYSQL_RES *pStoreRet = mysql_store_result(&m_Mysql);

    // 保存遍历行的结果
    MYSQL_ROW szRowData = mysql_fetch_row(pStoreRet);

    while (szRowData != NULL)
    {
        int i = 0;
        QUERY_INFO QueryInfo;

        strcpy_s(QueryInfo.m_StuId, szRowData[i++]);
        strcpy_s(QueryInfo.m_StuName, szRowData[i++]);
        strcpy_s(QueryInfo.m_StuSex, szRowData[i++]);
        strcpy_s(QueryInfo.m_StuData, szRowData[i++]);

        HEAD_INFO SendHead;
        // 发送操作成功
        SendHead.m_Cmd = OP_QUERYOK;
        SendHead.m_Len = 0;

        // 发送头
        pSockTcp->Send(sock, (char *)&SendHead, sizeof(HEAD_INFO));

        // 发送数据
        pSockTcp->Send(sock, (char *)&QueryInfo, sizeof(QUERY_INFO));

        szRowData = mysql_fetch_row(pStoreRet);
    }
}

// 获取错误信息
const char *CStudentDataBase::GetErrorInfo()
{
    return mysql_error(&m_Mysql);
}

// 创建数据库
bool CStudentDataBase::CreateDatabase(const char *account, const char *password)
{
    // 初始化
    if (mysql_init(&m_Mysql) == NULL)
    {
        printf("%s\r\n", mysql_error(&m_Mysql));
        return false;
    }

    // 设置字符集
    if (mysql_set_character_set(&m_Mysql, "gbk") != 0)
    {
        printf("设置字符集失败！\r\n");
        mysql_close(&m_Mysql);
        return false;
    }

    // 连接到默认数据库
    if (mysql_real_connect(&m_Mysql, "localhost", account, password, "mysql", 3306, NULL, 0) == NULL)
    {
        printf("%s\r\n", mysql_error(&m_Mysql));
        mysql_close(&m_Mysql);
        return false;
    }

    // 创建要创建的数据库
    if (mysql_query(&m_Mysql, "CREATE DATABASE MyStuManSys") != 0)
    { // mysql_query()　　执行指定为“以Null终结的字符串”的SQL查询。
        printf("%s\r\n", mysql_error(&m_Mysql));
        mysql_close(&m_Mysql);
        return false;
    }

    // 选择创建后的数据库
    if (mysql_select_db(&m_Mysql, "MyStuManSys") != 0)
    { // mysql_select_db()　　选择数据库。
        printf("%s\r\n", mysql_error(&m_Mysql));
        mysql_close(&m_Mysql);
        return false;
    }

    // 创建学生表
    char szCreateStudentTable[] = "CREATE TABLE t_student(student_id VARCHAR(45) NOT NULL,\
student_name VARCHAR(45) NOT NULL,\
student_sex VARCHAR(4) NOT NULL,\
student_data DATE NOT NULL,\
PRIMARY KEY(student_id))";

    if (mysql_query(&m_Mysql, szCreateStudentTable) != 0)
    {
        printf("%s\r\n", mysql_error(&m_Mysql));
        mysql_close(&m_Mysql);
        return false;
    }

    return true;
}

// 关闭数据库
void CStudentDataBase::Close()
{
    mysql_close(&m_Mysql);
}
