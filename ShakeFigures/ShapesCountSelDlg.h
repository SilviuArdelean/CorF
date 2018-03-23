#pragma once

#include "afxcmn.h"
#include "resource.h"
#include "afxwin.h"

// CShapesCountSelDlg dialog

class CShapesCountSelDlg : public CDialog
{
	DECLARE_DYNAMIC(CShapesCountSelDlg)

public:
	CShapesCountSelDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShapesCountSelDlg();

	// Dialog Data
	enum { IDD = IDD_DLG_SHAPES_COUNT };

	short GetSelCount() const { return m_selCount; };

protected:
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

private:
	short m_selCount;
public:
	afx_msg void OnCbnSelchangeComboboxexItems();
	CComboBox m_ctrlSelCounter;
};
