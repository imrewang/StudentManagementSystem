#pragma once

//添加此句，屏蔽"warning作为error", 提高兼容性
#pragma warning(disable:4996) 

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

    //连接socket
    BOOL ConnectSocket(const DWORD dwIpAddr, USHORT sProt);

    //接收信息,成功返回接收的字节数，连接正常关闭，返回0，否则返回SOCKET_ERROR 
    int Recv(char* szBuff, int len);

    //发送信息
    BOOL Send(const char* szBuff, int len);

    //关闭socket
    void CloseSocket();

private:
    SOCKET m_Socket;
};

