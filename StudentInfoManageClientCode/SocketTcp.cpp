#include "stdafx.h"
#include "SocketTcp.h"


//����
CSocketTcp::CSocketTcp()
{
    m_Socket = INVALID_SOCKET;
}


//����
CSocketTcp::~CSocketTcp()
{
    CloseSocket();
}


//����socket
BOOL CSocketTcp::CreateSocket()
{
    m_Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_Socket == INVALID_SOCKET)
    {
        return FALSE;
    }

    return TRUE;
}

//��socket
BOOL CSocketTcp::BindSocket(char* szIpAddr, USHORT sProt)
{
    SOCKADDR_IN si = { 0 };

    si.sin_family = AF_INET;
    si.sin_port = ::htons(sProt);
    si.sin_addr.S_un.S_addr = inet_addr(szIpAddr);

    int nRet = ::bind(m_Socket, (sockaddr*)&si, sizeof(si));

    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }

    return TRUE;
}


//����socket
BOOL CSocketTcp::ListenSocket()
{
    int nRet = ::listen(m_Socket, SOMAXCONN);

    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }

    return TRUE;
}


//�ȴ�����
SOCKET CSocketTcp::AcceptSocket(SOCKADDR_IN& si)
{
    int nSize = sizeof(si);

    return ::accept(m_Socket, (sockaddr*)&si, &nSize);
}


//����
BOOL CSocketTcp::ConnectSocket(char* szIpAddr, USHORT sProt)
{
    SOCKADDR_IN si = { 0 };

    si.sin_family = AF_INET;
    si.sin_port = htons(sProt);
    si.sin_addr.S_un.S_addr = inet_addr(szIpAddr);

    int nRet = ::connect(m_Socket, (sockaddr*)&si, sizeof(si));

    if (nRet != 0)
    {
        return FALSE;
    }

    return TRUE;
}

//����socket
BOOL CSocketTcp::ConnectSocket(const DWORD dwIpAddr, USHORT sProt)
{
    SOCKADDR_IN si = { 0 };

    si.sin_family = AF_INET;
    si.sin_port = htons(sProt);
    si.sin_addr.S_un.S_addr = ntohl(dwIpAddr);

    int nRet = connect(m_Socket, (sockaddr*)&si, sizeof(si));

    if (nRet != 0)
    {
        return FALSE;
    }

    return TRUE;
}

//������Ϣ
int CSocketTcp::Recv(char* szBuff, int len)
{
    return ::recv(m_Socket, szBuff, len, 0);
}


//������Ϣ
BOOL CSocketTcp::Send(const char* szBuff, int len)
{
    int nRet = send(m_Socket, szBuff, len, 0);

    if (nRet == SOCKET_ERROR)
    {
        return FALSE;
    }

    return TRUE;
}


//�ر�socket
void CSocketTcp::CloseSocket()
{
    if (m_Socket != INVALID_SOCKET)
    {
        closesocket(m_Socket);
        m_Socket = INVALID_SOCKET;
    }
}
