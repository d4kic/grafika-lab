
// lab2pripremaView.cpp : implementation of the Clab2pripremaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab2priprema.h"
#endif

#include "lab2pripremaDoc.h"
#include "lab2pripremaView.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static const float PI = 3.14;
static inline float Rad(float deg) { return deg * PI / 180.0; }

#define KOCKICA 25
#define DEO_SIRINA 50
#define DEO_VISINA 75
static inline float Kockica(float n) { return n * KOCKICA; }

// Clab2pripremaView

IMPLEMENT_DYNCREATE(Clab2pripremaView, CView)

BEGIN_MESSAGE_MAP(Clab2pripremaView, CView)
	// Standard printing commands
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

void Clab2pripremaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const float step = Rad(5);
	switch (nChar)
	{
	case 'G':      grid = !grid;        break;
	case VK_LEFT:  m_angle1 -= step;    break;
	case VK_RIGHT: m_angle1 += step;    break;
	case 'A':      m_angle2 -= step;    break;
	case 'D':      m_angle2 += step;    break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void Clab2pripremaView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = 1.0f;
	mat.eM12 = 0.0f;
	mat.eM21 = 0.0f;
	mat.eM22 = 1.0f;
	mat.eDx = dX;
	mat.eDy = dY;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void Clab2pripremaView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = sX;
	mat.eM12 = 0.0f;
	mat.eM21 = 0.0f;
	mat.eM22 = sY;
	mat.eDx = 0.0f;
	mat.eDy = 0.0f;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void Clab2pripremaView::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = cosf(angle);
	mat.eM12 = sinf(angle);
	mat.eM21 = -sinf(angle);
	mat.eM22 = cosf(angle);
	mat.eDx = 0.0f;
	mat.eDy = 0.0f;
	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void Clab2pripremaView::DrawGrid(CDC* pDC, int size, int cell)
{
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* pOld = pDC->SelectObject(&pen);
	for (int i = 0; i <= size; i += cell)
	{
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, size);
		pDC->MoveTo(0, i);
		pDC->LineTo(size, i);
	}
	pDC->SelectObject(pOld);
}


// Clab2pripremaView construction/destruction

Clab2pripremaView::Clab2pripremaView() noexcept
{
	grid = true;
	m_angle1 = 0;
	m_angle2 = 0;

	m_hCactus = GetEnhMetaFile(_T("../cactus_part.emf"));
	m_hCactusLight = GetEnhMetaFile(_T("../cactus_part_light.emf"));
}

Clab2pripremaView::~Clab2pripremaView()
{
	if (m_hCactus) DeleteEnhMetaFile(m_hCactus);
	if (m_hCactusLight) DeleteEnhMetaFile(m_hCactusLight);
}

BOOL Clab2pripremaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Clab2pripremaView drawing

void Clab2pripremaView::DrawPartAt(CDC* pDC, float x, float y, float angle, float sx, float sy, bool light)
{
	HENHMETAFILE h = light ? m_hCactusLight : m_hCactus;
	if (!h) return;

	XFORM saved;
	pDC->GetWorldTransform(&saved);

	Translate(pDC, Kockica(x), Kockica(y), false);
	Rotate(pDC, angle, false);
	Scale(pDC, sx, sy, false);

	CRect rc(-DEO_SIRINA / 2, -DEO_VISINA, DEO_SIRINA / 2, 0);
	PlayEnhMetaFile(pDC->m_hDC, h, &rc);

	pDC->SetWorldTransform(&saved);
}

void Clab2pripremaView::DrawJoint(CDC* pDC, int r)
{
	CBrush br(RGB(34, 180, 34));
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* pOldBr = pDC->SelectObject(&br);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Ellipse(-r, -r, r, r);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBr);
}

void Clab2pripremaView::DrawJointAt(CDC* pDC, float x, float y)
{
	XFORM saved;
	pDC->GetWorldTransform(&saved);
	Translate(pDC, Kockica(x), Kockica(y), false);
	DrawJoint(pDC, 11);
	pDC->SetWorldTransform(&saved);
}

void Clab2pripremaView::DrawPot(CDC* pDC)
{
	CBrush br(RGB(220, 150, 0));
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* pOldBr = pDC->SelectObject(&br);
	CPen* pOldPen = pDC->SelectObject(&pen);

	POINT telo[4] = {
		{ (int)Kockica(-2), (int)Kockica(1) }, { (int)Kockica(2), (int)Kockica(1) },
		{ (int)Kockica(1.5f),  (int)Kockica(3) }, { (int)Kockica(-1.5f), (int)Kockica(3) } };
	pDC->Polygon(telo, 4);

	pDC->Rectangle((int)Kockica(-2.5f), (int)Kockica(0), (int)Kockica(2.5f), (int)Kockica(1));

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBr);
}

void Clab2pripremaView::DrawFigure(CDC* pDC)
{
	XFORM start, frame;
	pDC->GetWorldTransform(&start);

	Translate(pDC, Kockica(10), Kockica(17), false);
	pDC->GetWorldTransform(&frame);

	const float d = 2.15f;

	Rotate(pDC, m_angle1, false);
	DrawPartAt(pDC, 0, 0, 0, 1.2, 1, true);

	DrawPartAt(pDC, 0, -3, - PI / 4, 0.5f, 1, false);
	DrawPartAt(pDC, 0, -3, 0, 0.5f, 1, false);
	DrawPartAt(pDC, 0, -3, PI / 4, 0.5f, 1, false);

	DrawPartAt(pDC, d, -3 - d, m_angle2, 0.8f, 1, true);
	DrawPartAt(pDC, d, -3 - d, PI / 2, 0.8f, 1, false);

	DrawPartAt(pDC, 2.4 * d, -3 - d, PI / 4, 0.8f, 1, false);
	DrawPartAt(pDC, 2.4 * d, -3 - d, 3 * PI / 4, 0.8f, 1, false);

	DrawPartAt(pDC, -d, -3 - d, 0, 0.8f, 1, false);
	DrawPartAt(pDC, -d, -3 - d, - PI / 2, 0.8f, 1, false);

	DrawPartAt(pDC, -d, -6 - d, 0, 1.2, 1, false);

	DrawJointAt(pDC, 0, 0);
	DrawJointAt(pDC, 0, -3);
	DrawJointAt(pDC, d, -3-d);
	DrawJointAt(pDC, -d, -3 - d);
	DrawJointAt(pDC, 2.4 * d, -3 - d);
	DrawJointAt(pDC, -d, -6 - d);

	pDC->SetWorldTransform(&frame);
	DrawPot(pDC);

	pDC->SetWorldTransform(&start);
}


void Clab2pripremaView::OnDraw(CDC* pDC)
{
	Clab2pripremaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	const int SIZE = 500;
	int oldMode = pDC->SetGraphicsMode(GM_ADVANCED);

	CBrush sky(RGB(135, 206, 235));
	CRect rc(0, 0, SIZE, SIZE);
	pDC->FillRect(&rc, &sky);

	DrawFigure(pDC);

	if (grid)
		DrawGrid(pDC, SIZE, 25);

	pDC->SetGraphicsMode(oldMode);
}


// Clab2pripremaView printing

BOOL Clab2pripremaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab2pripremaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab2pripremaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab2pripremaView diagnostics

#ifdef _DEBUG
void Clab2pripremaView::AssertValid() const
{
	CView::AssertValid();
}

void Clab2pripremaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab2pripremaDoc* Clab2pripremaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab2pripremaDoc)));
	return (Clab2pripremaDoc*)m_pDocument;
}
#endif //_DEBUG


// Clab2pripremaView message handlers
