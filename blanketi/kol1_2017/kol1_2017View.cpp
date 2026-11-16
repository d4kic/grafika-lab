
// kol1_2017View.cpp : implementation of the Ckol12017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "kol1_2017.h"
#endif

#include "kol1_2017Doc.h"
#include "kol1_2017View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ckol12017View

IMPLEMENT_DYNCREATE(Ckol12017View, CView)

BEGIN_MESSAGE_MAP(Ckol12017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Ckol12017View construction/destruction

Ckol12017View::Ckol12017View() noexcept
{
	telo = new DImage;
	pozadina = new DImage;
	noga1 = new DImage;
	noga2 = new DImage;
	noga3 = new DImage;

	telo->Load(CString("Body.png"));
	pozadina->Load(CString("Back2.jpg"));
	noga1->Load(CString("Leg1.png"));
	noga2->Load(CString("Leg2.png"));
	noga3->Load(CString("Leg3.png"));
}

Ckol12017View::~Ckol12017View()
{
	if (telo) delete telo;
	if (pozadina) delete pozadina;
	if (noga1) delete noga1;
	if (noga2) delete noga2;
	if (noga3) delete noga3;
}

BOOL Ckol12017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Ckol12017View::LoadIdentity(CDC* pDC)
{
	XFORM mat;
	mat.eM11 = 1;
	mat.eM12 = 0;
	mat.eM21 = 0;
	mat.eM22 = 1;
	mat.eDx = 0;
	mat.eDy = 0;

	pDC->SetWorldTransform(&mat);
}

void Ckol12017View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = 1;
	mat.eM12 = 0;
	mat.eM21 = 0;
	mat.eM22 = 1;
	mat.eDx = dX;
	mat.eDy = dY;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void Ckol12017View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = cos(angle);
	mat.eM12 = sin(angle);
	mat.eM21 = -sin(angle);
	mat.eM22 = cos(angle);
	mat.eDx = 0;
	mat.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void Ckol12017View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = sX;
	mat.eM12 = 0;
	mat.eM21 = 0;
	mat.eM22 = sY;
	mat.eDx = 0;
	mat.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

// Ckol12017View drawing

void Ckol12017View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM oldxf;
	int gm = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&oldxf);

	CRect srcRect = rect;
	pozadina->Draw(pDC, srcRect, rect);

	Translate(pDC, 100, 100, false);

	telo->DrawTransparent(pDC,
		CRect(0, 0, telo->Width(), telo->Height()),
		CRect(0, 0, telo->Width(), telo->Height()),
		RGB(255, 255, 255));

	DrawLeg(pDC, 0, 52, 168);
	DrawLeg(pDC, 20, 52, 168);
	DrawLeg(pDC, 0, 270, 168);
	DrawLeg(pDC, 0, 270, 168);

	pDC->SetWorldTransform(&oldxf);
	pDC->SetGraphicsMode(gm);
}

void Ckol12017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM oldxf;
	pDC->GetWorldTransform(&oldxf);

	Translate(pDC, 28 + dx, 28 + dy, false);
	Rotate(pDC, -alpha, false);
	Translate(pDC, -28, -28, false);

	noga1->DrawTransparent(pDC,
		CRect(0, 0, noga1->Width(), noga1->Height()),
		CRect(0, 0, noga1->Width(), noga1->Height()),
		RGB(255, 255, 255));

	Translate(pDC, 29, 29 + 138, false);
	Rotate(pDC, alpha, false);
	Translate(pDC, -29, -29, false);

	noga2->DrawTransparent(pDC,
		CRect(0, 0, noga2->Width(), noga2->Height()),
		CRect(0, 0, noga2->Width(), noga2->Height()),
		RGB(255, 255, 255));

	Translate(pDC, -34, noga2->Height() - 40, false);

	noga3->DrawTransparent(pDC,
		CRect(0, 0, noga3->Width(), noga3->Height()),
		CRect(0, 0, noga3->Width(), noga3->Height()),
		RGB(255, 255, 255));

	pDC->SetWorldTransform(&oldxf);
}

void Ckol12017View::OnDraw(CDC* pDC)
{
	Ckol12017Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* pMemDC = new CDC();
	pMemDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = pMemDC->SelectObject(&bmp);

	DrawScene(pMemDC, rect);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), pMemDC, 0, 0, SRCCOPY);
	pMemDC->SelectObject(pOldBmp);
	delete pMemDC;
}


// Ckol12017View printing

BOOL Ckol12017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Ckol12017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Ckol12017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Ckol12017View diagnostics

#ifdef _DEBUG
void Ckol12017View::AssertValid() const
{
	CView::AssertValid();
}

void Ckol12017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ckol12017Doc* Ckol12017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ckol12017Doc)));
	return (Ckol12017Doc*)m_pDocument;
}
#endif //_DEBUG


// Ckol12017View message handlers

void Ckol12017View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
