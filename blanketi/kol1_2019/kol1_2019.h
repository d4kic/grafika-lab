
// kol1_2019.h : main header file for the kol1_2019 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Ckol12019App:
// See kol1_2019.cpp for the implementation of this class
//

class Ckol12019App : public CWinApp
{
public:
	Ckol12019App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ckol12019App theApp;
