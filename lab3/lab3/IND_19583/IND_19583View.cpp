
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
#include "DImage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 500

// CIND19583View

IMPLEMENT_DYNCREATE(CIND19583View, CView)

BEGIN_MESSAGE_MAP(CIND19583View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CIND19583View construction/destruction

CIND19583View::CIND19583View() noexcept
{
	m_pomX = 250;
	m_pomY = 100;

	for (int i = 1; i <= 9; i++)
	{
		CString ime;
		ime.Format(_T("%d.bmp"), i);
		m_slike[i].Load(ime);
	}
}

CIND19583View::~CIND19583View()
{
}

BOOL CIND19583View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CIND19583View drawing

void CIND19583View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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

void CIND19583View::Rotate(CDC* pDC, float ugao, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = cos(ugao);
	mat.eM12 = sin(ugao);
	mat.eM21 = -sin(ugao);
	mat.eM22 = cos(ugao);
	mat.eDx = 0;
	mat.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CIND19583View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void CIND19583View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
{
	XFORM mat;
	mat.eM11 = mx ? (FLOAT)(-1.0) : (FLOAT)(1.0);
	mat.eM12 = (FLOAT)0.0;
	mat.eM21 = (FLOAT)0.0;
	mat.eM22 = my ? (FLOAT)(-1.0) : (FLOAT)(1.0);
	mat.eDx = (FLOAT)0.0;
	mat.eDy = (FLOAT)0.0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&mat, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&mat, MWT_LEFTMULTIPLY);
}

void CIND19583View::NacrtajMrezu(CDC* pDC)
{
	CPen nPen(PS_SOLID, 1, RGB(210, 210, 210));

	CPen* oldPen = pDC->SelectObject(&nPen);

	for (int i = 0; i <= SIZE; i += KOCKICA) {
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, SIZE);

		pDC->MoveTo(0, i);
		pDC->LineTo(SIZE, i);
	}

	pDC->SelectObject(oldPen);
}

void CIND19583View::FilterBitaMapa(CBitmap* map, int kanal)
{
	BITMAP bmp;
	map->GetBitmap(&bmp);

	int velicina = bmp.bmHeight * bmp.bmWidthBytes;
	unsigned char* bitovi = new unsigned char[velicina];

	map->GetBitmapBits(velicina, bitovi);

	for (int i = 0; i < velicina; i += 4)
	{
		int vrednost = 64 + (bitovi[i] + bitovi[i + 1] + bitovi[i + 2]) / 3;

		if (vrednost > 255)
			vrednost = 255;

		switch (kanal)
		{
		case 1:
			bitovi[i] = bitovi[i + 1] = 0;
			bitovi[i + 2] = vrednost;
			break;

		case 2:
			bitovi[i] = bitovi[i + 2] = 0;
			bitovi[i + 1] = vrednost;
			break;

		case 3:
			bitovi[i + 1] = bitovi[i + 2] = 0;
			bitovi[i] = vrednost;
			break;

		default:
			bitovi[i] = bitovi[i + 1] = bitovi[i + 2] = vrednost;
		}
	}

	map->SetBitmapBits(velicina, bitovi);
	delete[] bitovi;
}

void CIND19583View::DrawPuzzlePiece(CDC* pDC, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my, int filter)
{
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	this->Rotate(pDC, angle, true);
	this->Mirror(pDC, mx, my, true);
	this->Scale(pDC, sx, sy, true);
	this->Translate(pDC, x, y, true);

	this->NacrtajDeoSlagalice(pDC, img, filter);
}


void CIND19583View::NacrtajDeoSlagalice(CDC* pDC, DImage& Dslika, int filter)
{
	CBitmap* maska = new CBitmap();
	CBitmap* slika = new CBitmap();

	maska->CreateBitmap(Dslika.Width(), Dslika.Height(), 1, 1, NULL);
	slika->CreateBitmap(Dslika.Width(), Dslika.Height(), 1, 32, Dslika.GetDIBBits());

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();

	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* oldSrcBmp = srcDC->SelectObject(slika);
	CBitmap* oldDstBmp = dstDC->SelectObject(maska);

	COLORREF bgColor = srcDC->GetPixel(0, 0);
	COLORREF oldSrcBgColor = srcDC->SetBkColor(bgColor);

	dstDC->BitBlt(0, 0, Dslika.Width(), Dslika.Height(), srcDC, 0, 0, SRCCOPY);

	COLORREF oldSrcFrontColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));

	this->FilterBitaMapa(slika, filter);

	srcDC->BitBlt(0, 0, Dslika.Width(), Dslika.Height(), dstDC, 0, 0, SRCAND);

	srcDC->SetBkColor(oldSrcBgColor);
	srcDC->SetTextColor(oldSrcFrontColor);

	srcDC->SelectObject(oldSrcBmp);
	dstDC->SelectObject(oldDstBmp);

	srcDC->DeleteDC();
	dstDC->DeleteDC();

	delete srcDC;
	delete dstDC;

	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);

	CBitmap* oldMemBmp = MemDC->SelectObject(maska);
	pDC->BitBlt(-Dslika.Width() / 2, -Dslika.Height() / 2, Dslika.Width(), Dslika.Height(), MemDC, 0, 0, SRCAND);

	MemDC->SelectObject(slika);
	pDC->BitBlt(-Dslika.Width() / 2, -Dslika.Height() / 2, Dslika.Width(), Dslika.Height(), MemDC, 0, 0, SRCPAINT);

	MemDC->SelectObject(oldMemBmp);
	MemDC->DeleteDC();

	delete MemDC;

	maska->DeleteObject();
	delete maska;

	slika->DeleteObject();
	delete slika;
}

void CIND19583View::OnDraw(CDC* pDC)
{
	CIND19583Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);

	CBitmap* slikaMem = new CBitmap();
	slikaMem->CreateCompatibleBitmap(pDC, SIZE + 1, SIZE + 1);
	CBitmap* oldMemBmp = MemDC->SelectObject(slikaMem);

	CBrush brush(RGB(255, 255, 255));
	MemDC->FillRect(CRect(0, 0, SIZE + 1, SIZE + 1), &brush);

	MemDC->SetGraphicsMode(GM_ADVANCED);
	this->NacrtajMrezu(MemDC);

	const float RAD = 3.14 / 180;

	// gore levo
	this->DrawPuzzlePiece(MemDC, m_slike[8], 106, 94, -70.5 * RAD, 1, 1, true, false, 0);

	// gore sredina
	this->DrawPuzzlePiece(MemDC, m_slike[4], 243, 99, 55 * RAD, 1, 1, true, false, 0);

	// gore desno
	this->DrawPuzzlePiece(MemDC, m_slike[9], 406, 95, -80.3 * RAD, 1, 1, true, false, 0);

	// sredina levo
	this->DrawPuzzlePiece(MemDC, m_slike[5], 108, 250, 239 * RAD, 1, 1, true, false, 0);

	// sredina
	this->DrawPuzzlePiece(MemDC, m_slike[7], 260, 248, -102 * RAD, 1, 1, true, false, 0);

	// dole levo
	this->DrawPuzzlePiece(MemDC, m_slike[6], 99, 391, -9 * RAD, 1, 1, true, false, 0);

	// dole sredina
	this->DrawPuzzlePiece(MemDC, m_slike[3], 256, 397, -93 * RAD, 1, 1, true, false, 0);

	// dole desno
	this->DrawPuzzlePiece(MemDC, m_slike[1], 406, 396, -83 * RAD, 1, 1, true, false, 0);

	// sredina desno
	this->DrawPuzzlePiece(MemDC, m_slike[2], 406 - m_pomX, 254 + m_pomY, -144 * RAD, 1, 1, true, false, 1);

	// kopiranje na ekran
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	pDC->BitBlt(START_X, START_Y, SIZE + 1, SIZE + 1, MemDC, 0, 0, SRCCOPY);

	MemDC->SelectObject(oldMemBmp);
	slikaMem->DeleteObject();
	delete slikaMem;
	MemDC->DeleteDC();
	delete MemDC;
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

BOOL CIND19583View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;   // anti-flicker
}

void CIND19583View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const float korak = 5.0;

	switch (nChar)
	{
	case VK_LEFT:
		m_pomX += korak;
		break;
	case VK_RIGHT:
		m_pomX -= korak;
		break;
	case VK_UP:
		m_pomY -= korak;
		break;
	case VK_DOWN:
		m_pomY += korak;
		break;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}