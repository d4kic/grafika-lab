
// lab1priprema.h : main header file for the lab1priprema application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Clab1pripremaApp:
// See lab1priprema.cpp for the implementation of this class
//

class Clab1pripremaApp : public CWinApp
{
public:
	Clab1pripremaApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Clab1pripremaApp theApp;
