
// lab1pripremaView.h : interface of the Clab1pripremaView class
//

#pragma once


class Clab1pripremaView : public CView
{
protected: // create from serialization only
	Clab1pripremaView() noexcept;
	DECLARE_DYNCREATE(Clab1pripremaView)
	bool grid;
// Attributes
public:
	Clab1pripremaDoc* GetDocument() const;

// Operations
public:
	void DrawGrid(CDC* pDC, int size, int cell);
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	void DrawPolygon(CDC* pDC, CPoint* points, int n);
	CPoint Tacka(float nx, float ny);
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
	virtual ~Clab1pripremaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in lab1pripremaView.cpp
inline Clab1pripremaDoc* Clab1pripremaView::GetDocument() const
   { return reinterpret_cast<Clab1pripremaDoc*>(m_pDocument); }
#endif

