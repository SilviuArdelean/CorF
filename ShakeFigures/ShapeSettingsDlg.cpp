// ShapeSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShapeSettingsDlg.h"

#include "afxdialogex.h"


// CShapeSettingsDlg dialog

IMPLEMENT_DYNAMIC(CShapeSettingsDlg, CDialog)

CShapeSettingsDlg::CShapeSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShapeSettingsDlg::IDD, pParent)
	, m_nShapeSize(50)
	, m_nShapeMoveSpeed(100)
	, m_nMiliseconds(500)

{

}

CShapeSettingsDlg::~CShapeSettingsDlg()
{
}

void CShapeSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_SHAPE_SIZE, m_ctrlShapeSize);
	DDX_Slider(pDX, IDC_SLIDER_SHAPE_SIZE, m_nShapeSize);
	DDX_Control(pDX, IDC_SLIDER_SHAPE_MOVE_SPEED, m_ctrlShapeSpeed);
	DDX_Slider(pDX, IDC_SLIDER_SHAPE_MOVE_SPEED, m_nShapeMoveSpeed);
	DDX_Control(pDX, IDC_SLIDER_SHAPE_MILISECONDS, m_ctrlMiliseconds);
	DDX_Slider(pDX, IDC_SLIDER_SHAPE_MILISECONDS, m_nMiliseconds);
	DDX_Control(pDX, IDC_EDIT_SIZE, m_editSize);
	DDX_Control(pDX, IDC_EDIT_SPEED, m_editSpeed);
	DDX_Control(pDX, IDC_EDIT_MILISECONDS, m_editMiliseconds);
}


BEGIN_MESSAGE_MAP(CShapeSettingsDlg, CDialog)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CShapeSettingsDlg message handlers

BOOL CShapeSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	
	
	m_ctrlShapeSize.SetRange(50, 200);
	m_ctrlShapeSize.SetPos(100);
	m_ctrlShapeSize.SetTic(100);
	m_ctrlShapeSize.SetTicFreq(10);

	m_ctrlShapeSpeed.SetRange(10, 500);
	m_ctrlShapeSpeed.SetPos(50);
	m_ctrlShapeSpeed.SetTic(10);
	m_ctrlShapeSpeed.SetTicFreq(10);

	m_ctrlMiliseconds.SetRange(100, 1000);
	m_ctrlMiliseconds.SetPos(500);
	m_ctrlMiliseconds.SetTic(10);
	m_ctrlMiliseconds.SetTicFreq(10);

	CString strTmp;
	strTmp.Format(_T("%d"), m_nShapeSize);
	m_editSize.SetWindowTextW(strTmp);
	
	strTmp.Format(_T("%d"), m_nShapeMoveSpeed);
	m_editSpeed.SetWindowTextW(strTmp);
	
	strTmp.Format(_T("%d"), m_nMiliseconds);
	m_editMiliseconds.SetWindowTextW(strTmp);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShapeSettingsDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl *pSliderCtrl = (CSliderCtrl *)pScrollBar;
	int nID = pSliderCtrl->GetDlgCtrlID();

	UpdateData(TRUE);
	CString strTmp;

	switch (nID)
	{
		case IDC_SLIDER_SHAPE_SIZE:
			strTmp.Format(_T("%d"), m_nShapeSize);
			m_editSize.SetWindowTextW(strTmp);
			break;

		case IDC_SLIDER_SHAPE_MOVE_SPEED:
			strTmp.Format(_T("%d"), m_nShapeMoveSpeed);
			m_editSpeed.SetWindowTextW(strTmp);
			break;

		case IDC_SLIDER_SHAPE_MILISECONDS:
			strTmp.Format(_T("%d"), m_nMiliseconds);
			m_editMiliseconds.SetWindowTextW(strTmp);
			break;

		default:
			break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
