# 文本
## 创建光标
```c++
CLearnMFCView::OnCreate
	//获取字体，从而自动设定光标大小
	CClientDC dc(this);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	//创建光标,也可以创建位图光标-18min
	CreateSolidCaret(tm.tmAveCharWidth/8, tm.tmHeight);
	//显示光标
	ShowCaret();
```

## 字符串资源与显示
```c++
CLearnMFCView::OnDraw
// 注意CString的使用
    // 定义并输入字符串
    CString str;
	str = "test";
	pDC->TextOutW(50, 50, str);
    
    //定义好字符串资源后
    str.LoadStringW(IDS_TEST);
	pDC->TextOutW(100, 100, str);
```

## 路径层的功能（实现图层间的覆盖操作）
```c++
CLearnMFCView::OnDraw
	// 定义并输入字符串
	CString str;
	str = "test";
	pDC->TextOutW(50, 50, str);
	// 获取字符串高度和宽度（注意必须放在改变字符串之前）
	CSize sz = pDC->GetTextExtent(str);
	//创建路径层,通过剪切区域实现不同效果
	pDC->BeginPath();
	// 创建矩形
	pDC->Rectangle(50, 50, 50 + sz.cx, 50 + sz.cy);
	pDC->EndPath();
	// 设置路径层与其他层的互操作模式
	pDC->SelectClipPath(RGN_DIFF);
	// 创建方格，用于演示路径层作用
	for (int i = 0; i < 300; i += 10) {
		pDC->MoveTo(0,i);
		pDC->LineTo(300, i);
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 300);
	}
```

## 输入文字
```c++
//在p-View类中添加的成员变量
- CString   m_strLine   用于存储输入字符串
- CPoint    m_ptOrigin  用于存储鼠标点击的坐标

CLearnMFCView::CLearnMFCView
    // 初始化变量
    m_strLine = "";
    m_ptOrigin = 0；

CLearnMFCView::OnLButtonDown
    // 设置光标跟随鼠标点击移动
	SetCaretPos(point);
	// 将字符串内所保存的字符清空
	m_strLine.Empty();
	// 保存鼠标点击的坐标，用于在OnChar中进行处理
	m_ptOrigin = point;

CLearnMFCView::OnChar
    // 获取当前字体单字符的高度和长度
	CClientDC dc(this);
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
```

## 改变字体
```c++
CLearnMFCView::OnChar
    CClientDC dc(this);
	// 设置字体
	CFont font;
	font.CreatePointFont(300, _T("华文行楷")，NULL);
    // 选择上字体，同时pOld用来存放原字体指针
	CFont *pOld = dc.SelectObject(&font);

```
## 卡拉OK变色效果（平滑变色）
```c++
- int   m_nWidth    宽度变量

CLearnMFCView::CLearnMFCView
    m_nWidth = 0;

CLearnMFCView::OnCreate
    // 设定定时器，每100ms调用中断相应函数，定时器标识为1
	SetTimer(1, 100,NULL);

CLearnMFCView::OnTimer
    // 按像素为5逐步增加
	m_nWidth += 5;
	// 对于已有的字体，确定矩形长和宽，逐步显示
    // 注意：变色是在已有字体上，原有黑色，再建一个红色的渐变来覆盖
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

```



