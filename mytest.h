
// mytest.h : main header file for the PROJECT_NAME application
//

#ifndef MY_TEST_H_
#define MY_TEST_H_

#include "resource.h"		// main symbols

#include <afxwin.h>

// CmytestApp:
// See mytest.cpp for the implementation of this class
//

class CmytestApp : public CWinApp
{
public:
	CmytestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CmytestApp theApp;
#endif // MY_TEST_H_