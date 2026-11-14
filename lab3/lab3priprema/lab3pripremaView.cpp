
// lab3pripremaView.cpp : implementation of the Clab3pripremaView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "lab3priprema.h"
#endif

#include "lab3pripremaDoc.h"
#include "lab3pripremaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <cmath>
#include <cstring>
#include "DImage.h"

#define START_X 10
#define START_Y 10
#define KOCKICA 25
#define SIZE 500

// Clab3pripremaView

IMPLEMENT_DYNCREATE(Clab3pripremaView, CView)

BEGIN_MESSAGE_MAP(Clab3pripremaView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Clab3pripremaView construction/destruction

Clab3pripremaView::Clab3pripremaView() noexcept
{
	m_pomX = 180.0f;
	m_pomY = -250.0f;

	for (int i = 1; i <= 9; i++)
	{
		CString ime;
		ime.Format(_T("%d.bmp"), i);
		m_slike[i].Load(ime);
	}
}

Clab3pripremaView::~Clab3pripremaView()
{
}

BOOL Clab3pripremaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Clab3pripremaView drawing


void Clab3pripremaView::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM transMatrix;
	transMatrix.eM11 = sX;
	transMatrix.eM12 = 0;
	transMatrix.eM21 = 0;
	transMatrix.eM22 = sY;
	transMatrix.eDx = 0;
	transMatrix.eDy = 0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&transMatrix, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&transMatrix, MWT_LEFTMULTIPLY);
}

void Clab3pripremaView::Rotate(CDC* pDC, float ugao, bool rightMultiply)
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

void Clab3pripremaView::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void Clab3pripremaView::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply)
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


void Clab3pripremaView::NacrtajMrezu(CDC* pDC)
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

void Clab3pripremaView::FilterBitaMapa(CBitmap* map, int kanal)
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

void Clab3pripremaView::DrawPuzzlePiece(CDC* pDC, DImage& img, float x, float y, float angle, float sx, float sy, bool mx, bool my, int filter)
{
	pDC->ModifyWorldTransform(NULL, MWT_IDENTITY);

	this->Rotate(pDC, angle, true);
	this->Mirror(pDC, mx, my, true);
	this->Scale(pDC, sx, sy, true);
	this->Translate(pDC, x, y, true);

	this->NacrtajDeoSlagalice(pDC, img, filter);
}


void Clab3pripremaView::NacrtajDeoSlagalice(CDC* pDC, DImage& Dslika, int filter)
{
	// pravimo 2 bitmape, jedna sa 32bpp i jedna sa 1bpp
	CBitmap* maska = new CBitmap();
	CBitmap* slika = new CBitmap();

	maska->CreateBitmap(Dslika.Width(), Dslika.Height(), 1, 1, NULL);
	slika->CreateBitmap(Dslika.Width(), Dslika.Height(), 1, 32, Dslika.GetDIBBits());

	// dodatne DC za masku i sliku
	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();

	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	// selektujemo bitmape u DC
	CBitmap* oldSrcBmp = srcDC->SelectObject(slika);
	CBitmap* oldDstBmp = dstDC->SelectObject(maska);

	// odredjujemo boju pozadine
	COLORREF bgColor = srcDC->GetPixel(0, 0);
	COLORREF oldSrcBgColor = srcDC->SetBkColor(bgColor);

	// pravimo masku
	dstDC->BitBlt(0, 0, Dslika.Width(), Dslika.Height(), srcDC, 0, 0, SRCCOPY);

	// pripremamo srcDC za AND operaciju
	COLORREF oldSrcFrontColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));

	this->FilterBitaMapa(slika, filter);

	// uklanjamo pozadinu slike pomocu maske
	srcDC->BitBlt(0, 0, Dslika.Width(), Dslika.Height(), dstDC, 0, 0, SRCAND);

	// vracamo stare vrednosti i cistimo DC
	srcDC->SetBkColor(oldSrcBgColor);
	srcDC->SetTextColor(oldSrcFrontColor);

	srcDC->SelectObject(oldSrcBmp);
	dstDC->SelectObject(oldDstBmp);

	srcDC->DeleteDC();
	dstDC->DeleteDC();

	delete srcDC;
	delete dstDC;

	// DC za crtanje na glavnom pDC
	CDC* MemDC = new CDC();
	MemDC->CreateCompatibleDC(pDC);

	// crtamo masku
	CBitmap* oldMemBmp = MemDC->SelectObject(maska);
	pDC->BitBlt(-Dslika.Width() / 2, -Dslika.Height() / 2,
		Dslika.Width(), Dslika.Height(), MemDC, 0, 0, SRCAND);

	// crtamo sliku preko maske
	MemDC->SelectObject(slika);
	pDC->BitBlt(-Dslika.Width() / 2, -Dslika.Height() / 2,
		Dslika.Width(), Dslika.Height(), MemDC, 0, 0, SRCPAINT);

	// vracamo sve i cistimo
	MemDC->SelectObject(oldMemBmp);
	MemDC->DeleteDC();

	delete MemDC;

	maska->DeleteObject();
	delete maska;

	slika->DeleteObject();
	delete slika;
}


void Clab3pripremaView::OnDraw(CDC* pDC)
{
	Clab3pripremaDoc* pDoc = GetDocument();
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
	this->DrawPuzzlePiece(MemDC, m_slike[1], 106, 104, 205 * RAD, 1, 1, true, false, 0);

	// gore sredina
	this->DrawPuzzlePiece(MemDC, m_slike[7], 259, 100, 235 * RAD, 1, 1, true, false, 0);

	// gore desno
	this->DrawPuzzlePiece(MemDC, m_slike[5], 409, 93, 284 * RAD, 1, 1, true, false, 0);

	// sredina levo
	this->DrawPuzzlePiece(MemDC, m_slike[9], 93, 254, 107 * RAD, 1, 1, true, false, 0);

	// sredina
	this->DrawPuzzlePiece(MemDC, m_slike[2], 243, 247, 58 * RAD, 1, 1, true, false, 0);

	// sredina desno
	this->DrawPuzzlePiece(MemDC, m_slike[3], 393, 249, 72 * RAD, 1, 1, true, false, 0);

	// dole levo
	this->DrawPuzzlePiece(MemDC, m_slike[4], 95, 393, 18.5 * RAD, 1, 1, true, false, 0);

	// dole desno
	this->DrawPuzzlePiece(MemDC, m_slike[8], 402, 409, 160.5 * RAD, 1, 1, true, false, 0);

	// dole sredina - plavi deo
	this->DrawPuzzlePiece(MemDC, m_slike[6], 200 + m_pomX, 406 + m_pomY, 113 * RAD, 1, 1, true, false, 3);

	// kopiranje na ekran
	MemDC->ModifyWorldTransform(NULL, MWT_IDENTITY);
	pDC->BitBlt(START_X, START_Y, SIZE + 1, SIZE + 1, MemDC, 0, 0, SRCCOPY);

	MemDC->SelectObject(oldMemBmp);
	slikaMem->DeleteObject();
	delete slikaMem;
	MemDC->DeleteDC();
	delete MemDC;
}


// Clab3pripremaView printing

BOOL Clab3pripremaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Clab3pripremaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Clab3pripremaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Clab3pripremaView diagnostics

#ifdef _DEBUG
void Clab3pripremaView::AssertValid() const
{
	CView::AssertValid();
}

void Clab3pripremaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Clab3pripremaDoc* Clab3pripremaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Clab3pripremaDoc)));
	return (Clab3pripremaDoc*)m_pDocument;
}
#endif //_DEBUG


// Clab3pripremaView message handlers


BOOL Clab3pripremaView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;   // anti-flicker
}

void Clab3pripremaView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const float korak = 5.0;

	switch (nChar)
	{
		case VK_LEFT:  
			m_pomX -= korak; 
			break;
		case VK_RIGHT: 
			m_pomX += korak;
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