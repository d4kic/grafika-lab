
// kol1_2017View.h : interface of the Ckol12017View class
//

#pragma once
#include "DImage.h"

class Ckol12017View : public CView
{
protected: // create from serialization only
	Ckol12017View() noexcept;
	DECLARE_DYNCREATE(Ckol12017View)

// Attributes
public:
	Ckol12017Doc* GetDocument() const;

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
	virtual ~Ckol12017View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DImage* telo;
	DImage* pozadina;
	DImage* noga1;
	DImage* noga2;
	DImage* noga3;

	float ugaoRotPrednje;
	float ugaoRotZadnje;
	float predjeniPut;
	float skaliranje;
	bool prednjeNapred;
	bool zadnjeNapred;
	float pozadinaX;
	float pozadinaY;
	float rectH;
	float rectW;

	void LoadIdentity(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);

	void DrawScene(CDC* pDC, CRect rect);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in kol1_2017View.cpp
inline Ckol12017Doc* Ckol12017View::GetDocument() const
   { return reinterpret_cast<Ckol12017Doc*>(m_pDocument); }
#endif

