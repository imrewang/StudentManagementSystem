#include "SocketTcp.h"

// 构造
CSocketTcp::CSocketTcp()
{
    m_Socket = INVALID_SOCKET;
}

// 析构
CSocketTcp::~CSocketTcp()
{
    CloseSocket();
}

// 创建socket
BOOL CSocketTcp::CreateSocket()
{
    m_Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_Socket == INVALID_SOCKET)
    {
        return FALSE;
    }

    return TRUE;
}

// 绑定socket
BOOL CSocketTcp::BindSocket(char *szIpAddr, USHORT sProt)
{
    SOCKADDR_IN si = {0};

    si.sin_family = AF_INET; // 指定IP地址地址版本人为IPV4
    si.sin_port = ::htons(sProt);
    si.sin_addr.S_un.S_addr = inet_addr(szIpAddr);

    int nRet = ::bind(m_Socket, (sockaddr *)&si, sizeof(si));

    if (nRet == SOCKET_ERROR)
    {
        printf("WSAGetLastError:%d\n", WSAGetLastError());
        return FALSE;
    }

    return TRUE;
}

// 监听socket
BOOL CSocketTcp::ListenSocket()
{
    int nRet = ::listen(m_Socket, SOMAXCONN);

    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }

    return TRUE;
}

// 等待连接
SOCKET CSocketTcp::AcceptSocket(SOCKADDR_IN &si)
{
    int nSize = sizeof(si);

    return ::accept(m_Socket, (sockaddr *)&si, &nSize);
}

// 连接
BOOL CSocketTcp::ConnectSocket(char *szIpAddr, USHORT sProt)
{
    SOCKADDR_IN si = {0};

    si.sin_family = AF_INET;
    si.sin_port = htons(sProt);
    si.sin_addr.S_un.S_addr = inet_addr(szIpAddr);

    int nRet = ::connect(m_Socket, (sockaddr *)&si, sizeof(si));

    if (nRet != 0)
    {
        return FALSE;
    }

    return TRUE;
}

// 接收信息
int CSocketTcp::Recv(SOCKET sock, char *szBuff, int len)
{
    return ::recv(sock, szBuff, len, 0);
}

// 发送信息
BOOL CSocketTcp::Send(SOCKET sock, const char *szBuff, int len)
{
    int nRet = send(sock, szBuff, len, 0);

    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }

    return TRUE;
}

// 关闭socket
void CSocketTcp::CloseSocket()
{
    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
}
