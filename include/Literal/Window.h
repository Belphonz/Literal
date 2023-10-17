#ifndef LITERAL_CLASS_WINDOW_H
#define LITERAL_CLASS_WINDOW_H

#include <Windows.h>
#include <iostream>
#include <io.h>

#include "Literal/Vector2.h"
#include "Arson/Matrix.h"

namespace Lit
{
	//Creates a window which can display drawables
	template <SHORT Width = 1, SHORT Height = 1>
	class Window
	{
		Arn::Matrix<wchar_t, Width, Height> _win_data{};
		constexpr static wchar_t WIN_BORDER{L'\u2588'};

		//Draw the border
		void draw_window_border();
		//Changes the console size , pos and character size in proportion to the Width and Height Given
		void init_console();

		//Holds the current view position of the top left char
		Vector2<int> _view_pos{};

		//Window console variables
		const HWND DESKTOP_WIN_HANDLE{GetDesktopWindow()};
		const HWND CMD_HANDLE{GetConsoleWindow()};
		const HANDLE CMD_OUTPUT_OBJ_HANDLE{GetStdHandle(STD_OUTPUT_HANDLE)};
		RECT DESKTOP_WIN_RECT{};
		CONSOLE_FONT_INFOEX CMD_CHAR_DATA{};
		CONSOLE_SCREEN_BUFFER_INFO CMD_BUFFER_DATA{};

	public:
		//Default Constructor
		//WARNING: Initializing this class will draw a border in the console which may overwrite preexisting text
		explicit Window(bool is_console_utf_ = true, wchar_t custom_border_ = ' ');

		//Set the Cursor Position on the Window	(Has a corrector)
		void set_cursor_position_in_window(Vector2<SHORT> position) const;
		//Set the Cursor Position on the Console
		void set_cursor_position_abs(Vector2<SHORT> position) const;
		//Get the Cursor Position on the Window	(Has a corrector)
		Vector2<SHORT> get_cursor_position_in_window() ;
		//Get the Cursor Position on the Console
		Vector2<SHORT> get_cursor_position_abs() ;

		//Clears the Window
		void clear();
		//Fill the window with a char
		void fill(wchar_t character);
		//Fill a part of the window with a char
		void fill(wchar_t character, Vector2<size_t> top_left, Vector2<size_t> bottom_right);
		//Clear the console
		void clear_cmd();
		//Display what has been drawn onto the Window
		void display();
	};

#include "Window_src.h"
}


#endif
