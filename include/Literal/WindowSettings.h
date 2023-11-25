#ifndef LITERAL_STRUCT_WINDOW_SETTINGS_H
#define LITERAL_STRUCT_WINDOW_SETTINGS_H

#include <Windows.h>
#include "Arson/Style.h"

namespace Lit
{
	//Enum Holding the Possible window styles
	enum WindowStyles : long {
		DEFAULT = 351207424L,
		RESIZABLE = WS_SIZEBOX,
		TITLE_BAR = WS_SYSMENU | WS_CAPTION,
		//WARNING : Must Have Title bar to take effect
		MAXIMIZE_BUTTON = WS_MAXIMIZEBOX,
		//WARNING : Must Have Title bar to take effect
		MINIMIZE_BUTTON = WS_MINIMIZEBOX,

		BASE = DEFAULT ^ RESIZABLE ^ MAXIMIZE_BUTTON ^ MINIMIZE_BUTTON ^ WS_SYSMENU ^ WS_BORDER,
	};

	//Structure which stores configurable settings for the window
	struct WindowSettings
	{
		wchar_t WIN_TITLE[MAX_PATH]{ L"[PLACEHOLDER]" };
		long WIN_STYLE{ WindowStyles::DEFAULT };
		wchar_t WIN_BORDER{ L'\u2588' };
	};
}

#endif