
// mytestDlg.cpp : implementation file
//
#include "stdafx.h"
#include "mytest.h"
#include "mytestDlg.h"
#include "afxdialogex.h"
#include "png_decoder.h"

#include "third_party/libpng/pngpriv.h"

#include <iostream>  
#include <sstream> 
#include <vector>  
#include <bitset>  
#include <array>  
#include <string>  

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace {

  static const unsigned int kDisIntervalTimer = 0x701;
  static const unsigned int kDisIntervalTimeLong = 300;

  void WalkDirs(CString csPath, const CString& ext, std::list<std::wstring>& out_set) {
    CString csPrePath = csPath;
    CString csNextPath = csPath;
    CFileFind ff;
    csPath += _T("*.*");//遍历这一级全部的目录
    int nResult = ff.FindFile(csPath);
    while (nResult)
    {
      nResult = ff.FindNextFileW();
      if (ff.IsDirectory() && !ff.IsDots())
      {
        csNextPath += ff.GetFileName();
        csNextPath += _T("\\");
        WalkDirs(csNextPath, ext, out_set);// 递归
      } else if (!ff.IsDirectory() && !ff.IsDots()) {
        int index = ff.GetFileName().ReverseFind('.');
        if (index != -1) {
          CString fileext = ff.GetFileName().Right(ff.GetFileName().GetLength() - index);
          if (fileext == ext) {
            out_set.push_back(std::wstring(ff.GetFilePath()));
          }
        }
      }
      csNextPath = csPrePath;
    }
  }


#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

  //Aero效果是否已启用
  BOOL IsCompositionEnabled()
  {
    BOOL bEnabled, bResult;
    bResult = (SUCCEEDED(DwmIsCompositionEnabled(&bEnabled)) && bEnabled);
    return bResult;
  }

  //对已分层的窗口启动玻璃效果
  HRESULT EnableBlurBehindWindow(HWND hWnd, //窗口句柄
    BOOL bEnable = TRUE, //启用或禁用
    HRGN hRgn = 0, //模糊窗体中某个区域
    BOOL bTransitionOnMaximized = FALSE) //最大化时是否启用
  {
    DWM_BLURBEHIND blurBehind = { 0 };
    blurBehind.dwFlags = DWM_BB_ENABLE | DWM_BB_TRANSITIONONMAXIMIZED;
    blurBehind.fEnable = bEnable;
    blurBehind.fTransitionOnMaximized = bTransitionOnMaximized;
    if (bEnable && hRgn != NULL)
    {
      blurBehind.dwFlags |= DWM_BB_BLURREGION;
      blurBehind.hRgnBlur = hRgn;
    }
    return DwmEnableBlurBehindWindow(hWnd, &blurBehind);
  }


  class InstructionSet
  {
    // forward declarations  
    class InstructionSet_Internal;

  public:
    // getters  
    static std::string Vendor(void) { return CPU_Rep.vendor_; }
    static std::string Brand(void) { return CPU_Rep.brand_; }

    static bool SSE3(void) { return CPU_Rep.f_1_ECX_[0]; }
    static bool PCLMULQDQ(void) { return CPU_Rep.f_1_ECX_[1]; }
    static bool MONITOR(void) { return CPU_Rep.f_1_ECX_[3]; }
    static bool SSSE3(void) { return CPU_Rep.f_1_ECX_[9]; }
    static bool FMA(void) { return CPU_Rep.f_1_ECX_[12]; }
    static bool CMPXCHG16B(void) { return CPU_Rep.f_1_ECX_[13]; }
    static bool SSE41(void) { return CPU_Rep.f_1_ECX_[19]; }
    static bool SSE42(void) { return CPU_Rep.f_1_ECX_[20]; }
    static bool MOVBE(void) { return CPU_Rep.f_1_ECX_[22]; }
    static bool POPCNT(void) { return CPU_Rep.f_1_ECX_[23]; }
    static bool AES(void) { return CPU_Rep.f_1_ECX_[25]; }
    static bool XSAVE(void) { return CPU_Rep.f_1_ECX_[26]; }
    static bool OSXSAVE(void) { return CPU_Rep.f_1_ECX_[27]; }
    static bool AVX(void) { return CPU_Rep.f_1_ECX_[28]; }
    static bool F16C(void) { return CPU_Rep.f_1_ECX_[29]; }
    static bool RDRAND(void) { return CPU_Rep.f_1_ECX_[30]; }

    static bool MSR(void) { return CPU_Rep.f_1_EDX_[5]; }
    static bool CX8(void) { return CPU_Rep.f_1_EDX_[8]; }
    static bool SEP(void) { return CPU_Rep.f_1_EDX_[11]; }
    static bool CMOV(void) { return CPU_Rep.f_1_EDX_[15]; }
    static bool CLFSH(void) { return CPU_Rep.f_1_EDX_[19]; }
    static bool MMX(void) { return CPU_Rep.f_1_EDX_[23]; }
    static bool FXSR(void) { return CPU_Rep.f_1_EDX_[24]; }
    static bool SSE(void) { return CPU_Rep.f_1_EDX_[25]; }
    static bool SSE2(void) { return CPU_Rep.f_1_EDX_[26]; }

    static bool FSGSBASE(void) { return CPU_Rep.f_7_EBX_[0]; }
    static bool BMI1(void) { return CPU_Rep.f_7_EBX_[3]; }
    static bool HLE(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[4]; }
    static bool AVX2(void) { return CPU_Rep.f_7_EBX_[5]; }
    static bool BMI2(void) { return CPU_Rep.f_7_EBX_[8]; }
    static bool ERMS(void) { return CPU_Rep.f_7_EBX_[9]; }
    static bool INVPCID(void) { return CPU_Rep.f_7_EBX_[10]; }
    static bool RTM(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_7_EBX_[11]; }
    static bool AVX512F(void) { return CPU_Rep.f_7_EBX_[16]; }
    static bool RDSEED(void) { return CPU_Rep.f_7_EBX_[18]; }
    static bool ADX(void) { return CPU_Rep.f_7_EBX_[19]; }
    static bool AVX512PF(void) { return CPU_Rep.f_7_EBX_[26]; }
    static bool AVX512ER(void) { return CPU_Rep.f_7_EBX_[27]; }
    static bool AVX512CD(void) { return CPU_Rep.f_7_EBX_[28]; }
    static bool SHA(void) { return CPU_Rep.f_7_EBX_[29]; }

    static bool PREFETCHWT1(void) { return CPU_Rep.f_7_ECX_[0]; }

    static bool LAHF(void) { return CPU_Rep.f_81_ECX_[0]; }
    static bool LZCNT(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_ECX_[5]; }
    static bool ABM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[5]; }
    static bool SSE4a(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[6]; }
    static bool XOP(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[11]; }
    static bool TBM(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_ECX_[21]; }

    static bool SYSCALL(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[11]; }
    static bool MMXEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[22]; }
    static bool RDTSCP(void) { return CPU_Rep.isIntel_ && CPU_Rep.f_81_EDX_[27]; }
    static bool _3DNOWEXT(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[30]; }
    static bool _3DNOW(void) { return CPU_Rep.isAMD_ && CPU_Rep.f_81_EDX_[31]; }

  private:
    static const InstructionSet_Internal CPU_Rep;

    class InstructionSet_Internal
    {
    public:
      InstructionSet_Internal()
        : nIds_{ 0 },
        nExIds_{ 0 },
        isIntel_{ false },
        isAMD_{ false },
        f_1_ECX_{ 0 },
        f_1_EDX_{ 0 },
        f_7_EBX_{ 0 },
        f_7_ECX_{ 0 },
        f_81_ECX_{ 0 },
        f_81_EDX_{ 0 },
        data_{},
        extdata_{}
      {
        //int cpuInfo[4] = {-1};  
        std::array<int, 4> cpui;

        // Calling __cpuid with 0x0 as the function_id argument  
        // gets the number of the highest valid function ID.  
        __cpuid(cpui.data(), 0);
        nIds_ = cpui[0];

        for (int i = 0; i <= nIds_; ++i)
        {
          __cpuidex(cpui.data(), i, 0);
          data_.push_back(cpui);
        }

        // Capture vendor string  
        char vendor[0x20];
        memset(vendor, 0, sizeof(vendor));
        *reinterpret_cast<int*>(vendor) = data_[0][1];
        *reinterpret_cast<int*>(vendor + 4) = data_[0][3];
        *reinterpret_cast<int*>(vendor + 8) = data_[0][2];
        vendor_ = vendor;
        if (vendor_ == "GenuineIntel")
        {
          isIntel_ = true;
        }
        else if (vendor_ == "AuthenticAMD")
        {
          isAMD_ = true;
        }

        // load bitset with flags for function 0x00000001  
        if (nIds_ >= 1)
        {
          f_1_ECX_ = data_[1][2];
          f_1_EDX_ = data_[1][3];
        }

        // load bitset with flags for function 0x00000007  
        if (nIds_ >= 7)
        {
          f_7_EBX_ = data_[7][1];
          f_7_ECX_ = data_[7][2];
        }

        // Calling __cpuid with 0x80000000 as the function_id argument  
        // gets the number of the highest valid extended ID.  
        __cpuid(cpui.data(), 0x80000000);
        nExIds_ = cpui[0];

        char brand[0x40];
        memset(brand, 0, sizeof(brand));

        for (int i = 0x80000000; i <= nExIds_; ++i)
        {
          __cpuidex(cpui.data(), i, 0);
          extdata_.push_back(cpui);
        }

        // load bitset with flags for function 0x80000001  
        if (nExIds_ >= 0x80000001)
        {
          f_81_ECX_ = extdata_[1][2];
          f_81_EDX_ = extdata_[1][3];
        }

        // Interpret CPU brand string if reported  
        if (nExIds_ >= 0x80000004)
        {
          memcpy(brand, extdata_[2].data(), sizeof(cpui));
          memcpy(brand + 16, extdata_[3].data(), sizeof(cpui));
          memcpy(brand + 32, extdata_[4].data(), sizeof(cpui));
          brand_ = brand;
        }
      };

      int nIds_;
      int nExIds_;
      std::string vendor_;
      std::string brand_;
      bool isIntel_;
      bool isAMD_;
      std::bitset<32> f_1_ECX_;
      std::bitset<32> f_1_EDX_;
      std::bitset<32> f_7_EBX_;
      std::bitset<32> f_7_ECX_;
      std::bitset<32> f_81_ECX_;
      std::bitset<32> f_81_EDX_;
      std::vector<std::array<int, 4>> data_;
      std::vector<std::array<int, 4>> extdata_;
    };
  };

  // Initialize static member data  
  const InstructionSet::InstructionSet_Internal InstructionSet::CPU_Rep;

  std::wstring GetCpuTest() {
    std::wostringstream stream;
    auto& outstream = stream;

    auto support_message = [&outstream](std::wstring isa_feature, bool is_supported) {
      outstream << isa_feature << (is_supported ? L" supported" : L" not supported") << L"\r\n";
    };

    std::cout << InstructionSet::Vendor() << std::endl;
    std::cout << InstructionSet::Brand() << std::endl;

    support_message(L"3DNOW", InstructionSet::_3DNOW());
    support_message(L"3DNOWEXT", InstructionSet::_3DNOWEXT());
    support_message(L"ABM", InstructionSet::ABM());
    support_message(L"ADX", InstructionSet::ADX());
    support_message(L"AES", InstructionSet::AES());
    support_message(L"AVX", InstructionSet::AVX());
    support_message(L"AVX2", InstructionSet::AVX2());
    support_message(L"AVX512CD", InstructionSet::AVX512CD());
    support_message(L"AVX512ER", InstructionSet::AVX512ER());
    support_message(L"AVX512F", InstructionSet::AVX512F());
    support_message(L"AVX512PF", InstructionSet::AVX512PF());
    support_message(L"BMI1", InstructionSet::BMI1());
    support_message(L"BMI2", InstructionSet::BMI2());
    support_message(L"CLFSH", InstructionSet::CLFSH());
    support_message(L"CMPXCHG16B", InstructionSet::CMPXCHG16B());
    support_message(L"CX8", InstructionSet::CX8());
    support_message(L"ERMS", InstructionSet::ERMS());
    support_message(L"F16C", InstructionSet::F16C());
    support_message(L"FMA", InstructionSet::FMA());
    support_message(L"FSGSBASE", InstructionSet::FSGSBASE());
    support_message(L"FXSR", InstructionSet::FXSR());
    support_message(L"HLE", InstructionSet::HLE());
    support_message(L"INVPCID", InstructionSet::INVPCID());
    support_message(L"LAHF", InstructionSet::LAHF());
    support_message(L"LZCNT", InstructionSet::LZCNT());
    support_message(L"MMX", InstructionSet::MMX());
    support_message(L"MMXEXT", InstructionSet::MMXEXT());
    support_message(L"MONITOR", InstructionSet::MONITOR());
    support_message(L"MOVBE", InstructionSet::MOVBE());
    support_message(L"MSR", InstructionSet::MSR());
    support_message(L"OSXSAVE", InstructionSet::OSXSAVE());
    support_message(L"PCLMULQDQ", InstructionSet::PCLMULQDQ());
    support_message(L"POPCNT", InstructionSet::POPCNT());
    support_message(L"PREFETCHWT1", InstructionSet::PREFETCHWT1());
    support_message(L"RDRAND", InstructionSet::RDRAND());
    support_message(L"RDSEED", InstructionSet::RDSEED());
    support_message(L"RDTSCP", InstructionSet::RDTSCP());
    support_message(L"RTM", InstructionSet::RTM());
    support_message(L"SEP", InstructionSet::SEP());
    support_message(L"SHA", InstructionSet::SHA());
    support_message(L"SSE", InstructionSet::SSE());
    support_message(L"SSE2", InstructionSet::SSE2());
    support_message(L"SSE3", InstructionSet::SSE3());
    support_message(L"SSE4.1", InstructionSet::SSE41());
    support_message(L"SSE4.2", InstructionSet::SSE42());
    support_message(L"SSE4a", InstructionSet::SSE4a());
    support_message(L"SSSE3", InstructionSet::SSSE3());
    support_message(L"SYSCALL", InstructionSet::SYSCALL());
    support_message(L"TBM", InstructionSet::TBM());
    support_message(L"XOP", InstructionSet::XOP());
    support_message(L"XSAVE", InstructionSet::XSAVE());

    return stream.str();
  }
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
  CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
  enum { IDD = IDD_ABOUTBOX };
#endif
  BOOL OnInitDialog() override;
  protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
  DECLARE_MESSAGE_MAP()

  CEdit c_cpu_;
  afx_msg LRESULT OnLoadcpuinfo(WPARAM wParam, LPARAM lParam);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

#define WM_LOADCPUINFO WM_USER + 706

BOOL CAboutDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();
  c_cpu_.SubclassDlgItem(IDC_EDIT_CPU, this);
  PostMessage(WM_LOADCPUINFO);
  return TRUE;  // return TRUE  unless you set the focus to a control
}


afx_msg LRESULT CAboutDlg::OnLoadcpuinfo(WPARAM wParam, LPARAM lParam)
{
  std::wstring cpuinfo = GetCpuTest();
  c_cpu_.SetWindowTextW(cpuinfo.c_str());
  return 0;
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
  ON_MESSAGE(WM_LOADCPUINFO, &CAboutDlg::OnLoadcpuinfo)
END_MESSAGE_MAP()


// CmytestDlg dialog



CmytestDlg::CmytestDlg(CWnd* pParent /*=nullptr*/)
  : CDialogEx(IDD_MYTEST_DIALOG, pParent)
{
  m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_APP_ICON);
}

void CmytestDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmytestDlg, CDialogEx)
  ON_WM_SYSCOMMAND()
  ON_WM_PAINT()
  ON_WM_QUERYDRAGICON()
  ON_BN_CLICKED(IDC_BUTTON_FILE, &CmytestDlg::OnBnClickedButtonFile)
  ON_BN_CLICKED(IDC_BUTTON_DISP, &CmytestDlg::OnBnClickedButtonDisp)
  ON_BN_CLICKED(IDC_BUTTON_DIR, &CmytestDlg::OnBnClickedButtonDir)
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CmytestDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CmytestDlg message handlers

BOOL CmytestDlg::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // Add "About..." menu item to system menu.

  // IDM_ABOUTBOX must be in the system command range.
  ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
  ASSERT(IDM_ABOUTBOX < 0xF000);

  CMenu* pSysMenu = GetSystemMenu(FALSE);
  if (pSysMenu != nullptr)
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

  // 查找资源
  HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDB_PNG_ERROR), L"PNG");
  if (hRsrc != NULL) {

    // 加载资源
    HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
    if (hImgData == NULL)
    {
      ::FreeResource(hImgData);
    }
    else {
      // 锁定内存中的指定资源
      LPVOID lpVoid = ::LockResource(hImgData);

      LPSTREAM pStream = NULL;
      DWORD dwSize = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
      HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
      LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
      ::memcpy(lpByte, lpVoid, dwSize);

      // 解除内存中的指定资源
      ::GlobalUnlock(hNew);

      // 从指定内存创建流对象
      HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
      if (ht != S_OK)
      {
        GlobalFree(hNew);
      }
      else
      {
        // 加载图片
        errimage_.Load(pStream);
        errbitmap_ = errimage_.Detach();

        GlobalFree(hNew);
      }

      // 释放资源
      ::FreeResource(hImgData);
    }
  }

  c_file_.SubclassDlgItem(IDC_EDIT_FILE, this);
  c_pic_.SubclassDlgItem(IDC_STATIC_PIC, this);
  c_log_.SubclassDlgItem(IDC_EDIT_LOG, this);
  c_info_.SubclassDlgItem(IDC_EDIT_INFO, this);

  //if (IsCompositionEnabled() == TRUE)
  //{
  //  EnableBlurBehindWindow(m_hWnd, TRUE);
  //}

  //CFont myFont;               //定义字体对象  
  //myFont.CreateFontIndirect(&lf);  //创建逻辑字体 

  // Set the icon for this dialog.  The framework does this automatically
  //  when the application's main window is not a dialog
  SetIcon(m_hIcon, TRUE);			// Set big icon
  SetIcon(m_hIcon, FALSE);		// Set small icon

  // TODO: Add extra initialization here

  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CmytestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CmytestDlg::OnPaint()
{
  if (IsIconic())
  {
    CPaintDC dc(this); // device context for painting

    SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

    // Center icon in client rectangle
    int cxIcon = GetSystemMetrics(SM_CXICON);
    int cyIcon = GetSystemMetrics(SM_CYICON);
    CRect rect;
    GetClientRect(&rect);
    int x = (rect.Width() - cxIcon + 1) / 2;
    int y = (rect.Height() - cyIcon + 1) / 2;

    // Draw the icon
    dc.DrawIcon(x, y, m_hIcon);
  }
  else
  {
    CDialogEx::OnPaint();
  }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmytestDlg::OnQueryDragIcon()
{
  return static_cast<HCURSOR>(m_hIcon);
}



void CmytestDlg::OnBnClickedButtonFile()
{
  // TODO: Add your control notification handler code here
  KillTimer(kDisIntervalTimer);

  CString strFile = _T("");

  CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.png)|*.png|All Files (*.*)|*.*||"), NULL);

  if (dlgFile.DoModal())
  {
    strFile = dlgFile.GetPathName();
  }

  if (!strFile.IsEmpty()) {
    c_file_.SetWindowTextW(strFile);
  }
}


void CmytestDlg::OnBnClickedButtonDisp()
{
  KillTimer(kDisIntervalTimer);
  log_.clear();
  c_log_.SetWindowTextW(L"");
  BuildFileList();
  CheckAndDispImageList();
  // TODO: Add your control notification handler code here
  if (file_list_.size() >= 1) {
    SetTimer(kDisIntervalTimer, kDisIntervalTimeLong, NULL);
  }
}


void CmytestDlg::OnBnClickedButtonDir()
{
  KillTimer(kDisIntervalTimer);

  // TODO: Add your control notification handler code here
  TCHAR           szFolderPath[MAX_PATH] = { 0 };
  CString         strFolderPath = TEXT("");

  BROWSEINFO      sInfo;
  ::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
  sInfo.pidlRoot = 0;
  sInfo.lpszTitle = _T("请选择需要遍历PNG图片的文件夹");
  sInfo.ulFlags =  BIF_NONEWFOLDERBUTTON | BIF_USENEWUI | BIF_EDITBOX;
  sInfo.lpfn = NULL;

  // 显示文件夹选择对话框  
  LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
  if (lpidlBrowse != NULL)
  {
    // 取得文件夹名  
    if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
    {
      strFolderPath = szFolderPath;
    }
  }
  if (lpidlBrowse != NULL)
  {
    ::CoTaskMemFree(lpidlBrowse);
  }

  if (!strFolderPath.IsEmpty()) {
    c_file_.SetWindowTextW(strFolderPath);
  }
}


void CmytestDlg::BuildFileList()
{
  CString file_or_path;
  c_file_.GetWindowTextW(file_or_path);

  if (!PathFileExists(file_or_path))
    return;

  file_list_.clear();

  if (GetFileAttributes(file_or_path) & FILE_ATTRIBUTE_DIRECTORY) {
    if (file_or_path.Right(2) != "\\") {
      file_or_path += "\\";
    }
    WalkDirs(file_or_path, L".png", file_list_);
  } else {
    // single file
    file_list_.push_back(std::wstring(file_or_path));
  }

  all_ = file_list_.size();
  ok_ = 0;
  err_ = 0;
}


void CmytestDlg::OnTimer(UINT_PTR nIDEvent)
{
  // TODO: Add your message handler code here and/or call default

  if (nIDEvent == kDisIntervalTimer) {
    CheckAndDispImageList();

    if (file_list_.size() == 0) {
      KillTimer(kDisIntervalTimer);
    }
  }

  CDialogEx::OnTimer(nIDEvent);
}

void CmytestDlg::SetLineColor(int line, COLORREF color) {
  CRichEditCtrl m_cRichEdit;
  int lineStart, lineEnd;
  CHARFORMAT cFmt;
  cFmt.cbSize = sizeof(CHARFORMAT);
  cFmt.crTextColor = color;
  cFmt.dwEffects = 0;
  cFmt.dwMask = CFM_COLOR;

  lineStart = c_log_.LineIndex(line);//取第一行的第一个字符的索引
  int nLineLength = c_log_.LineLength(line);
  lineEnd = lineStart + nLineLength;
  c_log_.SetSel(lineStart, lineEnd);//选取第一行字符
  c_log_.SetSelectionCharFormat(cFmt);//设置颜色
}

void CmytestDlg::CheckAndDispImageList() {
  if (file_list_.size() >= 1) {
    std::wstring file = file_list_.front();
    file_list_.pop_front();
    CString cstr_file(file.c_str());

#if USE_LIBPNG
    CFile mFile;
    LONG  dwSize;

    //1　打开文件并获得文件的真实大小
    if (mFile.Open(cstr_file, CFile::modeRead | CFile::shareDenyNone) && ((dwSize = mFile.GetLength()) > 0))
    {
      //2　从堆中分配指定数量字节的一整块，这时系统无法提供零零碎碎的局部或全局的堆
      HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize);
      LPVOID pvData = NULL;
      if (hGlobal != NULL)
      {
        //3　锁定全局内存对象并返回它的首地址
        if ((pvData = GlobalLock(hGlobal)) != NULL)
        {
          mFile.Read(pvData, dwSize);
          mFile.Close();
          GlobalUnlock(hGlobal);

          std::vector<unsigned char> bits;
          int w, h;
          bool ok = PngDecoder::Decode((unsigned char*)pvData, dwSize, PngDecoder::ColorFormat::FORMAT_SkBitmap, &bits, &w, &h);
          if (!ok) {

            GlobalFree(hGlobal);
            err_++;
            DispLog(cstr_file.GetString(), false);
            c_pic_.SetBitmap(errbitmap_);
            MessageBox(cstr_file + L" 解码失败", L"Error", MB_OK);
            return;
          }

          HBITMAP bt = CreateBitmap(w, h, 32, 1, bits.data());

          c_pic_.SetBitmap(bt);

          GlobalFree(hGlobal);
        }
      }
    } else {
      err_++;
      DispLog(cstr_file.GetString(), false);
      MessageBox(L"读取png文件失败！", L"Error", MB_OK);
      return;
    }
#else
    image_.Destroy();
    image_.Load(cstr_file);
    bitmap_ = image_.Detach();
    c_pic_.SetBitmap(bitmap_);
#endif
    ok_++;
    DispLog(cstr_file.GetString(), true);
  }
}

void CmytestDlg::DispLog(std::wstring file, bool ok) {

  LogInfo info;
  info.ok = ok;
  info.logline = L"image:" + file + L" OK";
  log_.push_back(info);

  std::wstring log_str;
  for (auto& item : log_) {
    log_str += item.logline + L"\n";
  }

  c_log_.SetWindowTextW(log_str.c_str());

  for (size_t i = 0; i < log_.size(); i++) {
    if (!log_[i].ok)
      SetLineColor(i, RGB(255, 0, 0));
  }

  c_log_.LineScroll(c_log_.GetLineCount());

  std::wstring info_sum = L"All:" + std::to_wstring(all_) + L" Ok:" + std::to_wstring(ok_) + L" Err:" + std::to_wstring(err_);
  c_info_.SetWindowTextW(info_sum.c_str());
}

void CmytestDlg::OnBnClickedButtonClear()
{
  // TODO: Add your control notification handler code here

  KillTimer(kDisIntervalTimer);
  log_.clear();
  c_log_.SetWindowTextW(L"");
  c_pic_.SetBitmap(NULL);
  c_file_.SetWindowTextW(L"");
  file_list_.clear();
  all_ = 0;
  ok_ = 0;
  err_ = 0;
  c_info_.SetWindowTextW(L"");
}
