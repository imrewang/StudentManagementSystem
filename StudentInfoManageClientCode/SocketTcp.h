#pragma once
#include "CommonHeadFile.h"

//添加此句，屏蔽"warning作为error", 提高兼容性
//#pragma warning(disable:4996) 
//https://blog.csdn.net/sanqima/article/details/124643090 解决“‘inet_addr‘: Use inet_pton() or InetPton() instead “问题

class CSocketTcp
{
public:
    //构造
    CSocketTcp();

    //析构
    ~CSocketTcp();

    //创建socket
    BOOL CreateSocket();

    //绑定socket
    BOOL BindSocket(char* szIpAddr, USHORT sProt);

    //监听socket
    BOOL ListenSocket();

    //等待连接，成功返回新SOCKET,失败返回INVALID_SOCKET
    SOCKET AcceptSocket(SOCKADDR_IN& si);

    //连接
    BOOL ConnectSocket(char* szIpAddr, USHORT sProt);

    //接收信息,成功返回接收的字节数，连接正常关闭，返回0，否则返回SOCKET_ERROR 
    int Recv(SOCKET sock, char* szBuff, int len);

    //发送信息
    BOOL Send(SOCKET sock, const char* szBuff, int len);

    //关闭socket
    void CloseSocket();

private:
    SOCKET m_Socket;
};


