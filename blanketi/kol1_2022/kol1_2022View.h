
// kol1_2022View.h : interface of the Ckol12022View class
//

#pragma once
#include "DImage.h"

class Ckol12022View : public CView
{
protected: // create from serialization only
	Ckol12022View() noexcept;
	DECLARE_DYNCREATE(Ckol12022View)

// Attributes
public:
	Ckol12022Doc* GetDocument() const;

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
	virtual ~Ckol12022View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DImage* base;
	DImage* arm1;
	DImage* arm2;
	DImage* head;
	DImage* pozadina;
	DImage* sbase;
	DImage* sarm1;
	DImage* sarm2;
	DImage* shead;

	float ugaoArm1 = -50;
	float ugaoArm2 = -90;
	float ugaoHead = 180;

	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void DrawBackground(CDC* pDC);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void DrawLampBase(CDC* pDC, bool bIsShadow);
	void DrawLampArm1(CDC* pDC, bool bIsShadow);
	void DrawLampArm2(CDC* pDC, bool bIsShadow);
	void DrawLampHead(CDC* pDC, bool bIsShadow);
	void DrawLamp(CDC* pDC, bool bIsShadow);
	void DrawLampShadow(CDC* pDC);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in kol1_2022View.cpp
inline Ckol12022Doc* Ckol12022View::GetDocument() const
   { return reinterpret_cast<Ckol12022Doc*>(m_pDocument); }
#endif

