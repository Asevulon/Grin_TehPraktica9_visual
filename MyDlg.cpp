
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
	, cap(2e3)
	, appr(false)
	, NeedToDraw(false)
	, ReadyToBeTerminated(true)
	, ApprN(3)
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

	iter.clear();
	cas.clear();


	tid = SetTimer(1, TIMERVAL, NULL);
	gonnakill = false;
	GlobalCounter = 0;
	CloseHandle(thr);
	GlobalClock = clock();
	thr = CreateThread(NULL, NULL, thfunc, this, NULL, NULL);
}


struct multithreadData
{
	MyDlg* dlg = nullptr;
	int id = 0;
	multithreadData() :dlg(nullptr), id(0) {}
	multithreadData(MyDlg* dlg, int id) :dlg(dlg), id(id) {}
};
CRITICAL_SECTION cs;

DWORD WINAPI MyDlg::thfunc(LPVOID p)
{
	InitializeCriticalSection(&cs);


	srand(time(NULL));
	
	float clck = clock();

	MyDlg* buf = (MyDlg*)p;

	buf->gonnakill = false;

	int n = buf->n;
	int len = buf->len;
	
	multithreadData td1(buf, 1);
	multithreadData td2(buf, 2);
	multithreadData td3(buf, 3);
	multithreadData td4(buf, 4);
		
	HANDLE thread1 = CreateThread(NULL, NULL, multithread, &td1, NULL, NULL);
	HANDLE thread2 = CreateThread(NULL, NULL, multithread, &td2, NULL, NULL);
	HANDLE thread3 = CreateThread(NULL, NULL, multithread, &td3, NULL, NULL);
	HANDLE thread4 = CreateThread(NULL, NULL, multithread, &td4, NULL, NULL);
		

	while (buf->GlobalCounter < len)
	{
		if (buf->gonnakill)
		{
			CloseHandle(thread1);
			CloseHandle(thread2);
			CloseHandle(thread3);
			CloseHandle(thread4);
			ExitThread(0);
		}
		CString timestr;
		timestr.Format(L"Текущее время выполнения: %.2f", (clock() - clck) / 1000.);
		buf->log.InsertString(-1, timestr);
		timestr.Format(L"длина массива: %d", buf->iter.size());
		buf->log.InsertString(-1, timestr);
		buf->log.SetTopIndex(buf->log.GetCount() - 1);

		Sleep(0);
	}

	buf->KillTimer(buf->tid);
	
	CloseHandle(thread1);
	CloseHandle(thread2);
	CloseHandle(thread3);
	CloseHandle(thread4);

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
		ApprMHJ(buf->ApprN, buf->iter, buf->cas, x, calc);
		vector<double>aval, akey;
		MakeApprValnKeys(x, *min_element(buf->cas.begin(), buf->cas.end()), *max_element(buf->cas.begin(), buf->cas.end()), 200, aval, akey);
		buf->drw.DrawAppr(buf->iter, buf->cas, aval, akey);
	}
	else buf->drw.DrawDots(buf->iter, buf->cas);

	CString str;
	str.Format(L"Завершено!");
	buf->log.InsertString(-1, str);
	str.Format(L"Полное время выполнения: %.2f cек", (clock() - clck)/1000.);
	buf->log.InsertString(-1, str);

	buf->log.SetTopIndex(buf->log.GetCount() - 1);

	return 0;
}

DWORD WINAPI multithread(LPVOID p)
{
	multithreadData* td = (multithreadData*) p;
	MyDlg* buf = td->dlg;

	buf->gonnakill = false;
	int lockalclck = buf->GlobalClock;
	lockalclck += clock();
	srand(lockalclck / td->id);
	int n = buf->n;
	int len = buf->len;
	while (buf->GlobalCounter < len)
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
				MHJ(n, A, B, x, calc, buf->cap);
			}
		);

		th1.join();
		th2.join();


		if (buf->gonnakill)ExitThread(0);



		if ((calc > buf->cap) || (sigma[0] / sigma[n - 1] > 500))continue;

		EnterCriticalSection(&cs);

		buf->iter.push_back(calc);
		buf->cas.push_back(sigma[0] / sigma[n - 1]);

		if (buf->NeedToDraw)
		{
			buf->NeedToDraw = false;
			buf->ktdrw.push_back(buf->cas);
			buf->vtdrw.push_back(buf->iter);
		}
		buf->GlobalCounter++;
		LeaveCriticalSection(&cs);
		
	}

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
		drw.DrawDots(vtdrw[0], ktdrw[0]);
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