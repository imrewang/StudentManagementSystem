#pragma once

//��Ӵ˾䣬����"warning��Ϊerror", ��߼�����
#pragma warning(disable:4996) 

class CSocketTcp
{
public:
    //����
    CSocketTcp();

    //����
    ~CSocketTcp();

    //����socket
    BOOL CreateSocket();

    //��socket
    BOOL BindSocket(char* szIpAddr, USHORT sProt);

    //����socket
    BOOL ListenSocket();

    //�ȴ����ӣ��ɹ�������SOCKET,ʧ�ܷ���INVALID_SOCKET
    SOCKET AcceptSocket(SOCKADDR_IN& si);

    //����
    BOOL ConnectSocket(char* szIpAddr, USHORT sProt);

    //����socket
    BOOL ConnectSocket(const DWORD dwIpAddr, USHORT sProt);

    //������Ϣ,�ɹ����ؽ��յ��ֽ��������������رգ�����0�����򷵻�SOCKET_ERROR 
    int Recv(char* szBuff, int len);

    //������Ϣ
    BOOL Send(const char* szBuff, int len);

    //�ر�socket
    void CloseSocket();

private:
    SOCKET m_Socket;
};

