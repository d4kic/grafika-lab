
// kol1_2022.h : main header file for the kol1_2022 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Ckol12022App:
// See kol1_2022.cpp for the implementation of this class
//

class Ckol12022App : public CWinApp
{
public:
	Ckol12022App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ckol12022App theApp;
