
// lab1pripremaView.cpp : implementation of the Clab1pripremaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab1priprema.h"
#endif

#include "lab1pripremaDoc.h"
#include "lab1pripremaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#define SIZE 500
#define TILE 25

// Clab1pripremaView

IMPLEMENT_DYNCREATE(Clab1pripremaView, CView)

BEGIN_MESSAGE_MAP(Clab1pripremaView, CView)
	// Standard printing commands
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Clab1pripremaView construction/destruction

Clab1pripremaView::Clab1pripremaView() noexcept
{
	grid = true;
}

Clab1pripremaView::~Clab1pripremaView()
{
}

BOOL Clab1pripremaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Clab1pripremaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'G')
	{
		grid = !grid;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

// Clab1pripremaView drawing

void Clab1pripremaView::DrawGrid(CDC* pDC, int size, int cell)
{
	CPen npen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* oldPen = pDC->SelectObject(&npen);

	for (int i = 0; i < size; i += cell) 
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, size);
		pDC->MoveTo(0, i);
		pDC->LineTo(size, i);
	}
	pDC->SelectObject(oldPen);
}

void Clab1pripremaView::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	CPoint* pts = new CPoint[n];
	for (int i = 0; i < n; ++i)
	{
		double alfa = rotAngle + 2.0 * M_PI * i / n;
		pts[i].x = cx + (int)floor(r * cos(alfa) + 0.5);
		pts[i].y = cy + (int)floor(r * sin(alfa) + 0.5);
	}
	pDC->Polygon(pts, n);
	delete[] pts;
}

void Clab1pripremaView::DrawPolygon(CDC* pDC, CPoint* points, int n)
{
	pDC->Polygon(points, n);
}

CPoint Clab1pripremaView::Tacka(float nx, float ny)
{
	CPoint X;
	X.x = (int)(TILE * nx);
	X.y = (int)(TILE * ny);
	return X;
}

void Clab1pripremaView::OnDraw(CDC* pDC)
{
	Clab1pripremaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	// siva pozadina
	CBrush bgBrush(RGB(220, 220, 220));
	CRect bgRect(0, 0, SIZE, SIZE);
	pDC->FillRect(&bgRect, &bgBrush);

	CPen penYellow(PS_SOLID, 3, RGB(255, 255, 0));
	CPen* oldPen = pDC->SelectObject(&penYellow);

	// zeleni trougao
	CBrush brGreen(RGB(0, 255, 0));
	CBrush* oldBr = pDC->SelectObject(&brGreen);
	CPoint green[3] = { Tacka(13, 1), Tacka(16, 4), Tacka(10, 4) };
	DrawPolygon(pDC, green, 3);

	// narandzasti kvadrat
	CBrush brOrange(RGB(255, 150, 0));
	pDC->SelectObject(&brOrange);
	CPoint orange[4] = { Tacka(10, 4), Tacka(13, 4), Tacka(13, 7), Tacka(10, 7)};
	DrawPolygon(pDC, orange, 4);

	// ljubicasti trougao
	CBrush brPurple(RGB(170, 0, 200));
	pDC->SelectObject(&brPurple);
	CPoint purple[3] = { Tacka(10, 7), Tacka(16, 7), Tacka(10, 13) };
	DrawPolygon(pDC, purple, 3);

	// beli straftasti trougao
	CBrush brWhite(HS_VERTICAL, RGB(0, 0, 255));
	pDC->SelectObject(&brWhite);
	int oldBk = pDC->SetBkColor(RGB(255, 255, 255));
	CPoint white[3] = { Tacka(16, 7), Tacka(18, 9), Tacka(14, 9) };
	DrawPolygon(pDC, white, 3);
	pDC->SetBkColor(oldBk);

	// zuti trougao
	CBrush brYellow(RGB(255, 255, 0));
	pDC->SelectObject(&brYellow);
	CPoint yellow[3] = { Tacka(10, 7), Tacka(10, 16), Tacka(5, 11.5) };
	DrawPolygon(pDC, yellow, 3);

	// roze trougao
	CBrush brPink(RGB(255, 180, 180));
	pDC->SelectObject(&brPink);
	CPoint pink[3] = { Tacka(10, 15), Tacka(10, 18), Tacka(13, 18) };
	DrawPolygon(pDC, pink, 3);

	// crveni paralelogram
	CBrush brRed(RGB(255, 0, 0));
	pDC->SelectObject(&brRed);
	CPoint red[4] = { Tacka(5, 11.5), Tacka(5, 14.5), Tacka(1.5, 17.5), Tacka(1.5, 14.5) };
	DrawPolygon(pDC, red, 4);

	// mnogouglovi
	CPen penThin(PS_SOLID, 2, RGB(255, 255, 0));
	pDC->SelectObject(&penThin);
	pDC->SelectStockObject(NULL_BRUSH);

	CPoint c;
	
	// osmougao u zelenom trouglu
	c = Tacka(13, 2.8f);
	DrawRegularPolygon(pDC, c.x, c.y, 16, 8, 0);

	// sestougao u ljubicastom trouglu
	c = Tacka(11.8f, 8.8f);
	DrawRegularPolygon(pDC, c.x, c.y, 20, 6, 0);

	// cetvorougao u belom trouglu
	c = Tacka(16, 8.1f);
	DrawRegularPolygon(pDC, c.x, c.y, 12, 4, 0);

	// sestougao u ljubicastom trouglu
	c = Tacka(10.9f, 17.1f);
	DrawRegularPolygon(pDC, c.x, c.y, 12, 7, 0);

	// grid
	if (grid)
		DrawGrid(pDC, SIZE, TILE);

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBr);
}


// Clab1pripremaView printing

BOOL Clab1pripremaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab1pripremaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab1pripremaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab1pripremaView diagnostics

#ifdef _DEBUG
void Clab1pripremaView::AssertValid() const
{
	CView::AssertValid();
}

void Clab1pripremaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab1pripremaDoc* Clab1pripremaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab1pripremaDoc)));
	return (Clab1pripremaDoc*)m_pDocument;
}
#endif //_DEBUG


// Clab1pripremaView message handlers
