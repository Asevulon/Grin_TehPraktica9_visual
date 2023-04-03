#include"pch.h"
#include"Drawer.h"


void Drawer::Create(HWND hWnd)
{
	// �������� ��������� �� ����.
	wnd = CWnd::FromHandle(hWnd);
	// �������� ������������� ����.
	wnd->GetClientRect(frame);
	// �������� �������� ��� ��������� � ���� ����.
	dc = wnd->GetDC();

	// ������� �����-��������.
	memDC.CreateCompatibleDC(dc);
	// ������� ����� ��� ��������� ���������.
	bmp.CreateCompatibleBitmap(dc, frame.Width(), frame.Height());
	// �������� ����� ��� ������������� �����-����������.
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
		FALSE,				// ������
		FALSE,				// ������������
		FALSE,				// �������������
		DEFAULT_CHARSET,	// ����� ��������
		OUT_OUTLINE_PRECIS,	// �������� ������������.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// ��������
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

	// ����� ���.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// ������ ����� � ��������.
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
void Drawer::DrawDots(vector<double> data, vector<double> key)
{
	if (!init) return;
	if (key.empty())return;
	if (data.empty())return;



	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(0, 100, 200));
	CPen data_pen2(PS_SOLID, 2, RGB(200, 100, 0));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// ������
		FALSE,				// ������������
		FALSE,				// �������������
		DEFAULT_CHARSET,	// ����� ��������
		OUT_OUTLINE_PRECIS,	// �������� ������������.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// ��������
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

	// ����� ���.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// ������ ����� � ��������.
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

	for (int i = 0; i < key.size() - 1; i++)
	{
		memDC.Ellipse(key[i] - 2, data[i] - 2, key[i] + 2, data[i] + 2);
	}

	memDC.SelectObject(&data_pen2);

	memDC.Ellipse(key[key.size() - 1] - 2, data[data.size() - 1] - 2, key[key.size() - 1] + 2, data[data.size() - 1] + 2);


	dc->BitBlt(0, 0, frame.Width(), frame.Height(), &memDC, 0, 0, SRCCOPY);
}
void Drawer::DrawAppr(vector<double>val, vector<double>key, vector<double>apprval,vector<double> apprkey)
{
	if (!init) return;
	if (key.empty())return;
	if (val.empty())return;
	if (apprval.empty())return;
	if (apprkey.empty())return;


	//LocalSort(val, key);


	CPen subgrid_pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen grid_pen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen data_pen(PS_SOLID, 2, RGB(200, 100, 0));
	CPen data_pen2(PS_SOLID, 2, RGB(0, 100, 200));
	CFont font;
	font.CreateFontW(18, 0, 0, 0,
		FW_DONTCARE,
		FALSE,				// ������
		FALSE,				// ������������
		FALSE,				// �������������
		DEFAULT_CHARSET,	// ����� ��������
		OUT_OUTLINE_PRECIS,	// �������� ������������.	
		CLIP_DEFAULT_PRECIS,//  
		CLEARTYPE_QUALITY,	// ��������
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


	double data_x_min = min(*min_element(key.begin(), key.end()), *min_element(apprkey.begin(), apprkey.end()));
	double data_x_max = max(*max_element(key.begin(), key.end()), *max_element(apprkey.begin(), apprkey.end()));
	double data_y_min = min(*min_element(val.begin(), val.end()), *min_element(apprval.begin(), apprval.end()));
	double data_y_max = max(*max_element(val.begin(), val.end()), *max_element(apprval.begin(), apprval.end()));

	// ����� ���.
	memDC.FillSolidRect(frame, RGB(255, 255, 255));

	// ������ ����� � ��������.
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


	memDC.SelectObject(&data_pen2);

	val = convert_range(val, actual_top, actual_bottom, data_y_max, data_y_min);
	key = convert_range(key, actual_right, actual_left, data_x_max, data_x_min);

	for (int i = 0; i < key.size(); i++)
	{
		memDC.Ellipse(key[i] - 2, val[i] - 2, key[i] + 2, val[i] + 2);
	}



	memDC.SelectObject(&data_pen);

	
	
	apprval = convert_range(apprval, actual_top, actual_bottom, data_y_max, data_y_min);
	apprkey = convert_range(apprkey, actual_right, actual_left, data_x_max, data_x_min);
	
	memDC.MoveTo(apprkey[0], apprval[0]);
	for (int i = 0; i < apprval.size(); i++)memDC.LineTo(apprkey[i], apprval[i]);
	

	





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

