
// IND_19583View.h : interface of the CIND19583View class
//

#pragma once
#include "DImage.h"

class CIND19583View : public CView
{
protected: // create from serialization only
	CIND19583View() noexcept;
	DECLARE_DYNCREATE(CIND19583View)

// Attributes
public:
	CIND19583Doc* GetDocument() const;

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
	virtual ~CIND19583View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DImage m_slike[10];
	float m_pomX, m_pomY;
	void Scale(CDC* pDC, float sX, float sY, bool desnoMnozenje);
	void Rotate(CDC* pDC, float ugao, bool desnoMnozenje);
	void Translate(CDC* pDC, float dX, float dY, bool desnoMnozenje);
	void Mirror(CDC* pDC, bool mx, bool my, bool desnoMnozenje);

	void NacrtajMrezu(CDC* pDC);
	void FilterBitaMapa(CBitmap* map, int kanal);
	void DrawPuzzlePiece(CDC* pDC, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my, int filter);
	void NacrtajDeoSlagalice(CDC* pDC, DImage& img, int filter);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IND_19583View.cpp
inline CIND19583Doc* CIND19583View::GetDocument() const
   { return reinterpret_cast<CIND19583Doc*>(m_pDocument); }
#endif

