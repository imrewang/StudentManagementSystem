
// StudentInfoManageClientDlg.h : 头文件
//

#pragma once
#include "SocketInit.h"
#include "SocketTcp.h"
#include "protocol.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include <map>

using namespace std;


// CStudentInfoManageClientDlg 对话框
class CStudentInfoManageClientDlg : public CDialogEx
{
    // 构造
public:
    CStudentInfoManageClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_STUDENTINFOMANAGECLIENT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    CSocketInit m_SockInit;
    CSocketTcp m_SockTcp;
    int m_nIdx;

    //修改后的数据
    CString m_strStuName;
    CString m_strStuSex;
    CString m_strStuData;

    //用multimap容器装缓存
    CString m_QueryCommand;   //查询的命令
    multimap<CString, QUERY_INFO> m_MyMap;
    typedef multimap<CString, QUERY_INFO>::size_type sz_type;

    //接收数据线程
    static DWORD WINAPI DataThread(LPVOID lpParameter);

private:
    //匹配数据
    void Matching(HEAD_INFO head);

    //匹配到修改成功
    void MatchingUpdateOk();

    //匹配到查询成功
    void MatchingQueryOk();

public:
    afx_msg void OnClose();
    afx_msg void OnBnClickedAdd();
    afx_msg void OnBnClickedFind();
    CString m_EditStuID;
    CString m_EditStuName;
    int m_nSex;
    BOOL m_bQuerySex;
    CDateTimeCtrl m_DataTimeCtrl;
    BOOL m_bQueryData;
    CListCtrl m_ListCtrl;
    afx_msg void OnDelete();
    afx_msg void OnUpdata();
    afx_msg void OnRclickLstList(NMHDR* pNMHDR, LRESULT* pResult);
};
