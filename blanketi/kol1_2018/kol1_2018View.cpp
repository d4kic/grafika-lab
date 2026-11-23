
// kol1_2018View.cpp : implementation of the Ckol12018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "kol1_2018.h"
#endif

#include "kol1_2018Doc.h"
#include "kol1_2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TO_RAD 3.14 / 180

// Ckol12018View

IMPLEMENT_DYNCREATE(Ckol12018View, CView)

BEGIN_MESSAGE_MAP(Ckol12018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Ckol12018View construction/destruction

Ckol12018View::Ckol12018View() noexcept
{
	bager = new DImage;
	arm1 = new DImage;
	arm2 = new DImage;
	pozadina = new DImage;

	arm1->Load(CString("arm1.png"));
	arm2->Load(CString("arm2.png"));
	bager->Load(CString("bager.png"));
	pozadina->Load(CString("pozadina.png"));
	viljuska = GetEnhMetaFile(CString("viljuska.emf"));

	ENHMETAHEADER hdr;
	GetEnhMetaFileHeader(viljuska, sizeof(hdr), &hdr);
	viljuskaRect = CRect(hdr.rclBounds.left, hdr.rclBounds.top, hdr.rclBounds.right, hdr.rclBounds.bottom);
}

Ckol12018View::~Ckol12018View()
{
	delete arm1;
	delete arm2;
	delete bager;
	delete pozadina;
	DeleteEnhMetaFile(viljuska);
}

BOOL Ckol12018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Ckol12018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void Ckol12018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void Ckol12018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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

// Ckol12018View drawing

void Ckol12018View::DrawBackground(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	int x = (rc.Width() - pozadina->Width()) / 2;
	int y = rc.Height() - pozadina->Height();

	CRect rcSrc(0, 0, pozadina->Width(), pozadina->Height());
	CRect rcDst(x, y, x + pozadina->Width(), y + pozadina->Height());

	pozadina->Draw(pDC, rcSrc, rcDst);
}

void Ckol12018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
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

void Ckol12018View::DrawBody(CDC* pDC)
{
	DrawImgTransparent(pDC, bager);
	Translate(pDC, 5, 100, false);
}

void Ckol12018View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 58, 61, false);
	Rotate(pDC, ugaoArm1 * TO_RAD, false);
	Translate(pDC, -58, -61, false);
	DrawImgTransparent(pDC, arm1);
}

void Ckol12018View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 36, 40, false);
	Rotate(pDC, ugaoArm2 * TO_RAD, false);
	Translate(pDC, -36, -40, false);
	DrawImgTransparent(pDC, arm2);
}

void Ckol12018View::DrawFork(CDC* pDC)
{
	Translate(pDC, 14, 20, false);
	Rotate(pDC, ugaoViljuska * TO_RAD, false);
	Translate(pDC, -14, -20, false);
	Scale(pDC, 2.5, 2.5, false);
	pDC->PlayMetaFile(viljuska, viljuskaRect);
}

void Ckol12018View::DrawExcavator(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	Translate(pDC, rect.Width() - bager->Width(), rect.Height() - bager->Height(), false);
	
	DrawBody(pDC);
	DrawArm1(pDC);
	Translate(pDC, 309 - 36, 61 - 40, false);
	DrawArm2(pDC);
	Translate(pDC, 272 - 14, 40 - 20, false);
	DrawFork(pDC);
}

void Ckol12018View::OnDraw(CDC* pDC)
{
	Ckol12018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC pMemDC;
	pMemDC.CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	CBitmap* oldBmp = pMemDC.SelectObject(&bmp);

	XFORM oldxf;
	int gm = pMemDC.SetGraphicsMode(GM_ADVANCED);
	pMemDC.GetWorldTransform(&oldxf);

	DrawBackground(&pMemDC);
	Translate(&pMemDC, -dst, 0, false);
	DrawExcavator(&pMemDC);

	pMemDC.SetWorldTransform(&oldxf);
	pMemDC.SetGraphicsMode(gm);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &pMemDC, 0, 0, SRCCOPY);
	pMemDC.SelectObject(oldBmp);
}

// Ckol12018View printing

BOOL Ckol12018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Ckol12018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Ckol12018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Ckol12018View diagnostics

#ifdef _DEBUG
void Ckol12018View::AssertValid() const
{
	CView::AssertValid();
}

void Ckol12018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ckol12018Doc* Ckol12018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ckol12018Doc)));
	return (Ckol12018Doc*)m_pDocument;
}
#endif //_DEBUG


// Ckol12018View message handlers

BOOL Ckol12018View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void Ckol12018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '1')
	{
		ugaoArm1 -= 10;
	}
	else if (nChar == '2')
	{
		ugaoArm1 += 10;
	}
	else if (nChar == '3')
	{
		ugaoArm2 -= 10;
	}
	else if (nChar == '4')
	{
		ugaoArm2 += 10;
	}
	else if (nChar == '5')
	{
		ugaoViljuska -= 10;
	}
	else if (nChar == '6')
	{
		ugaoViljuska += 10;
	}
	else if (nChar == VK_LEFT)
	{
		dst += 5;
	}
	else if (nChar == VK_RIGHT)
	{
		dst -= 5;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
