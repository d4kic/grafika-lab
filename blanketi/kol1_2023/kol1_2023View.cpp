
// kol1_2023View.cpp : implementation of the Ckol12023View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "kol1_2023.h"
#endif

#include "kol1_2023Doc.h"
#include "kol1_2023View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TO_RAD 3.14 / 180

// Ckol12023View

IMPLEMENT_DYNCREATE(Ckol12023View, CView)

BEGIN_MESSAGE_MAP(Ckol12023View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Ckol12023View construction/destruction

Ckol12023View::Ckol12023View() noexcept
{
	glava = new DImage;
	nadkolenica = new DImage;
	nadlaktica = new DImage;
	potkolenica = new DImage;
	podlaktica = new DImage;
	saka = new DImage;
	stopalo = new DImage;
	telo = new DImage;
	pozadina = new DImage;

	glava->Load(CString("glava.png"));
	nadkolenica->Load(CString("nadkolenica.png"));
	nadlaktica->Load(CString("nadlaktica.png"));
	potkolenica->Load(CString("podkolenica.png"));
	podlaktica->Load(CString("podlaktica.png"));
	saka->Load(CString("saka.png"));
	stopalo->Load(CString("stopalo.png"));
	telo->Load(CString("telo.png"));
	pozadina->Load(CString("pozadina.jpg"));
}

Ckol12023View::~Ckol12023View()
{
	if (glava) delete glava;
	if (nadkolenica) delete nadkolenica;
	if (nadlaktica) delete nadlaktica;
	if (potkolenica) delete potkolenica;
	if (podlaktica) delete podlaktica;
	if (saka) delete saka;
	if (stopalo) delete stopalo;
	if (telo) delete telo;
	if (pozadina) delete pozadina;
}

BOOL Ckol12023View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void Ckol12023View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
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

void Ckol12023View::Rotate(CDC* pDC, float angle, bool rightMultiply)
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

void Ckol12023View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
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

// Ckol12023View drawing

void Ckol12023View::DrawBackground(CDC* pDC)
{
	pozadina->Draw(pDC,
		CRect(0, 0, pozadina->Width(), pozadina->Height()),
		CRect(0, 0, pozadina->Width(), pozadina->Height()));
}

void Ckol12023View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	int w = pImage->Width();
	int h = pImage->Height();
	const CRect full(0, 0, w, h);

	// providna boja = boja prvog piksela
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

	// kopija slike u srcDC
	pImage->Draw(&srcDC, full, full);

	// maska: providna boja -> 1, ostalo -> 0
	srcDC.SetBkColor(color);
	maskDC.BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCCOPY);

	// providni pikseli u slici postaju crni
	srcDC.SetTextColor(RGB(255, 255, 255));
	srcDC.SetBkColor(RGB(0, 0, 0));
	srcDC.BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);

	// na odrediste: AND sa maskom pa OR sa slikom
	pDC->BitBlt(0, 0, w, h, &maskDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, &srcDC, 0, 0, SRCPAINT);

	srcDC.SelectObject(oldSrc);
	maskDC.SelectObject(oldMask);
}

void Ckol12023View::DrawHalf(CDC* pDC)
{
	DrawImgTransparent(pDC, telo);

	XFORM xf;
	pDC->GetWorldTransform(&xf);

	Translate(pDC, 25, 65, false);
	Rotate(pDC, ugaoNadlaktica * TO_RAD, false);
	Translate(pDC, -35, -35, false);

	DrawImgTransparent(pDC, nadlaktica);

	Translate(pDC, 22, 167, false);
	Rotate(pDC, ugaoPodlaktica * TO_RAD, false);
	Translate(pDC, -30, -33, false);

	DrawImgTransparent(pDC, podlaktica);

	Translate(pDC, 30, 140, false);
	Rotate(pDC, ugaoSaka * TO_RAD, false);
	Translate(pDC, -25, -3, false);

	DrawImgTransparent(pDC, saka);

	pDC->SetWorldTransform(&xf);

	Translate(pDC, 61 - 29, 262 - 20, false);
	DrawImgTransparent(pDC, nadkolenica);

	Translate(pDC, 30 - 25, 184 - 37, false);
	DrawImgTransparent(pDC, potkolenica);

	Translate(pDC, 25 - 20, 248 - 16, false);
	DrawImgTransparent(pDC, stopalo);

	pDC->SetWorldTransform(&xf);
}

void Ckol12023View::DrawHead(CDC* pDC)
{
	DrawImgTransparent(pDC, glava);
}

void Ckol12023View::DrawRobot(CDC* pDC)
{
	DrawHalf(pDC);

	XFORM desna;
	pDC->GetWorldTransform(&desna);

	Translate(pDC, 2 * telo->Width(), 0, false);
	Scale(pDC, -1, 1, false);
	DrawHalf(pDC);

	pDC->SetWorldTransform(&desna);

	Translate(pDC, telo->Width() - glava->Width() / 2, -glava->Height() + 20, false);
	DrawHead(pDC);
}

void Ckol12023View::OnDraw(CDC* pDC)
{
	Ckol12023Doc* pDoc = GetDocument();
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
	XFORM old;
	memDC.GetWorldTransform(&old);

	DrawBackground(&memDC);
	Translate(&memDC, pozadina->Width() / 2, pozadina->Height() / 2, false);
	Rotate(&memDC, ugaoRobot * TO_RAD, false);
	Scale(&memDC, skaliranje, skaliranje, false);
	Translate(&memDC, -pozadina->Width() / 2, -pozadina->Height() / 2, false);

	Translate(&memDC, pozadina->Width() / 2 - telo->Width(), pozadina->Height() / 2 - telo->Height(), false);

	DrawRobot(&memDC);

	memDC.SetWorldTransform(&old);
	memDC.SetGraphicsMode(gm);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(oldBmp);
}


// Ckol12023View printing

BOOL Ckol12023View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Ckol12023View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Ckol12023View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Ckol12023View diagnostics

#ifdef _DEBUG
void Ckol12023View::AssertValid() const
{
	CView::AssertValid();
}

void Ckol12023View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ckol12023Doc* Ckol12023View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ckol12023Doc)));
	return (Ckol12023Doc*)m_pDocument;
}
#endif //_DEBUG


// Ckol12023View message handlers

BOOL Ckol12023View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void Ckol12023View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	float step = 5;
	if (nChar == 'A')
	{
		if (ugaoSaka <= 30)
			ugaoSaka += step;
	}
	else if (nChar == 'S')
	{
		if (ugaoSaka >= -10)
			ugaoSaka -= step;
	}
	else if (nChar == 'D')
	{
		if (ugaoPodlaktica <= 80)
			ugaoPodlaktica += step;
	}
	else if (nChar == 'F')
	{
		if (ugaoPodlaktica >= -10)
			ugaoPodlaktica -= step;
	}
	else if (nChar == 'G')
	{
		if (ugaoNadlaktica <= 90)
			ugaoNadlaktica += step;
	}
	else if (nChar == 'H')
	{
		if (ugaoNadlaktica >= -10)
			ugaoNadlaktica -= step;
	}
	else if (nChar == '1')
	{
		ugaoRobot += step;
		skaliranje += 0.1;
	}
	else if (nChar == '2')
	{
		ugaoRobot -= step;
		skaliranje -= 0.1;
	}

	Invalidate();
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
