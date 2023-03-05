
// StudentInfoManageClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StudentInfoManageClient.h"
#include "StudentInfoManageClientDlg.h"
#include "afxdialogex.h"
#include "ConnectServer.h"
#include "MyUpdateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RECV_DATA_FAILED WM_USER + 1

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStudentInfoManageClientDlg �Ի���



CStudentInfoManageClientDlg::CStudentInfoManageClientDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(IDD_STUDENTINFOMANAGECLIENT_DIALOG, pParent)
    , m_EditStuID(_T(""))
    , m_EditStuName(_T(""))
    , m_nSex(0)
    , m_bQuerySex(FALSE)
    , m_bQueryData(FALSE)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_nIdx = -1;
}

void CStudentInfoManageClientDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, EDT_STUID, m_EditStuID);
    DDV_MaxChars(pDX, m_EditStuID, 45);
    DDX_Text(pDX, EDT_STUNAME, m_EditStuName);
    DDV_MaxChars(pDX, m_EditStuName, 45);
    DDX_Radio(pDX, IDC_RADIO1, m_nSex);
    DDX_Check(pDX, IDC_CHECK1, m_bQuerySex);
    DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DataTimeCtrl);
    DDX_Check(pDX, IDC_CHECK2, m_bQueryData);
    DDX_Control(pDX, LST_LIST, m_ListCtrl);
}

BEGIN_MESSAGE_MAP(CStudentInfoManageClientDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_CLOSE()
    ON_BN_CLICKED(BTN_ADD, &CStudentInfoManageClientDlg::OnBnClickedAdd)
    ON_BN_CLICKED(BTN_FIND, &CStudentInfoManageClientDlg::OnBnClickedFind)
    ON_COMMAND(MN_DELETE, &CStudentInfoManageClientDlg::OnDelete)
    ON_COMMAND(MN_UPDATA, &CStudentInfoManageClientDlg::OnUpdata)
    ON_NOTIFY(NM_RCLICK, LST_LIST, &CStudentInfoManageClientDlg::OnRclickLstList)
END_MESSAGE_MAP()


// CStudentInfoManageClientDlg ��Ϣ�������

BOOL CStudentInfoManageClientDlg::OnInitDialog()
{
    //���������
    if (!m_SockInit.Startup())
    {
        MessageBox("�������ʧ��", "��ʾ", MB_OK);
        PostMessage(WM_CLOSE, NULL, NULL);
        return FALSE;
    }

    //����socket
    if (!m_SockTcp.CreateSocket())
    {
        MessageBox("����socketʧ�ܣ�", "��ʾ", MB_OK);
        PostMessage(WM_CLOSE, NULL, NULL);
        return FALSE;
    }

    //�������ӶԻ���
    CConnectServer ConnectServerDlg;
    ConnectServerDlg.SetSocket(&m_SockTcp);
    if (ConnectServerDlg.DoModal() == IDCANCEL)
    {
        PostMessage(WM_CLOSE, NULL, NULL);
        return FALSE;
    }


    CDialogEx::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    //�������������߳�
    HANDLE hThread = CreateThread(NULL, 0, DataThread, this, 0, NULL);
    CloseHandle(hThread);

    //����ListCtrl���
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //��ӱ�ͷ
    m_ListCtrl.InsertColumn(0, "ѧ��", LVCFMT_LEFT, 180);
    m_ListCtrl.InsertColumn(1, "����", LVCFMT_LEFT, 180);
    m_ListCtrl.InsertColumn(2, "�Ա�", LVCFMT_LEFT, 180);
    m_ListCtrl.InsertColumn(3, "��������", LVCFMT_LEFT, 300);

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CStudentInfoManageClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CStudentInfoManageClientDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CStudentInfoManageClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


//���������߳�
DWORD WINAPI CStudentInfoManageClientDlg::DataThread(LPVOID lpParameter)
{
    CStudentInfoManageClientDlg* pThis = (CStudentInfoManageClientDlg*)lpParameter;

    while (true)
    {
        HEAD_INFO head;

        //����ͷ����
        int nRet = pThis->m_SockTcp.Recv((char*)&head, sizeof(head));

        if (nRet <= 0)
        {
            pThis->PostMessage(RECV_DATA_FAILED, NULL, NULL);
            break;
        }

        //ƥ������
        pThis->Matching(head);
    }

    return 0;
}

//ƥ������
void CStudentInfoManageClientDlg::Matching(HEAD_INFO head)
{
    switch (head.m_Cmd)
    {
    case OP_INSERTOK://���ӳɹ�
    {
        MessageBox("���ӳɹ�", "��ʾ", MB_OK);
        break;
    }
    case OP_INSERTERROR://����ʧ��
    {
        MessageBox("����ʧ��", "��ʾ", MB_OK);
        break;
    }
    case OP_DELETEOK://ɾ���ɹ�
    {
        m_ListCtrl.DeleteItem(m_nIdx);
        MessageBox("ɾ���ɹ�", "��ʾ", MB_OK);
        break;
    }
    case OP_DELETEERROR://ɾ��ʧ��
    {
        MessageBox("ɾ��ʧ��", "��ʾ", MB_OK);
        break;
    }
    case OP_UPDATEOK://�޸ĳɹ�
    {
        MatchingUpdateOk();
        MessageBox("�޸ĳɹ�", "��ʾ", MB_OK);
        break;
    }
    case OP_UPDATEERROR://�޸�ʧ��
    {
        MessageBox("�޸�ʧ��", "��ʾ", MB_OK);
        break;
    }
    case OP_QUERYOK://��ѯ�ɹ�
    {
        MatchingQueryOk();
        break;
    }
    case OP_QUERYERROR://��ѯʧ��
    {
        MessageBox("��ѯʧ��", "��ʾ", MB_OK);
        break;
    }
    case OP_CACHEFAILED://����ʧЧ
    {
        //��ջ���
        m_MyMap.clear();
        break;
    }
    }
}


//ƥ�䵽�޸ĳɹ�
void CStudentInfoManageClientDlg::MatchingUpdateOk()
{
    m_ListCtrl.SetItemText(m_nIdx, 1, m_strStuName.GetBuffer());
    m_ListCtrl.SetItemText(m_nIdx, 2, m_strStuSex.GetBuffer());
    m_ListCtrl.SetItemText(m_nIdx, 3, m_strStuData.GetBuffer());
}

//ƥ�䵽��ѯ�ɹ�
void CStudentInfoManageClientDlg::MatchingQueryOk()
{
    QUERY_INFO QueryInfo;

    //��������
    int nRet = m_SockTcp.Recv((char*)&QueryInfo, sizeof(QUERY_INFO));

    if (nRet == SOCKET_ERROR)
    {
        MessageBox("��������ʧ��", "��ʾ", MB_OK);
        return;
    }

    //����ѯ�����ݼ��뻺��
    m_MyMap.insert(pair<CString, QUERY_INFO>(m_QueryCommand, QueryInfo));

    int nRow = m_ListCtrl.GetItemCount();

    //����ѯ��������ʾ���б�ؼ�
    m_ListCtrl.InsertItem(nRow, QueryInfo.m_StuId);
    m_ListCtrl.SetItemText(nRow, 1, QueryInfo.m_StuName);
    m_ListCtrl.SetItemText(nRow, 2, QueryInfo.m_StuSex);
    m_ListCtrl.SetItemText(nRow, 3, QueryInfo.m_StuData);
}

void CStudentInfoManageClientDlg::OnClose()
{
    m_SockTcp.CloseSocket();
    m_SockInit.Cleanup();
    CDialogEx::OnClose();
}


//���Ӱ�ť
void CStudentInfoManageClientDlg::OnBnClickedAdd()
{
    UpdateData(TRUE);

    if (m_EditStuID.IsEmpty())
    {
        MessageBox("������ѧ��ID", "��ʾ", MB_OK);
        return;
    }

    if (m_EditStuName.IsEmpty())
    {
        MessageBox("������ѧ������", "��ʾ", MB_OK);
        return;
    }

    //��ȡ������Ϣ
    CTime time;
    m_DataTimeCtrl.GetTime(time);
    CString strBirthDay = time.Format("%Y-%m-%d");

    CString strAdd;
    strAdd.Format("INSERT INTO  t_student VALUES('%s', '%s', '%s', '%s')",
        m_EditStuID.GetBuffer(),
        m_EditStuName.GetBuffer(),
        m_nSex == 0 ? "��" : "Ů",
        strBirthDay.GetBuffer());

    //�������ݵ��ͻ���
    HEAD_INFO head;
    head.m_Cmd = OP_INSERT;
    head.m_Len = strAdd.GetLength() + 1;

    int nSize = sizeof(int) * 2 + head.m_Len;
    char* pBuff = new char[nSize];
    ZeroMemory(pBuff, nSize);

    memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
    memcpy(pBuff + sizeof(HEAD_INFO), strAdd.GetBuffer(), head.m_Len);

    //��������
    int nRet = m_SockTcp.Send(pBuff, nSize);
    if (nRet <= 0)
    {
        MessageBox("��������ʧ��", "��ʾ", MB_OK);
        delete[] pBuff;
        return;
    }

    delete[] pBuff;
}


//��ѯ��ť
void CStudentInfoManageClientDlg::OnBnClickedFind()
{
    UpdateData(TRUE);

    CString strQuery("SELECT * FROM t_student WHERE 1=1");

    if (!m_EditStuID.IsEmpty())
    {
        strQuery += " AND student_id = '" + m_EditStuID + "'";
    }
    if (!m_EditStuName.IsEmpty())
    {
        strQuery += " AND student_name = '" + m_EditStuName + "'";
    }
    if (m_bQuerySex)
    {
        strQuery += " AND student_sex = '";
        strQuery += m_nSex == 0 ? "��'" : "Ů'";
    }
    if (m_bQueryData)
    {
        //��ȡ������Ϣ
        CTime time;
        m_DataTimeCtrl.GetTime(time);
        CString strBirthDay = time.Format("%Y-%m-%d");

        strQuery += " AND student_data = '" + strBirthDay + "'";
    }

    m_QueryCommand = strQuery;

    //�Ȳ�ѯ�����е����ݣ���������������������������

    //�ҵ�Ԫ�صĸ���
    sz_type nNum = m_MyMap.count(m_QueryCommand);

    //���û���ҵ������������������
    if (nNum == 0)
    {
        //�������ݵ��ͻ���
        HEAD_INFO head;
        head.m_Cmd = OP_QUERY;
        head.m_Len = strQuery.GetLength() + 1;

        int nSize = sizeof(int) * 2 + head.m_Len;
        char* pBuff = new char[nSize];
        ZeroMemory(pBuff, nSize);

        memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
        memcpy(pBuff + sizeof(HEAD_INFO), strQuery.GetBuffer(), head.m_Len);

        //��������
        int nRet = m_SockTcp.Send(pBuff, nSize);
        if (nRet <= 0)
        {
            MessageBox("��������ʧ��", "��ʾ", MB_OK);
            delete[] pBuff;
            return;
        }

        delete[] pBuff;

        //ɾ��������
        m_ListCtrl.DeleteAllItems();

        return;
    }
    //����ҵ��ˣ���ӻ�����������
    else
    {
        //ɾ��������
        m_ListCtrl.DeleteAllItems();

        //�ӻ�����������
        multimap<CString, QUERY_INFO>::iterator iter = m_MyMap.find(m_QueryCommand);
        for (sz_type count = 0; count != nNum; ++count, ++iter)
        {
            m_ListCtrl.InsertItem(count, iter->second.m_StuId);
            m_ListCtrl.SetItemText(count, 1, iter->second.m_StuName);
            m_ListCtrl.SetItemText(count, 2, iter->second.m_StuSex);
            m_ListCtrl.SetItemText(count, 3, iter->second.m_StuData);
        }
    }
}


//ɾ��
void CStudentInfoManageClientDlg::OnDelete()
{
    //��ȡѧ��ID
    CString strStuId = m_ListCtrl.GetItemText(m_nIdx, 0);

    CString strDelete;
    strDelete.Format("DELETE FROM t_student WHERE student_id = '%s'", strStuId.GetBuffer());

    //�������ݵ��ͻ���
    HEAD_INFO head;
    head.m_Cmd = OP_DELETE;
    head.m_Len = strDelete.GetLength() + 1;

    int nSize = sizeof(int) * 2 + head.m_Len;
    char* pBuff = new char[nSize];
    ZeroMemory(pBuff, nSize);

    memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
    memcpy(pBuff + sizeof(HEAD_INFO), strDelete.GetBuffer(), head.m_Len);

    //��������
    int nRet = m_SockTcp.Send(pBuff, nSize);
    if (nRet <= 0)
    {
        MessageBox("��������ʧ��", "��ʾ", MB_OK);
        delete[] pBuff;
        return;
    }

    delete[] pBuff;
}


//�޸�
void CStudentInfoManageClientDlg::OnUpdata()
{
    //��ȡѧ��ID
    CString strStuId = m_ListCtrl.GetItemText(m_nIdx, 0);

    //��ȡѧ������
    CString strStuName = m_ListCtrl.GetItemText(m_nIdx, 1);

    //��ȡѧ���Ա�
    CString strStuSex = m_ListCtrl.GetItemText(m_nIdx, 2);

    //��ȡ����
    CString strStuDate = m_ListCtrl.GetItemText(m_nIdx, 3);

    //�����޸ĶԻ���
    CMyUpdateDlg UpdateDlg;
    UpdateDlg.SetStuId(strStuId);
    UpdateDlg.SetStuName(strStuName);
    UpdateDlg.SetStuSex(strStuSex == "��" ? 0 : 1);
    UpdateDlg.SetStuDate(strStuDate);

    if (UpdateDlg.DoModal() != IDYES)
    {
        return;
    }

    m_strStuName = UpdateDlg.GetStuName();
    m_strStuSex = UpdateDlg.GetStuSex();
    m_strStuData = UpdateDlg.GetStuDate();

    CString strUpdate;
    strUpdate.Format("UPDATE t_student SET  student_name = '%s', student_sex = '%s', student_data = '%s' WHERE student_id = '%s'",
        m_strStuName.GetBuffer(),
        m_strStuSex.GetBuffer(),
        m_strStuData.GetBuffer(),
        strStuId.GetBuffer());

    //�������ݵ��ͻ���
    HEAD_INFO head;
    head.m_Cmd = OP_UPDATE;
    head.m_Len = strUpdate.GetLength() + 1;

    int nSize = sizeof(int) * 2 + head.m_Len;
    char* pBuff = new char[nSize];
    ZeroMemory(pBuff, nSize);

    memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
    memcpy(pBuff + sizeof(HEAD_INFO), strUpdate.GetBuffer(), head.m_Len);

    //��������
    int nRet = m_SockTcp.Send(pBuff, nSize);
    if (nRet <= 0)
    {
        MessageBox("��������ʧ��", "��ʾ", MB_OK);
        delete[] pBuff;
        return;
    }

    delete[] pBuff;
}


//List�ؼ��Ҽ�����
void CStudentInfoManageClientDlg::OnRclickLstList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    *pResult = 0;


    //�������������������ListCtrl��
    m_ListCtrl.ClientToScreen(&pNMItemActivate->ptAction);

    CMenu menu;

    //����Դ���ز˵�
    menu.LoadMenu(IDR_MENU1);

    //��ȡ�Ӳ˵�
    CMenu* pSubMenu = menu.GetSubMenu(0);

    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    m_nIdx = m_ListCtrl.GetNextSelectedItem(pos);

    if (m_nIdx == -1)
    {
        return;
    }

    //�����Ӳ˵�
    pSubMenu->TrackPopupMenu(
        TPM_LEFTALIGN,
        pNMItemActivate->ptAction.x,
        pNMItemActivate->ptAction.y,
        this);
}
