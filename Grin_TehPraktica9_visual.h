
// Grin_TehPraktica9_visual.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CGrinTehPraktica9visualApp:
// Сведения о реализации этого класса: Grin_TehPraktica9_visual.cpp
//

class CGrinTehPraktica9visualApp : public CWinApp
{
public:
	CGrinTehPraktica9visualApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CGrinTehPraktica9visualApp theApp;
