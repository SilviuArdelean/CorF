#pragma once

#include "afxcmn.h"
#include "resource.h"
#include "afxwin.h"

// CShapeSettingsDlg dialog

class CShapeSettingsDlg : public CDialog
{
	DECLARE_DYNAMIC(CShapeSettingsDlg)

public:
	CShapeSettingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShapeSettingsDlg();

	// Dialog Data
	enum { IDD = IDD_SHAPE_SETTINGS };

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
	int GetShapeSize() const { return m_nShapeSize; }
	int	GetMoveSpeed() const { return m_nShapeMoveSpeed; }
	int GetMiliseconds() const { return m_nMiliseconds; }

private:
	CSliderCtrl m_ctrlShapeSize;
	CSliderCtrl m_ctrlShapeSpeed;
	CSliderCtrl m_ctrlMiliseconds;

	int m_nShapeSize;
	int m_nShapeMoveSpeed;
	int m_nMiliseconds;

	CEdit m_editSize;
	CEdit m_editSpeed;
	CEdit m_editMiliseconds;
};
