// ShapeSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShapesCountSelDlg.h"
#include "afxdialogex.h"
#include "TCHAR.H"


// CShapesCountSelDlg dialog

IMPLEMENT_DYNAMIC(CShapesCountSelDlg, CDialog)

CShapesCountSelDlg::CShapesCountSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapesCountSelDlg::IDD, pParent)
	, m_selCount(4)
{

}

CShapesCountSelDlg::~CShapesCountSelDlg()
{
}

void CShapesCountSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ITEMS_COUNT, m_ctrlSelCounter);
}


BEGIN_MESSAGE_MAP(CShapesCountSelDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEMS_COUNT, &CShapesCountSelDlg::OnCbnSelchangeComboboxexItems)
END_MESSAGE_MAP()

// CShapesCountSelDlg message handlers

BOOL CShapesCountSelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	

	m_ctrlSelCounter.AddString(_T("1"));
	m_ctrlSelCounter.AddString(_T("2"));
	m_ctrlSelCounter.AddString(_T("3"));
	m_ctrlSelCounter.AddString(_T("4"));
	
	m_ctrlSelCounter.SetCurSel(3);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShapesCountSelDlg::OnCbnSelchangeComboboxexItems()
{
	auto item_index = m_ctrlSelCounter.GetCurSel();

	CString strtmp;
	if (item_index > -1)
	{
		m_ctrlSelCounter.GetLBText(item_index, strtmp);

		m_selCount = (short) _ttoi(strtmp);
	}
}