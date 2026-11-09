
// lab2_pr_desnoView.h : interface of the Clab2prdesnoView class
//

#pragma once


class Clab2prdesnoView : public CView
{
protected: // create from serialization only
	Clab2prdesnoView() noexcept;
	DECLARE_DYNCREATE(Clab2prdesnoView)
	bool   grid;
	HENHMETAFILE m_hCactus;
	HENHMETAFILE m_hCactusLight;
	float  m_angle1;
	float  m_angle2;
// Attributes
public:
	Clab2prdesnoDoc* GetDocument() const;

// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawJointAt(CDC* pDC, float x, float y);
	void DrawPartAt(CDC* pDC, float x, float y, float angle, float sx, float sy, bool light);
	void DrawJoint(CDC* pDC, int r);
	void DrawPot(CDC* pDC);
	void DrawFigure(CDC* pDC);
	void DrawGrid(CDC* pDC, int size, int cell);
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
	virtual ~Clab2prdesnoView();
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

#ifndef _DEBUG  // debug version in lab2_pr_desnoView.cpp
inline Clab2prdesnoDoc* Clab2prdesnoView::GetDocument() const
   { return reinterpret_cast<Clab2prdesnoDoc*>(m_pDocument); }
#endif

