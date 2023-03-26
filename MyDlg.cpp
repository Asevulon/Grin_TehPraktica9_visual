
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Grin_TehPraktica9_visual.h"
#include "MyDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GRIN_TEHPRAKTICA9_VISUAL_DIALOG, pParent)
	, n(3)
	, len(20)
	, filtr(false)
	, cap(1e4)
	, appr(false)
	, NeedToDraw(false)
	, ReadyToBeTerminated(true)
	, ApprN(2)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, n);
	DDX_Text(pDX, IDC_EDIT2, len);
	DDX_Control(pDX, IDC_LIST1, log);
	DDX_Control(pDX, IDC_LIST2, ctrlog);
	DDX_Text(pDX, IDC_EDIT3, cap);
	DDX_Text(pDX, IDC_EDIT4, ApprN);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK1, &MyDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &MyDlg::OnBnClickedCheck2)
	ON_MESSAGE(KILLME, &MyDlg::KillThread)
	ON_MESSAGE(GONNAKILL,&MyDlg::OnGONNAKILL)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	drw.Create(GetDlgItem(IDC_PICTURE)->GetSafeHwnd());

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		drw.Redraw();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	DWORD excode;
	GetExitCodeThread(thr, &excode);
	if (excode == STILL_ACTIVE)
	{
		PostMessage(GONNAKILL);
		return;
	}

	UpdateData();

	log.ResetContent();
	ctrlog.ResetContent();

	ktdrw.clear();
	vtdrw.clear();

	tid = SetTimer(1, TIMERVAL, NULL);
	gonnakill = false;
	thr = CreateThread(NULL, NULL, thfunc, this, NULL, NULL);
}


DWORD WINAPI MyDlg::thfunc(LPVOID p)
{
	srand(time(NULL));
	
	float clck = clock();

	MyDlg* buf = (MyDlg*)p;

	buf->gonnakill = false;

	int n = buf->n;
	int len = buf->len;
	vector<double>iter;
	vector<double>cas;
	for (int i = 0; i < len; i++)
	{
		auto linearA = GetRandomMatrix(n);

		vector<double>sigma;


		thread th1
		(
			[&sigma, linearA, n]()
			{
				sigma = calcSigma(linearA, n);
			}
		);

		auto A = TransformLinearMatrix(linearA, n);
		auto B = GetRandomB(n);
		int calc = 0;
		auto x = GetRandomB(n);
		thread th2
		(
			[&calc, A, B, n, &x, buf]()
			{
				MHJ(n, A, B, x, calc);
			}
		);
		
		th1.join();
		th2.join();
			

		if (buf->gonnakill)buf->PostMessageW(KILLME);


		if(buf->filtr)
		if (calc > buf->cap)
		{
			CString str;

			str.Format(L"Позиция в выборке: %d", i);
			buf->ctrlog.InsertString(-1, str);


			str.Format(L"Число итераций: %d", calc);
			buf->ctrlog.InsertString(-1, str);

			str.Format(L"Обусловленность: %.2f", sigma[0] / sigma[n - 1]);
			buf->ctrlog.InsertString(-1, str);

			str.Format(L"Время выполнения: %.2f cек", (clock() - clck) / 1000.);
			buf->ctrlog.InsertString(-1, str);

			buf->ctrlog.InsertString(-1, L" ");

			buf->ctrlog.SetTopIndex(buf->ctrlog.GetCount() - 1);
			i--;
			continue;
		}


		iter.push_back(calc);
		cas.push_back(sigma[0] / sigma[n - 1]);


		CString str;
		str.Format(L"Позиция в выборке: %d", i);
		buf->log.InsertString(-1, str);

		
		str.Format(L"Число итераций: %d", calc);
		buf->log.InsertString(-1, str);

		str.Format(L"Обусловленность: %.2f", sigma[0] / sigma[n - 1]);
		buf->log.InsertString(-1, str);

		str.Format(L"Время выполнения: %.2f cек", (clock() - clck) / 1000.);
		buf->log.InsertString(-1, str);

		buf->log.InsertString(-1, L" ");

		buf->log.SetTopIndex(buf->log.GetCount() - 1);

		
		if (buf->NeedToDraw)
		{
			buf->NeedToDraw = false;
			buf->ktdrw.push_back(cas);
			buf->vtdrw.push_back(iter);
		}
	}

	buf->KillTimer(buf->tid);


	if (buf->appr)
	{
		vector<double>x;
		for (int i = 0; i < buf->ApprN; i++) x.push_back(RANDMIN + RANDMAX * double(rand()) / double(RAND_MAX));

		
		CString str;
		str.Format(L"Вычисление аппроксимирующих коэффициентов");
		buf->log.InsertString(-1, str);
		buf->log.InsertString(-1, L" ");
		buf->log.SetTopIndex(buf->log.GetCount() - 1);


		int calc = 0;
		ApprMHJ(buf->ApprN, iter, cas, x, calc);
		vector<double>aval, akey;
		MakeApprValnKeys(x, *min_element(cas.begin(), cas.end()), *max_element(cas.begin(), cas.end()), 200, aval, akey);
		buf->drw.DrawAppr(iter, cas, aval, akey);
	}
	else buf->drw.DrawGraph(iter, cas);

	CString str;
	str.Format(L"Завершено!");
	buf->log.InsertString(-1, str);
	str.Format(L"Полное время выполнения: %.2f cек", (clock() - clck)/1000.);
	buf->log.InsertString(-1, str);

	buf->log.SetTopIndex(buf->log.GetCount() - 1);

	return 0;
}





void MyDlg::OnBnClickedCheck1()
{
	// TODO: добавьте свой код обработчика уведомлений
	filtr = !filtr;
}


void MyDlg::OnBnClickedCheck2()
{
	// TODO: добавьте свой код обработчика уведомлений
	appr = !appr;
}


afx_msg void MyDlg::OnTimer(UINT_PTR idEvent)
{
	NeedToDraw = true;

	while (!vtdrw.empty())
	{
		drw.DrawGraph(vtdrw[0], ktdrw[0]);
		vtdrw.erase(vtdrw.begin());
		ktdrw.erase(ktdrw.begin());
	}
}


afx_msg LRESULT MyDlg::KillThread(WPARAM wParam, LPARAM lParam)
{
	TerminateThread(thr, 0);
	CloseHandle(thr);


	UpdateData();

	log.ResetContent();
	ctrlog.ResetContent();

	ktdrw.clear();
	vtdrw.clear();

	tid = SetTimer(1, TIMERVAL, NULL);
	gonnakill = false;
	thr = CreateThread(NULL, NULL, thfunc, this, NULL, NULL);

	return 0;
}

afx_msg LRESULT MyDlg::OnGONNAKILL(WPARAM wParam, LPARAM lParam)
{
	gonnakill = true;
	return 0;
}