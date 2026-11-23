
// kol1_2022View.cpp : implementation of the Ckol12022View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "kol1_2022.h"
#endif

#include "kol1_2022Doc.h"
#include "kol1_2022View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TO_RAD 3.14 / 180

// Ckol12022View

IMPLEMENT_DYNCREATE(Ckol12022View, CView)

BEGIN_MESSAGE_MAP(Ckol12022View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Ckol12022View construction/destruction

Ckol12022View::Ckol12022View() noexcept
{
	base = new DImage;
	arm1 = new DImage;
	arm2 = new DImage;
	head = new DImage;
	pozadina = new DImage;
	sbase = new DImage;
	sarm1 = new DImage;
	sarm2 = new DImage;
	shead = new DImage;

	base->Load(CString("base.png"));
	arm1->Load(CString("arm1.png"));
	arm2->Load(CString("arm2.png"));
	head->Load(CString("head.png"));
	pozadina->Load(CString("pozadina.jpg"));
	sbase->Load(CString("base_shadow.png"));
	sarm1->Load(CString("arm1_shadow.png"));
	sarm2->Load(CString("arm2_shadow.png"));
	shead->Load(CString("head_shadow.png"));
}

Ckol12022View::~Ckol12022View()
{
	if (base) delete base;
	if (arm1) delete arm1;
	if (arm2) delete arm2;
	if (head) delete head;
	if (pozadina) delete pozadina;
	if (sbase) delete sbase;
	if (sarm1) delete sarm1;
	if (sarm2) delete sarm2;
	if (shead) delete shead;
}

BOOL Ckol12022View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Ckol12022View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM xf;
	xf.eM11 = 1;
	xf.eM12 = 0;
	xf.eM21 = 0;
	xf.eM22 = 1;
	xf.eDx = dX;
	xf.eDy = dY;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&xf, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&xf, MWT_LEFTMULTIPLY);
}

void Ckol12022View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	XFORM xf;
	xf.eM11 = cos(angle);
	xf.eM12 = sin(angle);
	xf.eM21 = -sin(angle);
	xf.eM22 = cos(angle);
	xf.eDx = 0;
	xf.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&xf, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&xf, MWT_LEFTMULTIPLY);
}

void Ckol12022View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM xf;
	xf.eM11 = sX;
	xf.eM12 = 0;
	xf.eM21 = 0;
	xf.eM22 = sY;
	xf.eDx = 0;
	xf.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&xf, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&xf, MWT_LEFTMULTIPLY);
}

// Ckol12022View drawing

void Ckol12022View::DrawBackground(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	int x = (rect.Width() - pozadina->Width()) / 2;
	int y = rect.Height() - pozadina->Height();
	pozadina->Draw(pDC,
		CRect(0, 0, pozadina->Width(), pozadina->Height()),
		CRect(x, y, x + pozadina->Width(), y + pozadina->Height()));
}

void Ckol12022View::DrawImgTransparent(CDC* pDC, DImage* pImage)
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

	srcDC.SetBkColor(color);
	maskDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCPAINT);

	srcDC.SelectObject(oldSrc);
	maskDC.SelectObject(oldMask);
}

void Ckol12022View::DrawLampBase(CDC* pDC, bool bIsShadow)
{
	if (bIsShadow)
		DrawImgTransparent(pDC, sbase);
	else
		DrawImgTransparent(pDC, base);
	Translate(pDC, base->Width() / 2, -50, false);
}

void Ckol12022View::DrawLampArm1(CDC* pDC, bool bIsShadow)
{
	Translate(pDC, 58, 61, false);
	Rotate(pDC, ugaoArm1 * TO_RAD, false);
	Translate(pDC, -58, -61, false);

	if (bIsShadow)
		DrawImgTransparent(pDC, sarm1);
	else
		DrawImgTransparent(pDC, arm1);
}

void Ckol12022View::DrawLampArm2(CDC* pDC, bool bIsShadow)
{
	Rotate(pDC, ugaoArm2 * TO_RAD, false);
	Translate(pDC, -36, -40, false);

	if (bIsShadow)
		DrawImgTransparent(pDC, sarm2);
	else
		DrawImgTransparent(pDC, arm2);
}

void Ckol12022View::DrawLampHead(CDC* pDC, bool bIsShadow)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);
	Rotate(pDC, ugaoArm2 * TO_RAD, false);
	Translate(pDC, 272 - 36, 40 - 40, false);
	Rotate(pDC, ugaoHead * TO_RAD, false);
	Translate(pDC, -178, -100, false);

	if (bIsShadow)
		DrawImgTransparent(pDC, shead);
	else
		DrawImgTransparent(pDC, head);
	pDC->SetWorldTransform(&xf);
}

void Ckol12022View::DrawLamp(CDC* pDC, bool bIsShadow)
{
	DrawLampBase(pDC, bIsShadow);
	DrawLampArm1(pDC, bIsShadow);
	Translate(pDC, 309, 61, false);
	DrawLampHead(pDC, bIsShadow);
	DrawLampArm2(pDC, bIsShadow);
}

void Ckol12022View::DrawLampShadow(CDC* pDC)
{
	XFORM xf;
	pDC->GetWorldTransform(&xf);
	Translate(pDC, base->Width() / 2 - 45, base->Height() / 2 + 40, false);
	Scale(pDC, 1, 0.25, false);
	Rotate(pDC, -90, false);
	DrawLamp(pDC, true);
	pDC->SetWorldTransform(&xf);
}

void Ckol12022View::OnDraw(CDC* pDC)
{
	Ckol12022Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	int gm = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM xf;
	memDC.GetWorldTransform(&xf);

	DrawBackground(&memDC);
	Translate(&memDC, rect.Width() / 2, rect.Height() - 250, false);
	DrawLampShadow(&memDC);
	DrawLamp(&memDC, false);

	memDC.SetWorldTransform(&xf);
	memDC.SetGraphicsMode(gm);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
}


// Ckol12022View printing

BOOL Ckol12022View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Ckol12022View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Ckol12022View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Ckol12022View diagnostics

#ifdef _DEBUG
void Ckol12022View::AssertValid() const
{
	CView::AssertValid();
}

void Ckol12022View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ckol12022Doc* Ckol12022View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ckol12022Doc)));
	return (Ckol12022Doc*)m_pDocument;
}
#endif //_DEBUG


// Ckol12022View message handlers

BOOL Ckol12022View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void Ckol12022View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float step = 10;
	if (nChar == '1')
	{
		ugaoArm1 -= step;
	}
	else if (nChar == '2')
	{
		ugaoArm1 += step;
	}
	else if (nChar == '3')
	{
		ugaoArm2 -= step;
	}
	else if (nChar == '4')
	{
		ugaoArm2 += step;
	}
	else if (nChar == '5')
	{
		ugaoHead -= step;
	}
	else if (nChar == '6')
	{
		ugaoHead += step;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
