
// IND_19583View.cpp : implementation of the CIND19583View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "IND_19583.h"
#endif

#include "IND_19583Doc.h"
#include "IND_19583View.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const float PI = 3.14;

#define KOCKICA 25
#define DEO_SIRINA 50
#define DEO_VISINA 75

float Kockica(float n) 
{ 
	return n * KOCKICA; 
}

float Rad(float deg)
{
	return deg * PI / 180.0;
}

// CIND19583View

IMPLEMENT_DYNCREATE(CIND19583View, CView)

BEGIN_MESSAGE_MAP(CIND19583View, CView)
	// Standard printing commands
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CIND19583View construction/destruction

CIND19583View::CIND19583View() noexcept
{
	grid = true;
	m_angle1 = 0;
	m_angle2 = 0;

	m_hCactus = GetEnhMetaFile(_T("cactus_part.emf"));
	m_hCactusLight = GetEnhMetaFile(_T("cactus_part_light.emf"));
}

CIND19583View::~CIND19583View()
{
	if (m_hCactus) 
		DeleteEnhMetaFile(m_hCactus);
	if (m_hCactusLight) 
		DeleteEnhMetaFile(m_hCactusLight);
}

BOOL CIND19583View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CIND19583View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const float step = Rad(30);
	switch (nChar)
	{
		case 'G':      
			grid = !grid;        
			break;
		case VK_LEFT:  
			m_angle1 -= step;    
			break;
		case VK_RIGHT: 
			m_angle1 += step;    
			break;
		case 'A':      
			m_angle2 -= step;   
			break;
		case 'D':    
			m_angle2 += step;   
			break;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CIND19583View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CIND19583View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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

void CIND19583View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

// CIND19583View drawing

void CIND19583View::DrawGrid(CDC* pDC, int size, int cell)
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

void CIND19583View::DrawPartAt(CDC* pDC, float x, float y, float angle, float sx, float sy, bool light)
{
	HENHMETAFILE h;
	if (light)
		h = m_hCactusLight;
	else
		h = m_hCactus;

	if (!h) 
		return;

	XFORM saved;
	pDC->GetWorldTransform(&saved);

	Translate(pDC, Kockica(x), Kockica(y), false);
	Rotate(pDC, angle, false);
	Scale(pDC, sx, sy, false);

	CRect rc(-DEO_SIRINA / 2, -DEO_VISINA, DEO_SIRINA / 2, 0);
	PlayEnhMetaFile(pDC->m_hDC, h, &rc);

	pDC->SetWorldTransform(&saved);
}

void CIND19583View::DrawJoint(CDC* pDC, int r)
{
	CBrush br(RGB(34, 180, 34));
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* pOldBr = pDC->SelectObject(&br);
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->Ellipse(-r, -r, r, r);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBr);
}

void CIND19583View::DrawJointAt(CDC* pDC, float x, float y)
{
	XFORM saved;
	pDC->GetWorldTransform(&saved);
	Translate(pDC, Kockica(x), Kockica(y), false);
	DrawJoint(pDC, 11);
	pDC->SetWorldTransform(&saved);
}

void CIND19583View::DrawPot(CDC* pDC)
{
	CBrush br(RGB(220, 150, 0));
	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* pOldBr = pDC->SelectObject(&br);
	CPen* pOldPen = pDC->SelectObject(&pen);

	POINT telo[4] = {
		{ (int)Kockica(-2), (int)Kockica(1) }, { (int)Kockica(2), (int)Kockica(1) },
		{ (int)Kockica(1.5),  (int)Kockica(3) }, { (int)Kockica(-1.5), (int)Kockica(3) } 
	};
	pDC->Polygon(telo, 4);

	pDC->Rectangle((int)Kockica(-2.5), (int)Kockica(0), (int)Kockica(2.5), (int)Kockica(1));

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBr);
}

void CIND19583View::DrawFigure(CDC* pDC)
{
	XFORM start, frame;
	pDC->GetWorldTransform(&start);

	Translate(pDC, Kockica(10), Kockica(3), false);
	Rotate(pDC, PI, false);
	pDC->GetWorldTransform(&frame);

	const float d = 2.15;

	Rotate(pDC, m_angle1, false);
	DrawPartAt(pDC, 0, 0, 0, 1.2, 1, true);

	DrawPartAt(pDC, 0, -3, 0, 0.5, 1, false);
	DrawPartAt(pDC, 0, -3, - PI / 4, 0.5, 1, false);
	DrawPartAt(pDC, 0, -3, PI / 4, 0.5, 1, false);

	DrawPartAt(pDC, -d, -3 - d, m_angle2 - PI / 4, 1.2, 1, true);

	DrawPartAt(pDC, d, -3 - d, 0, 0.5, 1, false);
	DrawPartAt(pDC, d, -3 - d, PI / 4, 0.5, 1, false);
	DrawPartAt(pDC, d, -3 - d, PI / 2, 0.5, 1, false);

	DrawPartAt(pDC, 2 * d, -5.1 - d, 0, 0.5, 1, false);
	DrawPartAt(pDC, 2 * d, -5.1 - d, PI / 4, 0.5, 1, false);
	DrawPartAt(pDC, 2 * d, -5.1 - d, PI / 2, 0.5, 1, false);

	DrawJointAt(pDC, 0, 0);
	DrawJointAt(pDC, 0, -3);
	DrawJointAt(pDC, d, -3 - d);
	DrawJointAt(pDC, -d, -3 - d);
	DrawJointAt(pDC, 2 * d, -5.1 - d);

	pDC->SetWorldTransform(&frame);
	DrawPot(pDC);

	pDC->SetWorldTransform(&start);
}

void CIND19583View::OnDraw(CDC* pDC)
{
	CIND19583Doc* pDoc = GetDocument();
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
		DrawGrid(pDC, SIZE, KOCKICA);

	pDC->SetGraphicsMode(oldMode);
}


// CIND19583View printing

BOOL CIND19583View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CIND19583View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CIND19583View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CIND19583View diagnostics

#ifdef _DEBUG
void CIND19583View::AssertValid() const
{
	CView::AssertValid();
}

void CIND19583View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CIND19583Doc* CIND19583View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIND19583Doc)));
	return (CIND19583Doc*)m_pDocument;
}
#endif //_DEBUG


// CIND19583View message handlers
