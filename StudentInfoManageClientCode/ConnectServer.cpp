// ConnectServer.cpp : 实现文件
//

#include "stdafx.h"
#include "StudentInfoManageClient.h"
#include "ConnectServer.h"
#include "afxdialogex.h"


// CConnectServer 对话框

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


// CConnectServer 消息处理程序


//连接按钮

void CConnectServer::SetSocket(CSocketTcp* pSockTcp)
{
    m_pSockTcp = pSockTcp;
}

void CConnectServer::OnBnClickedConnect()
{
    UpdateData(TRUE);

    //获取IP地址
    DWORD dwAddress = 0;
    m_IpCtrl.GetAddress(dwAddress);

    //连接socket
    if (!m_pSockTcp->ConnectSocket(dwAddress, m_EditPort))
    {
        MessageBox("连接失败，请检查IP或者端口是否输入正确！", "提示", MB_OK);
        return;
    }
    EndDialog(IDOK);
}


//取消按钮
void CConnectServer::OnBnClickedCancle()
{
    EndDialog(IDCANCEL);
}


//关闭按钮
void CConnectServer::OnClose()
{
    EndDialog(IDCANCEL);
    CDialogEx::OnClose();
}
