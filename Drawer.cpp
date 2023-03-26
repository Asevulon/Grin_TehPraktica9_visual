#include"pch.h"
#include"Drawer.h"


void Drawer::Create(HWND hWnd)
{
	// Получаем указатель на окно.
	wnd = CWnd::FromHandle(hWnd);
	// Получаем прямоугольник окна.
	wnd->GetClientRect(frame);
	// Получаем контекст для рисования в этом окне.
	dc = wnd->GetDC();

	// Создаем буфер-контекст.
	memDC.CreateCompatibleDC(dc);
	// Создаем растр для контекста рисования.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// Выбираем растр для использования буфер-контекстом.
	memDC.SelectObject(&bmp);
	init = true;


	memDC.FillSolidRect(frame, RGB(255, 255, 255));
}


double Drawer::convert_range(double data, double outmax, double outmin, double inmax, double inmin)
{
	double k = (outmax - outmin) / (inmax - inmin);
	return  (data - inmin) * k + outmin;
}
vector<double> Drawer::convert_range(vector <double>& data, double outmax, double outmin, double inmax, double inmin)
{
	vector<double> output = data;
	double k = (outmax - outmin) / (inmax - inmin);
	for (auto& item : output)
	{
		item = (item - inmin) * k + outmin;
	}

	return output;
}


void Drawer::DrawGraph(vector<double> data, vector<double> key)
{
	if (!init) return;
	if (key.empty())return;
	if (data.empty())return;

	LocalSort(data, key);


	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(200, 100, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	double data_x_min = *min_element(key.begin(), key.end());
	double data_x_max = *max_element(key.begin(), key.end());
	double data_y_min = *min_element(data.begin(), data.end());
	double data_y_max = *max_element(data.begin(), data.end());

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.2f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen);


	data = convert_range(data, actual_top, actual_bottom, data_y_max, data_y_min);
	key = convert_range(key, actual_right, actual_left, data_x_max, data_x_min);

	memDC.MoveTo(key[0], data[0]);
	for (int i = 0; i < key.size(); i++)
	{
		memDC.LineTo(key[i], data[i]);
	}


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawAppr(vector<double>val, vector<double>key, vector<double>appr)
{
	if (!init) return;
	if (key.empty())return;
	if (val.empty())return;
	if (appr.empty())return;


	//LocalSort(val, key);


	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(200, 100, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 100, 200));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// Курсив
		FALSE,				// Подчеркнутый
		FALSE,				// Перечеркнутый
		DEFAULT_CHARSET,	// Набор символов
		OUT_OUTLINE_PRECIS,	// Точность соответствия.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// Качество
		VARIABLE_PITCH,		//
		TEXT("Times New Roman")		//
	);

	int padding = 20;
	int left_keys_padding = 20;
	int bottom_keys_padding = 10;

	int actual_width = frame.Width() - 2 * padding - left_keys_padding;
	int actual_height = frame.Height() - 2 * padding - bottom_keys_padding;

	int actual_top = padding;
	int actual_bottom = actual_top + actual_height;
	int actual_left = padding + left_keys_padding;
	int actual_right = actual_left + actual_width;


	double data_x_min = *min_element(key.begin(), key.end());
	double data_x_max = *max_element(key.begin(), key.end());
	double data_y_min = *min_element(val.begin(), val.end());
	double data_y_max = *max_element(val.begin(), val.end());

	// Белый фон.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// Рисуем сетку и подсетку.
	unsigned int grid_size = 10;

	memDC.SelectObject(&subgrid_pen);

	for (double i = 0.5; i < grid_size; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}

	memDC.SelectObject(&grid_pen);

	for (double i = 0.0; i < grid_size + 1; i += 1.0)
	{
		memDC.MoveTo(actual_left + i * actual_width / grid_size, actual_top);
		memDC.LineTo(actual_left + i * actual_width / grid_size, actual_bottom);
		memDC.MoveTo(actual_left, actual_top + i * actual_height / grid_size);
		memDC.LineTo(actual_right, actual_top + i * actual_height / grid_size);
	}




	memDC.SelectObject(&font);
	memDC.SetTextColor(RGB(0, 0, 0));
	for (int i = 0; i < grid_size / 2 + 1; i++)
	{
		CString str;
		str.Format(L"%.2f", data_x_min + i * (data_x_max - data_x_min) / (grid_size / 2));
		memDC.TextOutW(actual_left + (double)i * actual_width / (grid_size / 2) - bottom_keys_padding, actual_bottom + bottom_keys_padding / 2, str);

		str.Format(L"%.1f", data_y_min + i * (data_y_max - data_y_min) / (grid_size / 2));
		memDC.TextOutW(actual_left - 1.5 * left_keys_padding, actual_bottom - (double)i * actual_height / (grid_size / 2) - bottom_keys_padding, str);
	}


	memDC.SelectObject(&data_pen);

	
	double x0(data_x_min);
	double y0 = appr[0] * x0 + appr[1];
	double x1(data_x_max);
	double y1 = appr[0] * x1 + appr[1];

	if (y0 < data_y_min)data_y_min = y0;
	if (y1 > data_y_max)data_y_max = y1;

	
	y0 = convert_range(y0, actual_top, actual_bottom, data_y_max, data_y_min);
	y1 = convert_range(y1, actual_top, actual_bottom, data_y_max, data_y_min);

	x0 = convert_range(x0, actual_right, actual_left, data_x_max, data_x_min);
	x1 = convert_range(x1, actual_right, actual_left, data_x_max, data_x_min);

	memDC.MoveTo(x0, y0);
	memDC.LineTo(x1, y1);

	memDC.SelectObject(&data_pen2);

	val = convert_range(val, actual_top, actual_bottom, data_y_max, data_y_min);
	key = convert_range(key, actual_right, actual_left, data_x_max, data_x_min);

	for (int i = 0; i < key.size(); i++)
	{
		memDC.Ellipse(key[i] - 2, val[i] - 2, key[i] + 2, val[i] + 2);
	}





	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}


void Drawer::LocalSort(vector<double>& data, vector<double>& key)
{
	int min = 0;

	for (int i = 0; i < data.size() - 1; i++)
	{
		min = i;
		for (int g = i + 1; g < data.size(); g++)
		{
			if (key[g] < key[min])
			{
				min = g;
			}
		}
		if (min == i)continue;
		double dtemp = data[i];
		double ktemp = key[i];

		data[i] = data[min];
		key[i] = key[min];

		data[min] = dtemp;
		key[min] = ktemp;
	}
}


void Drawer::Clear()
{
	memDC.FillSolidRect(frame, RGB(255, 255, 255));
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::Redraw()
{
	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}

