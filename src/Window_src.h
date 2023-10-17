#ifndef LITERAL_CLASS_WINDOW_SCR_H
#define LITERAL_CLASS_WINDOW_SCR_H

#include <fcntl.h>
#include "Window.h"

template <SHORT Width, SHORT Height>
void Window<Width, Height>::set_cursor_position_in_window(Vector2<SHORT> position) const
{
	std::wcout.flush();
	//Correction for the screen border 
	const COORD coordinates = {++position.x, ++position.y};
	SetConsoleCursorPosition(CMD_OUTPUT_OBJ_HANDLE, coordinates);
}

template <SHORT Width, SHORT Height>
void Window<Width, Height>::set_cursor_position_abs(const Vector2<SHORT> position) const
{
	std::wcout.flush();
	const COORD coordinates = {position.x, position.y};
	SetConsoleCursorPosition(CMD_OUTPUT_OBJ_HANDLE, coordinates);
}

template <SHORT Width, SHORT Height>
Vector2<SHORT> Window<Width, Height>::get_cursor_position_in_window()
{
	std::wcout.flush();
	GetConsoleScreenBufferInfo(CMD_OUTPUT_OBJ_HANDLE, &CMD_BUFFER_DATA);

	return {--CMD_BUFFER_DATA.dwCursorPosition.X, --CMD_BUFFER_DATA.dwCursorPosition.Y};
}

template <SHORT Width, SHORT Height>
Vector2<SHORT> Window<Width, Height>::get_cursor_position_abs()
{
	std::wcout.flush();
	GetConsoleScreenBufferInfo(CMD_OUTPUT_OBJ_HANDLE, &CMD_BUFFER_DATA);

	return {CMD_BUFFER_DATA.dwCursorPosition.X, CMD_BUFFER_DATA.dwCursorPosition.Y};
}

template <SHORT Width, SHORT Height>
void Window<Width, Height>::draw_window_border()
{
	for (size_t index{0}; index <= Width + 1; ++index)
	{
		std::wcout.write(&WIN_BORDER, 1);
	}
	std::wcout.write(L"\n", 1);
	for (size_t delta_index{0}; delta_index <= 2 * Height; ++delta_index)
	{
		//Print the Border on One side than jump to the Other Side
		if (delta_index % 2 == 0)
		{
			std::wcout.write(&WIN_BORDER, 1);
			set_cursor_position_abs({Width + 1, get_cursor_position_abs().y});
		}
		else
		{
			std::wcout.write(&WIN_BORDER, 1);
			std::wcout.write(L"\n", 1);
		}
	}
	set_cursor_position_abs({0, get_cursor_position_abs().y});
	for (size_t index{0}; index <= Width + 1; ++index)
	{
		std::wcout.write(&WIN_BORDER, 1);
	}
	std::wcout.write(L"\n", 1);
}

template <SHORT Width, SHORT Height>
void Window<Width, Height>::init_console()
{
	//Corrector Exists so that 1x1 squares can work correctly
	constexpr int size_corrector{7};
	Vector2<int> cmd_dimensions_px = {
		(Width + size_corrector) * (CMD_CHAR_DATA.dwFontSize.Y / 2 + 1),
		(Height + size_corrector) * CMD_CHAR_DATA.dwFontSize.Y
	};

	const HDC CMD_GRAPHICS_HANDLER = GetDC(CMD_HANDLE);
	//LP == LOGICAL COORDINATES	 , DP == PIXELS
	POINT CMD_CHAR_SIZE_LP = {CMD_CHAR_DATA.dwFontSize.X, CMD_CHAR_DATA.dwFontSize.Y};
	LPtoDP(CMD_GRAPHICS_HANDLER, &CMD_CHAR_SIZE_LP, 1);

	POINT MODIFIED_CMD_CHAR_SIZE_LP = {CMD_CHAR_SIZE_LP};
	//Sets the Console dimensions to Desktop dimensions if it is too big
	//Find the Amount that the Character size should shrink by to fit to the screen
	if (cmd_dimensions_px.y > DESKTOP_WIN_RECT.bottom)
	{
		const double y_multiplier = static_cast<double>(cmd_dimensions_px.y) / DESKTOP_WIN_RECT.bottom;
		cmd_dimensions_px.y = DESKTOP_WIN_RECT.bottom;
		MODIFIED_CMD_CHAR_SIZE_LP.y = static_cast<LONG>(CMD_CHAR_SIZE_LP.y / y_multiplier);
	}
	if (cmd_dimensions_px.x > DESKTOP_WIN_RECT.right)
	{
		const double x_multiplier = static_cast<double>(cmd_dimensions_px.x) / DESKTOP_WIN_RECT.right;
		cmd_dimensions_px.x = DESKTOP_WIN_RECT.right;
		const LONG temp_y = static_cast<LONG>(CMD_CHAR_SIZE_LP.y / x_multiplier);
		if (MODIFIED_CMD_CHAR_SIZE_LP.y > temp_y) { MODIFIED_CMD_CHAR_SIZE_LP.y = temp_y; }
	}
	CMD_CHAR_SIZE_LP = MODIFIED_CMD_CHAR_SIZE_LP;

	//Sets Back Pixels to Logical Coordinates
	DPtoLP(CMD_GRAPHICS_HANDLER, &CMD_CHAR_SIZE_LP, 1);
	//Sets Character Size to the new size
	CMD_CHAR_DATA.dwFontSize.X = static_cast<short>(CMD_CHAR_SIZE_LP.x);
	CMD_CHAR_DATA.dwFontSize.Y = static_cast<short>(CMD_CHAR_SIZE_LP.y);

	SetCurrentConsoleFontEx(CMD_OUTPUT_OBJ_HANDLE, FALSE, &CMD_CHAR_DATA);
	MoveWindow(CMD_HANDLE, DESKTOP_WIN_RECT.left, DESKTOP_WIN_RECT.top, cmd_dimensions_px.x, cmd_dimensions_px.y, TRUE);
	const Vector2<int> middle_pos{
		DESKTOP_WIN_RECT.right / 2 - cmd_dimensions_px.x / 2,
		DESKTOP_WIN_RECT.bottom / 2 - cmd_dimensions_px.y / 2
	};
	SetWindowPos(CMD_HANDLE, nullptr, middle_pos.x, middle_pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

template <SHORT Width, SHORT Height>
Window<Width, Height>::Window(const bool is_console_utf_, wchar_t custom_border_)
{
	//Initilizes the Console variables
	CMD_CHAR_DATA.cbSize = sizeof(CONSOLE_FONT_INFOEX); //Setting size of struct
	GetWindowRect(DESKTOP_WIN_HANDLE, &DESKTOP_WIN_RECT);
	GetCurrentConsoleFontEx(CMD_OUTPUT_OBJ_HANDLE, FALSE, &CMD_CHAR_DATA);

	if (is_console_utf_)
	{
		_setmode(_fileno(stdout), _O_U16TEXT);
		custom_border_ = WIN_BORDER;
	}
	init_console();
	draw_window_border();
}

template <SHORT Width, SHORT Height>
void Window<Width, Height>::clear()
{
	_win_data.data() = {};
}


template <SHORT Width, SHORT Height>
void Window<Width, Height>::fill(wchar_t character)
{
	std::fill_n(_win_data.data(), _win_data.max_size(), character);
}

template <SHORT Width, SHORT Height>
void Window<Width, Height>::fill(wchar_t character, Vector2<size_t> top_left, Vector2<size_t> bottom_right)
{
}

template <SHORT Width, SHORT Height>
void Window<Width, Height>::clear_cmd()
{
	constexpr COORD top_left = {0, 0};
	DWORD written;

	GetConsoleScreenBufferInfo(CMD_OUTPUT_OBJ_HANDLE, &CMD_BUFFER_DATA);
	FillConsoleOutputCharacterA(
		CMD_OUTPUT_OBJ_HANDLE, ' ', CMD_BUFFER_DATA.dwSize.X * CMD_BUFFER_DATA.dwSize.Y, top_left, &written
	);
	FillConsoleOutputAttribute(
		CMD_OUTPUT_OBJ_HANDLE, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		CMD_BUFFER_DATA.dwSize.X * CMD_BUFFER_DATA.dwSize.Y, top_left, &written
	);
	SetConsoleCursorPosition(CMD_OUTPUT_OBJ_HANDLE, top_left);
}

template <SHORT Width, SHORT Height>
void Window<Width, Height>::display()
{
}


#endif
