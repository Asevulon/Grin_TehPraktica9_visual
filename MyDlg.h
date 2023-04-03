
// MyDlg.h: файл заголовка
//

#pragma once

#define TIMERVAL 1000
#define GONNAKILL (WM_USER+0)
#define KILLME (WM_USER+1)
#include"Drawer.h"
#include"func.h"
#include<chrono>

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
	bool appr;
	afx_msg void OnBnClickedCheck2();
	afx_msg LRESULT KillThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGONNAKILL(WPARAM wParam, LPARAM lParam);
	afx_msg void MyDlg::OnTimer(UINT_PTR idEvent);
	bool NeedToDraw;
	vector<vector<double>>ktdrw, vtdrw;
	DWORD_PTR tid;
	bool ReadyToBeTerminated;
	bool gonnakill;
	int ApprN;
	int GlobalCounter;
	vector<double>iter;
	vector<double>cas;
	int GlobalClock;
};


DWORD WINAPI multithread(LPVOID);