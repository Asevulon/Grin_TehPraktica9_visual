
// MyDlg.h: файл заголовка
//

#pragma once

#define TIMERVAL 1000

#include"Drawer.h"
#include"func.h"


// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GRIN_TEHPRAKTICA9_VISUAL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	int n;
	afx_msg void OnBnClickedOk();
	Drawer drw;
	int len;
	static DWORD WINAPI MyDlg::thfunc(LPVOID);
	HANDLE thr;
	CListBox log;
	CListBox ctrlog;
	afx_msg void OnBnClickedCheck1();
	bool filtr;
	double cap;
};
