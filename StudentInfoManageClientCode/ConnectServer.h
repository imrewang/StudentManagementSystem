#pragma once
#include "afxcmn.h"
#include "SocketTcp.h"


// CConnectServer �Ի���

class CConnectServer : public CDialogEx
{
    DECLARE_DYNAMIC(CConnectServer)

public:
    CConnectServer(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CConnectServer();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG1 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()

private:
    CSocketTcp* m_pSockTcp;

public:
    void SetSocket(CSocketTcp* pSockTcp);

public:
    afx_msg void OnBnClickedConnect();
    afx_msg void OnBnClickedCancle();
    afx_msg void OnClose();
    CIPAddressCtrl m_IpCtrl;
    short m_EditPort;
};
