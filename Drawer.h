#pragma once

#include <afxwin.h>
#include <vector>
#include<algorithm>
using namespace std;


class Drawer
{
	// Прямоугольник области рисования.
	CRect frame;
	// Указатель на объект окна, обалсти рисования.
	CWnd * wnd;
	// Контекст рисования, привязанный к области рисования.
	CDC * dc;
	// Контекст рисования, привязанный к памяти.
	CDC memDC;
	// Память для контекста рисования memDC.
	CBitmap bmp;
	// Флаг для отслеживания состояния инициализации класса.
	bool init;

protected:
	double convert_range(double data, double outmax, double outmin, double inmax, double inmin);
	vector<double> convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin);
	void LocalSort(vector<double>& data, vector<double>& key);


public:
	// Проинициализировать объект класса на основе HWND.
	void Create(HWND hWnd);
	
	void Drawer::DrawGraph(vector<double> data, vector<double> key);
	
	void Redraw();
	void Clear();
};
