
// Learn_MFCView.h: CLearnMFCView 类的接口
//

#pragma once


class CLearnMFCView : public CView
{
protected: // 仅从序列化创建
	CLearnMFCView() noexcept;
	DECLARE_DYNCREATE(CLearnMFCView)

// 特性
public:
	CLearnMFCDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CLearnMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	// 用于输入文字，存储输入的字符串
	CString m_strLine;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 用于存储鼠标点击的坐标
	CPoint m_ptOrigin;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 宽度变量
	int m_nWidth;
};

#ifndef _DEBUG  // Learn_MFCView.cpp 中的调试版本
inline CLearnMFCDoc* CLearnMFCView::GetDocument() const
   { return reinterpret_cast<CLearnMFCDoc*>(m_pDocument); }
#endif

