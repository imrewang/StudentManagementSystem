// MyUpdateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StudentInfoManageClient.h"
#include "MyUpdateDlg.h"
#include "afxdialogex.h"


// CMyUpdateDlg �Ի���

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
    return m_nSex == 0 ? "��" : "Ů";
}

CString CMyUpdateDlg::GetStuDate()
{
    return m_strData;
}

BEGIN_MESSAGE_MAP(CMyUpdateDlg, CDialogEx)
    ON_WM_CLOSE()
    ON_BN_CLICKED(BTN_UPDATA, &CMyUpdateDlg::OnBnClickedUpdata)
END_MESSAGE_MAP()


// CMyUpdateDlg ��Ϣ�������


BOOL CMyUpdateDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    //��ʽ������
    int t[3] = { 0 };
    sscanf_s(m_strData.GetBuffer(), "%d-%d-%d", &t[0], &t[1], &t[2]);

    CTime time(t[0], t[1], t[2], 1, 1, 1);

    //��ֵ��ʾ���ؼ���
    m_DataTimeCtrl.SetTime(&time);


    UpdateData(FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // �쳣: OCX ����ҳӦ���� FALSE
}


void CMyUpdateDlg::OnClose()
{
    // TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

    CDialogEx::OnClose();
}


//�޸İ�ť
void CMyUpdateDlg::OnBnClickedUpdata()
{
    UpdateData(TRUE);

    if (m_EditStuName.IsEmpty())
    {
        MessageBox("������ѧ������", "��ʾ", MB_OK);
        return;
    }

    //��ȡ������Ϣ
    CTime time;
    m_DataTimeCtrl.GetTime(time);
    m_strData = time.Format("%Y-%m-%d");

    EndDialog(IDYES);
}
