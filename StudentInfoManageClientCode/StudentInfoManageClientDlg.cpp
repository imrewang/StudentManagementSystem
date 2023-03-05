
// StudentInfoManageClientDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_ABOUTBOX };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CStudentInfoManageClientDlg 对话框



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


// CStudentInfoManageClientDlg 消息处理程序

BOOL CStudentInfoManageClientDlg::OnInitDialog()
{
    //启动网络库
    if (!m_SockInit.Startup())
    {
        MessageBox("打开网络库失败", "提示", MB_OK);
        PostMessage(WM_CLOSE, NULL, NULL);
        return FALSE;
    }

    //创建socket
    if (!m_SockTcp.CreateSocket())
    {
        MessageBox("创建socket失败！", "提示", MB_OK);
        PostMessage(WM_CLOSE, NULL, NULL);
        return FALSE;
    }

    //弹出连接对话框
    CConnectServer ConnectServerDlg;
    ConnectServerDlg.SetSocket(&m_SockTcp);
    if (ConnectServerDlg.DoModal() == IDCANCEL)
    {
        PostMessage(WM_CLOSE, NULL, NULL);
        return FALSE;
    }


    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
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

    // 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);			// 设置大图标
    SetIcon(m_hIcon, FALSE);		// 设置小图标

    //创建接收数据线程
    HANDLE hThread = CreateThread(NULL, 0, DataThread, this, 0, NULL);
    CloseHandle(hThread);

    //设置ListCtrl风格
    m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    //添加表头
    m_ListCtrl.InsertColumn(0, "学号", LVCFMT_LEFT, 180);
    m_ListCtrl.InsertColumn(1, "姓名", LVCFMT_LEFT, 180);
    m_ListCtrl.InsertColumn(2, "性别", LVCFMT_LEFT, 180);
    m_ListCtrl.InsertColumn(3, "出生日期", LVCFMT_LEFT, 300);

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStudentInfoManageClientDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStudentInfoManageClientDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


//接收数据线程
DWORD WINAPI CStudentInfoManageClientDlg::DataThread(LPVOID lpParameter)
{
    CStudentInfoManageClientDlg* pThis = (CStudentInfoManageClientDlg*)lpParameter;

    while (true)
    {
        HEAD_INFO head;

        //接收头数据
        int nRet = pThis->m_SockTcp.Recv((char*)&head, sizeof(head));

        if (nRet <= 0)
        {
            pThis->PostMessage(RECV_DATA_FAILED, NULL, NULL);
            break;
        }

        //匹配数据
        pThis->Matching(head);
    }

    return 0;
}

//匹配数据
void CStudentInfoManageClientDlg::Matching(HEAD_INFO head)
{
    switch (head.m_Cmd)
    {
    case OP_INSERTOK://增加成功
    {
        MessageBox("增加成功", "提示", MB_OK);
        break;
    }
    case OP_INSERTERROR://增加失败
    {
        MessageBox("增加失败", "提示", MB_OK);
        break;
    }
    case OP_DELETEOK://删除成功
    {
        m_ListCtrl.DeleteItem(m_nIdx);
        MessageBox("删除成功", "提示", MB_OK);
        break;
    }
    case OP_DELETEERROR://删除失败
    {
        MessageBox("删除失败", "提示", MB_OK);
        break;
    }
    case OP_UPDATEOK://修改成功
    {
        MatchingUpdateOk();
        MessageBox("修改成功", "提示", MB_OK);
        break;
    }
    case OP_UPDATEERROR://修改失败
    {
        MessageBox("修改失败", "提示", MB_OK);
        break;
    }
    case OP_QUERYOK://查询成功
    {
        MatchingQueryOk();
        break;
    }
    case OP_QUERYERROR://查询失败
    {
        MessageBox("查询失败", "提示", MB_OK);
        break;
    }
    case OP_CACHEFAILED://缓存失效
    {
        //清空缓存
        m_MyMap.clear();
        break;
    }
    }
}


//匹配到修改成功
void CStudentInfoManageClientDlg::MatchingUpdateOk()
{
    m_ListCtrl.SetItemText(m_nIdx, 1, m_strStuName.GetBuffer());
    m_ListCtrl.SetItemText(m_nIdx, 2, m_strStuSex.GetBuffer());
    m_ListCtrl.SetItemText(m_nIdx, 3, m_strStuData.GetBuffer());
}

//匹配到查询成功
void CStudentInfoManageClientDlg::MatchingQueryOk()
{
    QUERY_INFO QueryInfo;

    //接收数据
    int nRet = m_SockTcp.Recv((char*)&QueryInfo, sizeof(QUERY_INFO));

    if (nRet == SOCKET_ERROR)
    {
        MessageBox("接收数据失败", "提示", MB_OK);
        return;
    }

    //将查询的数据加入缓存
    m_MyMap.insert(pair<CString, QUERY_INFO>(m_QueryCommand, QueryInfo));

    int nRow = m_ListCtrl.GetItemCount();

    //将查询的数据显示到列表控件
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


//增加按钮
void CStudentInfoManageClientDlg::OnBnClickedAdd()
{
    UpdateData(TRUE);

    if (m_EditStuID.IsEmpty())
    {
        MessageBox("请输入学生ID", "提示", MB_OK);
        return;
    }

    if (m_EditStuName.IsEmpty())
    {
        MessageBox("请输入学生姓名", "提示", MB_OK);
        return;
    }

    //获取生日信息
    CTime time;
    m_DataTimeCtrl.GetTime(time);
    CString strBirthDay = time.Format("%Y-%m-%d");

    CString strAdd;
    strAdd.Format("INSERT INTO  t_student VALUES('%s', '%s', '%s', '%s')",
        m_EditStuID.GetBuffer(),
        m_EditStuName.GetBuffer(),
        m_nSex == 0 ? "男" : "女",
        strBirthDay.GetBuffer());

    //发送数据到客户端
    HEAD_INFO head;
    head.m_Cmd = OP_INSERT;
    head.m_Len = strAdd.GetLength() + 1;

    int nSize = sizeof(int) * 2 + head.m_Len;
    char* pBuff = new char[nSize];
    ZeroMemory(pBuff, nSize);

    memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
    memcpy(pBuff + sizeof(HEAD_INFO), strAdd.GetBuffer(), head.m_Len);

    //发送数据
    int nRet = m_SockTcp.Send(pBuff, nSize);
    if (nRet <= 0)
    {
        MessageBox("发送数据失败", "提示", MB_OK);
        delete[] pBuff;
        return;
    }

    delete[] pBuff;
}


//查询按钮
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
        strQuery += m_nSex == 0 ? "男'" : "女'";
    }
    if (m_bQueryData)
    {
        //获取生日信息
        CTime time;
        m_DataTimeCtrl.GetTime(time);
        CString strBirthDay = time.Format("%Y-%m-%d");

        strQuery += " AND student_data = '" + strBirthDay + "'";
    }

    m_QueryCommand = strQuery;

    //先查询缓存中的数据，如果有数据则不向服务器请求数据

    //找到元素的个数
    sz_type nNum = m_MyMap.count(m_QueryCommand);

    //如果没有找到，向服务器请求数据
    if (nNum == 0)
    {
        //发送数据到客户端
        HEAD_INFO head;
        head.m_Cmd = OP_QUERY;
        head.m_Len = strQuery.GetLength() + 1;

        int nSize = sizeof(int) * 2 + head.m_Len;
        char* pBuff = new char[nSize];
        ZeroMemory(pBuff, nSize);

        memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
        memcpy(pBuff + sizeof(HEAD_INFO), strQuery.GetBuffer(), head.m_Len);

        //发送数据
        int nRet = m_SockTcp.Send(pBuff, nSize);
        if (nRet <= 0)
        {
            MessageBox("发送数据失败", "提示", MB_OK);
            delete[] pBuff;
            return;
        }

        delete[] pBuff;

        //删除所有项
        m_ListCtrl.DeleteAllItems();

        return;
    }
    //如果找到了，则从缓存中拿数据
    else
    {
        //删除所有项
        m_ListCtrl.DeleteAllItems();

        //从缓存中拿数据
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


//删除
void CStudentInfoManageClientDlg::OnDelete()
{
    //获取学生ID
    CString strStuId = m_ListCtrl.GetItemText(m_nIdx, 0);

    CString strDelete;
    strDelete.Format("DELETE FROM t_student WHERE student_id = '%s'", strStuId.GetBuffer());

    //发送数据到客户端
    HEAD_INFO head;
    head.m_Cmd = OP_DELETE;
    head.m_Len = strDelete.GetLength() + 1;

    int nSize = sizeof(int) * 2 + head.m_Len;
    char* pBuff = new char[nSize];
    ZeroMemory(pBuff, nSize);

    memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
    memcpy(pBuff + sizeof(HEAD_INFO), strDelete.GetBuffer(), head.m_Len);

    //发送数据
    int nRet = m_SockTcp.Send(pBuff, nSize);
    if (nRet <= 0)
    {
        MessageBox("发送数据失败", "提示", MB_OK);
        delete[] pBuff;
        return;
    }

    delete[] pBuff;
}


//修改
void CStudentInfoManageClientDlg::OnUpdata()
{
    //获取学生ID
    CString strStuId = m_ListCtrl.GetItemText(m_nIdx, 0);

    //获取学生姓名
    CString strStuName = m_ListCtrl.GetItemText(m_nIdx, 1);

    //获取学生性别
    CString strStuSex = m_ListCtrl.GetItemText(m_nIdx, 2);

    //获取日期
    CString strStuDate = m_ListCtrl.GetItemText(m_nIdx, 3);

    //弹出修改对话框
    CMyUpdateDlg UpdateDlg;
    UpdateDlg.SetStuId(strStuId);
    UpdateDlg.SetStuName(strStuName);
    UpdateDlg.SetStuSex(strStuSex == "男" ? 0 : 1);
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

    //发送数据到客户端
    HEAD_INFO head;
    head.m_Cmd = OP_UPDATE;
    head.m_Len = strUpdate.GetLength() + 1;

    int nSize = sizeof(int) * 2 + head.m_Len;
    char* pBuff = new char[nSize];
    ZeroMemory(pBuff, nSize);

    memcpy(pBuff, (char*)&head, sizeof(HEAD_INFO));
    memcpy(pBuff + sizeof(HEAD_INFO), strUpdate.GetBuffer(), head.m_Len);

    //发送数据
    int nRet = m_SockTcp.Send(pBuff, nSize);
    if (nRet <= 0)
    {
        MessageBox("发送数据失败", "提示", MB_OK);
        delete[] pBuff;
        return;
    }

    delete[] pBuff;
}


//List控件右键单击
void CStudentInfoManageClientDlg::OnRclickLstList(NMHDR* pNMHDR, LRESULT* pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    *pResult = 0;


    //设置鼠标的坐标是相对于ListCtrl的
    m_ListCtrl.ClientToScreen(&pNMItemActivate->ptAction);

    CMenu menu;

    //从资源加载菜单
    menu.LoadMenu(IDR_MENU1);

    //获取子菜单
    CMenu* pSubMenu = menu.GetSubMenu(0);

    POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
    m_nIdx = m_ListCtrl.GetNextSelectedItem(pos);

    if (m_nIdx == -1)
    {
        return;
    }

    //弹出子菜单
    pSubMenu->TrackPopupMenu(
        TPM_LEFTALIGN,
        pNMItemActivate->ptAction.x,
        pNMItemActivate->ptAction.y,
        this);
}
