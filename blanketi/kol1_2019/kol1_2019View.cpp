
// kol1_2019View.cpp : implementation of the Ckol12019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "kol1_2019.h"
#endif

#include "kol1_2019Doc.h"
#include "kol1_2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TO_RAD 3.14 / 180

// Ckol12019View

IMPLEMENT_DYNCREATE(Ckol12019View, CView)

BEGIN_MESSAGE_MAP(Ckol12019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Ckol12019View construction/destruction

Ckol12019View::Ckol12019View() noexcept
{
	body = new DImage;
	arm1 = new DImage;
	arm2 = new DImage;
	leg1 = new DImage;
	leg2 = new DImage;
	background = new DImage;

	body->Load(CString("body1.png"));
	arm1->Load(CString("arm1.png"));
	arm2->Load(CString("arm2.png"));
	leg1->Load(CString("leg1.png"));
	leg2->Load(CString("leg2.png"));
	background->Load(CString("background.jpg"));
}

Ckol12019View::~Ckol12019View()
{
	if (body) delete body;
	if (arm1) delete arm1;
	if (arm2) delete arm2;
	if (leg1) delete leg1;
	if (leg2) delete leg2;
	if (background) delete background;
}

BOOL Ckol12019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Ckol12019View drawing

void Ckol12019View::DrawBackground(CDC* pDC, CRect rc)
{
	background->Draw(pDC,
		CRect(0, 0, background->Width(), background->Height()),
		CRect(rc.left, rc.top, rc.left + background->Width(), rc.top + background->Height()));
}

void Ckol12019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
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

void Ckol12019View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void Ckol12019View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void Ckol12019View::DrawArm1(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, 34, 31, false);
	Rotate(pDC, ugaoArm1 * TO_RAD, false);
	Translate(pDC, -34, -31, false);
	DrawImgTransparent(pDC, arm1);

	pDC->SetWorldTransform(&old);
}

void Ckol12019View::DrawArm2(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, 34, 31, false);
	Rotate(pDC, ugaoArm1 * TO_RAD, false);
	Translate(pDC, -34, -31, false);

	Translate(pDC, 210, 102, false);
	Rotate(pDC, ugaoArm2 * TO_RAD, false);
	Translate(pDC, -23, -61, false);

	DrawImgTransparent(pDC, arm2);

	pDC->SetWorldTransform(&old);
}

void Ckol12019View::DrawLeg1(CDC* pDC)
{
	Translate(pDC, 30, 125, false);
	Rotate(pDC, ugaoLeg1 * TO_RAD, false);
	Translate(pDC, -30, -125, false);
	DrawImgTransparent(pDC, leg1);
}

void Ckol12019View::DrawLeg2(CDC* pDC)
{
	XFORM old;
	pDC->GetWorldTransform(&old);

	Translate(pDC, 35, 60, false);
	Rotate(pDC, ugaoLeg2 * TO_RAD, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, leg2);

	pDC->SetWorldTransform(&old);
}

void Ckol12019View::DrawBody1(CDC* pDC)
{
	Translate(pDC, 26, 133, false);
	Rotate(pDC, ugaoBody * TO_RAD, false);
	Translate(pDC, -26, -133, false);
	DrawImgTransparent(pDC, body);
}

void Ckol12019View::DrawTransformer(CDC* pDC)
{
	DrawLeg2(pDC);
	Translate(pDC, 5, -67, false);
	DrawLeg1(pDC);
	Translate(pDC, 214, -4, false);
	DrawBody1(pDC);
	Translate(pDC, 178, 54, false);
	DrawArm2(pDC);
	DrawArm1(pDC);
}

void Ckol12019View::OnDraw(CDC* pDC)
{
	Ckol12019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldBmp = memDC.SelectObject(&bmp);

	int gm = memDC.SetGraphicsMode(GM_ADVANCED);
	XFORM oldxf;
	memDC.GetWorldTransform(&oldxf);

	DrawBackground(&memDC, rect);
	Translate(&memDC, 200, background->Height() - 500, true);
	DrawTransformer(&memDC);

	memDC.SetWorldTransform(&oldxf);
	memDC.SetGraphicsMode(gm);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldBmp);
}


// Ckol12019View printing

BOOL Ckol12019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Ckol12019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Ckol12019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Ckol12019View diagnostics

#ifdef _DEBUG
void Ckol12019View::AssertValid() const
{
	CView::AssertValid();
}

void Ckol12019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ckol12019Doc* Ckol12019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ckol12019Doc)));
	return (Ckol12019Doc*)m_pDocument;
}
#endif //_DEBUG


// Ckol12019View message handlers

BOOL Ckol12019View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void Ckol12019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float step = 5;
	if (nChar == 'Q')
	{
		ugaoArm1 += step;
	}
	else if (nChar == 'A')
	{
		ugaoArm1 -= step;
	}
	else if (nChar == 'T')
	{
		ugaoArm2 += step;
	}
	else if (nChar == 'G')
	{
		ugaoArm2 -= step;
	}
	else if (nChar == 'E')
	{
		ugaoLeg1 += step;
	}
	else if (nChar == 'D')
	{
		ugaoLeg1 -= step;
	}
	else if (nChar == 'R')
	{
		ugaoLeg2 += step;
	}
	else if (nChar == 'F')
	{
		ugaoLeg2 -= step;
	}
	else if (nChar == 'W')
	{
		ugaoBody += step;
	}
	else if (nChar == 'S')
	{
		ugaoBody -= step;
	}
	else if (nChar == VK_RIGHT)
	{
		ugaoArm1 += step * 0.9;
		ugaoArm2 += step;
		ugaoLeg1 -= step;
		ugaoLeg2 += step;
		ugaoBody += step * 0.1;
	}
	else if (nChar == VK_LEFT)
	{
		ugaoArm1 -= step * 0.9;
		ugaoArm2 -= step;
		ugaoLeg1 += step;
		ugaoLeg2 -= step;
		ugaoBody -= step * 0.1;
	}
	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
