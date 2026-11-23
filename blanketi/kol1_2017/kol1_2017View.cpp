
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

#define TO_RAD 3.14159265 / 180.0f
#define TO_DEG 180.0f / 3.14159265

// Ckol12017View

IMPLEMENT_DYNCREATE(Ckol12017View, CView)

BEGIN_MESSAGE_MAP(Ckol12017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Ckol12017View construction/destruction

Ckol12017View::Ckol12017View() noexcept
{
	telo = new DImage;
	pozadina = new DImage;
	noga1 = new DImage;
	noga2 = new DImage;
	noga3 = new DImage;

	ugaoRotPrednje = 0;
	ugaoRotZadnje = 20;
	predjeniPut = 0;
	skaliranje = 1;
	pozadinaX = 0;
	pozadinaY = 0;

	prednjeNapred = true;
	zadnjeNapred = false;

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

void Ckol12017View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	int w = pImage->Width();
	int h = pImage->Height();
	const CRect full(0, 0, w, h);

	unsigned char* pBits = pImage->GetDIBBits();
	COLORREF color = RGB(pBits[2], pBits[1], pBits[0]);

	CDC srcDC, maskDC;
	srcDC.CreateCompatibleDC(pDC);
	maskDC.CreateCompatibleDC(pDC);

	CBitmap bmp, bmpMask;
	bmp.CreateCompatibleBitmap(pDC, w, h);
	bmpMask.CreateBitmap(w, h, 1, 1, NULL);
	CBitmap* oldSrc = srcDC.SelectObject(&bmp);
	CBitmap* oldMask = maskDC.SelectObject(&bmpMask);

	pImage->Draw(&srcDC, full, full);

	srcDC.SetBkColor(RGB(255, 255, 255));
	maskDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCPAINT);

	srcDC.SelectObject(oldSrc);
	maskDC.SelectObject(oldMask);
}

void Ckol12017View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM oldxf;
	int gm = pDC->SetGraphicsMode(GM_ADVANCED);
	pDC->GetWorldTransform(&oldxf);

	CRect srcRect = rect;
	srcRect.OffsetRect(pozadinaX, pozadinaY);
	pozadina->Draw(pDC, srcRect, rect);

	Translate(pDC, predjeniPut + 100, 100, false);
	Scale(pDC, skaliranje, skaliranje, false);

	DrawImgTransparent(pDC, telo);

	DrawLeg(pDC, ugaoRotZadnje, 52, 168);
	DrawLeg(pDC, -ugaoRotZadnje, 52, 168);
	DrawLeg(pDC, ugaoRotPrednje, 270, 168);
	DrawLeg(pDC, -ugaoRotPrednje, 270, 168);

	pDC->SetWorldTransform(&oldxf);
	pDC->SetGraphicsMode(gm);
}

void Ckol12017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM oldxf;
	pDC->GetWorldTransform(&oldxf);

	Translate(pDC, 28 + dx, 28 + dy, false);
	Rotate(pDC, (float)(alpha * TO_RAD), false);
	Translate(pDC, -28, -28, false);

	DrawImgTransparent(pDC, noga1);

	Translate(pDC, 29, 29 + 138, false);
	Rotate(pDC, (float)(-alpha * TO_RAD), false);
	Translate(pDC, -29, -29, false);

	DrawImgTransparent(pDC, noga2);

	Translate(pDC, -34, noga2->Height() - 40, false);

	DrawImgTransparent(pDC, noga3);

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
	rectH = rect.Height();
	rectW = rect.Width();

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
	if (nChar == VK_RIGHT)
	{
		predjeniPut += 3;
		if (prednjeNapred)
		{
			ugaoRotPrednje += 1;
			if (ugaoRotPrednje >= 20)
				prednjeNapred = false;
		}
		else
		{
			ugaoRotPrednje -= 1;
			if (ugaoRotPrednje <= -20)
				prednjeNapred = true;
		}

		if (zadnjeNapred)
		{
			ugaoRotZadnje += 1;
			if (ugaoRotZadnje >= 20)
				zadnjeNapred = false;
		}
		else
		{
			ugaoRotZadnje -= 1;
			if (ugaoRotZadnje <= -20)
				zadnjeNapred = true;
		}
	}
	else if (nChar == VK_LEFT)
	{
		predjeniPut -= 3;
		if (prednjeNapred)
		{
			ugaoRotPrednje -= 1;
			if (ugaoRotPrednje <= -20)
				prednjeNapred = false;
		}
		else
		{
			ugaoRotPrednje += 1;
			if (ugaoRotPrednje >= 20)
				prednjeNapred = true;
		}

		if (zadnjeNapred)
		{
			ugaoRotZadnje -= 1;
			if (ugaoRotZadnje <= -20)
				zadnjeNapred = false;
		}
		else
		{
			ugaoRotZadnje += 1;
			if (ugaoRotZadnje >= 20)
				zadnjeNapred = true;
		}
	}
	else if (nChar == VK_UP)
	{
		skaliranje += 0.1;
	}
	else if (nChar == VK_DOWN)
	{
		skaliranje -= 0.1;
	}
	else if (nChar == 'A')
	{
		if (pozadinaX >= 200)
			pozadinaX -= 200;
	}
	else if (nChar == 'D')
	{
		if (pozadinaX + 200 + rectW <= pozadina->Width())
			pozadinaX += 200;
	}
	else if (nChar == 'W')
	{
		if (pozadinaY >= 200)
			pozadinaY -= 200;
	}
	else if (nChar == 'S')
	{
		if (pozadinaY + 200 + rectH <= pozadina->Height())
			pozadinaY += 200;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL Ckol12017View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
