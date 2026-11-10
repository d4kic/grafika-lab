
// IND_19583View.h : interface of the CIND19583View class
//

#pragma once


class CIND19583View : public CView
{
protected: // create from serialization only
	CIND19583View() noexcept;
	DECLARE_DYNCREATE(CIND19583View)
	bool   grid;
	HENHMETAFILE m_hCactus;
	HENHMETAFILE m_hCactusLight;
	float  m_angle1;
	float  m_angle2;
// Attributes
public:
	CIND19583Doc* GetDocument() const;
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawJointAt(CDC* pDC, float x, float y);
	void DrawPartAt(CDC* pDC, float x, float y, float angle, float sx, float sy, bool light);
	void DrawJoint(CDC* pDC, int r);
	void DrawPot(CDC* pDC);
	void DrawFigure(CDC* pDC);
	void DrawGrid(CDC* pDC, int size, int cell);
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
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in IND_19583View.cpp
inline CIND19583Doc* CIND19583View::GetDocument() const
   { return reinterpret_cast<CIND19583Doc*>(m_pDocument); }
#endif

