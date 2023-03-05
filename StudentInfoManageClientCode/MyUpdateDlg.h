#pragma once
#include "afxdtctl.h"


// CMyUpdateDlg 对话框

class CMyUpdateDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CMyUpdateDlg)

public:
    CMyUpdateDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CMyUpdateDlg();

    // 对话框数据
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_DIALOG2 };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()

public:
    CTime m_Time;
    CString m_strData;

    void SetStuId(CString id);
    void SetStuName(CString name);
    void SetStuSex(int nSex);
    void SetStuDate(CString data1);

    CString GetStuName();
    CString GetStuSex();
    CString GetStuDate();

public:
    CString m_EditStuID;
    CString m_EditStuName;
    int m_nSex;
    CDateTimeCtrl m_DataTimeCtrl;
    virtual BOOL OnInitDialog();
    afx_msg void OnClose();
    afx_msg void OnBnClickedUpdata();
};
