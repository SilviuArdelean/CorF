
// ShakeFiguresView.h : interface of the CShakeFiguresView class
//

#pragma once
#include <vector>
#include "ShapesManager.h"

class CShakeFiguresView : public CView
{
protected: // create from serialization only
	CShakeFiguresView();
	DECLARE_DYNCREATE(CShakeFiguresView)

// Attributes
public:
	CShakeFiguresDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CShakeFiguresView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	bool			m_bTimerInProgress;
	CRect			m_moveAreaRect;
	bool			m_flagShapesSwitch;
	short			m_nMilliseconds;
	CShapesManager	*m_pShapesManager;
	short			m_nShapesCount;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSettingsShapesettings();
	afx_msg void OnSettingsShapescount();

	void	MoveShapes();
	void	ChangeSettings();
};


#define ID_TIMER		1

#ifndef _DEBUG  // debug version in ShakeFiguresView.cpp
inline CShakeFiguresDoc* CShakeFiguresView::GetDocument() const
   { return reinterpret_cast<CShakeFiguresDoc*>(m_pDocument); }
#endif

