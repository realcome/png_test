
// mytestDlg.h : header file
//

#ifndef MYTEST_DLG_H_
#define MYTEST_DLG_H_
#ifndef min
#define min
#endif
#ifndef max
#define max
#endif
#include <afxdialogex.h>
#include <atlimage.h>


#undef min
#undef max

#include <vector>
#include <list>
#include <string>
#define USE_LIBPNG 1
// CmytestDlg dialog
class CmytestDlg : public CDialogEx
{
// Construction
public:
	CmytestDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
  CEdit c_file_;
  CRichEditCtrl c_log_;
  CEdit c_info_;
  CStatic c_pic_;
  int all_ = 0;
  int ok_ = 0;
  int err_ = 0;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedButtonFile();
  afx_msg void OnBnClickedButtonDisp();
  afx_msg void OnBnClickedButtonDir();

private:
  void CheckAndDispImageList();

  std::list<std::wstring> file_list_;

  CImage image_;
  HBITMAP bitmap_;
  CImage errimage_;
  HBITMAP errbitmap_;

  struct LogInfo {
    std::wstring logline;
    bool ok = true;
  };

  std::vector<LogInfo> log_;

protected:
  void BuildFileList();
  void SetLineColor(int line, COLORREF color);
  void DispLog(std::wstring file, bool ok);
public:
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnBnClickedButtonClear();
};
#endif // MYTEST_DLG_H_