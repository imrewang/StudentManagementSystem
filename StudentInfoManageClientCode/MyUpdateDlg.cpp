// MyUpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StudentInfoManageClient.h"
#include "MyUpdateDlg.h"
#include "afxdialogex.h"


// CMyUpdateDlg 对话框

IMPLEMENT_DYNAMIC(CMyUpdateDlg, CDialogEx)

CMyUpdateDlg::CMyUpdateDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_DIALOG2, pParent)
    , m_EditStuID(_T(""))
    , m_EditStuName(_T(""))
    , m_nSex(0)
{

}

CMyUpdateDlg::~CMyUpdateDlg()
{
}

void CMyUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_EditStuID);
    DDV_MaxChars(pDX, m_EditStuID, 45);
    DDX_Text(pDX, IDC_EDIT2, m_EditStuName);
    DDV_MaxChars(pDX, m_EditStuName, 45);
    DDX_Radio(pDX, IDC_RADIO1, m_nSex);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DataTimeCtrl);
}


void CMyUpdateDlg::SetStuId(CString id)
{
    m_EditStuID = id;
}

void CMyUpdateDlg::SetStuName(CString name)
{
    m_EditStuName = name;
}

void CMyUpdateDlg::SetStuSex(int nSex)
{
    m_nSex = nSex;
}

void CMyUpdateDlg::SetStuDate(CString data1)
{
    m_strData = data1;
}

CString CMyUpdateDlg::GetStuName()
{
    return m_EditStuName;
}

CString CMyUpdateDlg::GetStuSex()
{
    return m_nSex == 0 ? "男" : "女";
}

CString CMyUpdateDlg::GetStuDate()
{
    return m_strData;
}

BEGIN_MESSAGE_MAP(CMyUpdateDlg, CDialogEx)
    ON_WM_CLOSE()
    ON_BN_CLICKED(BTN_UPDATA, &CMyUpdateDlg::OnBnClickedUpdata)
END_MESSAGE_MAP()


// CMyUpdateDlg 消息处理程序


BOOL CMyUpdateDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //格式化日期
    int t[3] = { 0 };
    sscanf_s(m_strData.GetBuffer(), "%d-%d-%d", &t[0], &t[1], &t[2]);

    CTime time(t[0], t[1], t[2], 1, 1, 1);

    //将值显示到控件上
    m_DataTimeCtrl.SetTime(&time);


    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}


void CMyUpdateDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值

    CDialogEx::OnClose();
}


//修改按钮
void CMyUpdateDlg::OnBnClickedUpdata()
{
    UpdateData(TRUE);

    if (m_EditStuName.IsEmpty())
    {
        MessageBox("请输入学生姓名", "提示", MB_OK);
        return;
    }

    //获取生日信息
    CTime time;
    m_DataTimeCtrl.GetTime(time);
    m_strData = time.Format("%Y-%m-%d");

    EndDialog(IDYES);
}
