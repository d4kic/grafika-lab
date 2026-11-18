
// kol1_2023View.h : interface of the Ckol12023View class
//

#pragma once
#include "DImage.h"

class Ckol12023View : public CView
{
protected: // create from serialization only
	Ckol12023View() noexcept;
	DECLARE_DYNCREATE(Ckol12023View)

// Attributes
public:
	Ckol12023Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Ckol12023View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DImage* glava;
	DImage* nadkolenica;
	DImage* nadlaktica;
	DImage* potkolenica;
	DImage* podlaktica;
	DImage* saka;
	DImage* stopalo;
	DImage* telo;
	DImage* pozadina;

	float ugaoNadlaktica = 0;
	float ugaoPodlaktica = 0;
	float ugaoSaka = 0;
	float ugaoRobot = 0;
	float skaliranje = 1;

	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawHalf(CDC* pDC);
	void DrawHead(CDC* pDC);
	void DrawRobot(CDC* pDC);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in kol1_2023View.cpp
inline Ckol12023Doc* Ckol12023View::GetDocument() const
   { return reinterpret_cast<Ckol12023Doc*>(m_pDocument); }
#endif

