
// kol1_2023.h : main header file for the kol1_2023 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Ckol12023App:
// See kol1_2023.cpp for the implementation of this class
//

class Ckol12023App : public CWinApp
{
public:
	Ckol12023App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Ckol12023App theApp;
