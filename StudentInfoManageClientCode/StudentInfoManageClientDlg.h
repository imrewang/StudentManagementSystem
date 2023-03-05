
// StudentInfoManageClientDlg.h : ͷ�ļ�
//

#pragma once
#include "SocketInit.h"
#include "SocketTcp.h"
#include "protocol.h"
#include "afxdtctl.h"
#include "afxcmn.h"
#include <map>

using namespace std;


// CStudentInfoManageClientDlg �Ի���
class CStudentInfoManageClientDlg : public CDialogEx
{
    // ����
public:
    CStudentInfoManageClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_STUDENTINFOMANAGECLIENT_DIALOG };
#endif

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

private:
    CSocketInit m_SockInit;
    CSocketTcp m_SockTcp;
    int m_nIdx;

    //�޸ĺ������
    CString m_strStuName;
    CString m_strStuSex;
    CString m_strStuData;

    //��multimap����װ����
    CString m_QueryCommand;   //��ѯ������
    multimap<CString, QUERY_INFO> m_MyMap;
    typedef multimap<CString, QUERY_INFO>::size_type sz_type;

    //���������߳�
    static DWORD WINAPI DataThread(LPVOID lpParameter);

private:
    //ƥ������
    void Matching(HEAD_INFO head);

    //ƥ�䵽�޸ĳɹ�
    void MatchingUpdateOk();

    //ƥ�䵽��ѯ�ɹ�
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
