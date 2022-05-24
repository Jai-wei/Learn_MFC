
// Learn_MFCView.cpp: CLearnMFCView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Learn_MFC.h"
#endif

#include "Learn_MFCDoc.h"
#include "Learn_MFCView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLearnMFCView

IMPLEMENT_DYNCREATE(CLearnMFCView, CView)

BEGIN_MESSAGE_MAP(CLearnMFCView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLearnMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CLearnMFCView 构造/析构

CLearnMFCView::CLearnMFCView() noexcept
{
	// TODO: 在此处添加构造代码
	// 初始化用于存储输入字符的变量
	m_strLine = "";
	m_ptOrigin = 0;
	m_nWidth = 0;
}

CLearnMFCView::~CLearnMFCView()
{
}

BOOL CLearnMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CLearnMFCView 绘图

void CLearnMFCView::OnDraw(CDC* pDC)
{
	CLearnMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 定义并输入字符串
	CString str;
	str = "test";
	pDC->TextOutW(50, 50, str);

	//定义好字符串资源后
	str.LoadStringW(IDS_TEST);
	pDC->TextOutW(100, 100, str);

	// 获取字符串高度和宽度（注意必须放在改变字符串之前）
	CSize sz = pDC->GetTextExtent(str);

	//定义好字符串资源后
	str.LoadStringW(IDS_TEST);
	pDC->TextOutW(100, 100, str);

	//创建路径层,通过剪切区域实现不同效果
	pDC->BeginPath();
	// 创建矩形
	pDC->Rectangle(50, 50, 50 + sz.cx, 50 + sz.cy);
	pDC->EndPath();
	// 设置路径层与其他层的操作关系
	pDC->SelectClipPath(RGN_DIFF);

	// 创建方格，用于演示路径层作用
	for (int i = 0; i < 300; i += 10) {
		pDC->MoveTo(0,i);
		pDC->LineTo(300, i);
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 300);
	}

	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CLearnMFCView 打印


void CLearnMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLearnMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLearnMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CLearnMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CLearnMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLearnMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLearnMFCView 诊断

#ifdef _DEBUG
void CLearnMFCView::AssertValid() const
{
	CView::AssertValid();
}

void CLearnMFCView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLearnMFCDoc* CLearnMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLearnMFCDoc)));
	return (CLearnMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CLearnMFCView 消息处理程序


int CLearnMFCView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//获取字体，从而自动设定光标大小
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	//创建光标,也可以创建位图光标-18min
	CreateSolidCaret(tm.tmAveCharWidth/8, tm.tmHeight);
	//显示光标
	ShowCaret();

	// 设定定时器，100ms
	SetTimer(1, 100,NULL);

	return 0;
}


void CLearnMFCView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 获取当前字体单字符的高度和长度
	CClientDC dc(this);
	// 设置字体
	CFont font;
	font.CreatePointFont(300, _T("华文行楷"), NULL);
	CFont *pOld = dc.SelectObject(&font);

	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	// 判断回车，0x0d为ASCII码(十进制为13)，对应回车键
	if(0x0d == nChar){
		// 按下回车时清空字符串 && 光标移动到下一行
		m_strLine.Empty();
		m_ptOrigin.y += tm.tmHeight;
	}
	// 退格键删除
	else if (0x08 == nChar) {
		// 按退格键时，先把颜色设置为背景色，注意clr存储的是原字体色
		COLORREF clr = dc.SetTextColor(dc.GetBkColor());
		// 按背景色输出字符串，使用户看不见
		dc.TextOutW(m_ptOrigin.x, m_ptOrigin.y, m_strLine);
		// 减少一个字符串，并恢复字符串原颜色
		m_strLine = m_strLine.Left(m_strLine.GetLength() - 1);
		dc.SetTextColor(clr);
	}
	// 其他输出进行保存字符
	else{
		m_strLine += _TCHAR(nChar);
	}
	// 光标根据输入字符移动
	CSize sz = dc.GetTextExtent(m_strLine);
	CPoint pt;
	pt.x = m_ptOrigin.x + sz.cx;
	pt.y = m_ptOrigin.y;
	SetCaretPos(pt);
	// 输出字符串
	dc.TextOutW(m_ptOrigin.x, m_ptOrigin.y,m_strLine);

	//dc.SelectObject(pOld);

	CView::OnChar(nChar, nRepCnt, nFlags);
}


void CLearnMFCView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	// 设置光标跟随鼠标点击移动
	SetCaretPos(point);
	// 将字符串内所保存的字符清空
	m_strLine.Empty();
	// 保存鼠标点击的坐标，用于在OnChar中进行处理
	m_ptOrigin = point;

	CView::OnLButtonDown(nFlags, point);
}


void CLearnMFCView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	// 按像素为5逐步增加
	m_nWidth += 5;
	// 对于已有的字体，确定矩形长和宽，逐步显示
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);

	CRect rect;
	rect.left = 100; rect.top = 100;
	rect.right = m_nWidth; rect.bottom = rect.top + tm.tmHeight;
	dc.SetTextColor(RGB(255, 0, 0));

	CString str;
	str.LoadStringW(IDS_TEST);
	// 对齐方式 - 左对齐
	dc.DrawText(str, rect, DT_LEFT);

	rect.top = 50;
	rect.bottom = rect.top + tm.tmHeight;
	// 对齐方式 - 右对齐
	dc.DrawText(str, rect, DT_RIGHT);

	// 当矩形宽度大于字符串长度时，重新进行
	CSize sz = dc.GetTextExtent(str);
	if (m_nWidth > sz.cx) {
		m_nWidth = 0;
		// 再次直接改变颜色
		dc.SetTextColor(RGB(0,255,0));
		dc.TextOutW(100,100,str);
	}
	CView::OnTimer(nIDEvent);
}
