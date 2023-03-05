// ConnectServer.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StudentInfoManageClient.h"
#include "ConnectServer.h"
#include "afxdialogex.h"


// CConnectServer �Ի���

IMPLEMENT_DYNAMIC(CConnectServer, CDialogEx)

CConnectServer::CConnectServer(CWnd * pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG1, pParent)
    , m_EditPort(0)
{

}

CConnectServer::~CConnectServer()
{
}

void CConnectServer::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS1, m_IpCtrl);
    DDX_Text(pDX, IDC_EDIT1, m_EditPort);
}


BEGIN_MESSAGE_MAP(CConnectServer, CDialogEx)
    ON_BN_CLICKED(BTN_CONNECT, &CConnectServer::OnBnClickedConnect)
    ON_BN_CLICKED(BTN_CANCLE, &CConnectServer::OnBnClickedCancle)
    ON_WM_CLOSE()
END_MESSAGE_MAP()


// CConnectServer ��Ϣ�������


//���Ӱ�ť

void CConnectServer::SetSocket(CSocketTcp* pSockTcp)
{
    m_pSockTcp = pSockTcp;
}

void CConnectServer::OnBnClickedConnect()
{
    UpdateData(TRUE);

    //��ȡIP��ַ
    DWORD dwAddress = 0;
    m_IpCtrl.GetAddress(dwAddress);

    //����socket
    if (!m_pSockTcp->ConnectSocket(dwAddress, m_EditPort))
    {
        MessageBox("����ʧ�ܣ�����IP���߶˿��Ƿ�������ȷ��", "��ʾ", MB_OK);
        return;
    }
    EndDialog(IDOK);
}


//ȡ����ť
void CConnectServer::OnBnClickedCancle()
{
    EndDialog(IDCANCEL);
}


//�رհ�ť
void CConnectServer::OnClose()
{
    EndDialog(IDCANCEL);
    CDialogEx::OnClose();
}
