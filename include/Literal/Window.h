#ifndef LITERAL_CLASS_WINDOW_H
#define LITERAL_CLASS_WINDOW_H

#include <Windows.h>
#include <iostream>
#include <io.h>

#include "Arson/Matrix.h"
#include "Literal/Vector2.h"
#include "Literal/Timer.h"

namespace Lit
{
	//Creates a window which can display drawables
	template <size_t Width = 1, size_t Height = 1>
	class Window
	{
		//Main Array storing the current frame
		//To create a double buffer system array is double size 
		Arn::Matrix<wchar_t, Width, 2 * Height> _win_data{};
		constexpr static wchar_t WIN_BORDER{L'\u2588'};

		size_t _fps_limit{0};
		Timer  _internal_clock{};
		//Draw the border
		void draw_window_border(wchar_t border);
		//Changes the console size , pos and character size in proportion to the Width and Height Given
		void init_console();

		//Holds the current view position of the top left char
		Vector2<int> _view_pos{0, 0};

		//Window console variables
		inline static const HWND DESKTOP_WIN_HANDLE{GetDesktopWindow()};
		inline static const HWND CMD_HANDLE{GetConsoleWindow()};
		inline static const HANDLE CMD_OUTPUT_OBJ_HANDLE{GetStdHandle(STD_OUTPUT_HANDLE)};
		inline static RECT DESKTOP_WIN_RECT{};
		inline static CONSOLE_FONT_INFOEX CMD_CHAR_DATA{};
		inline static CONSOLE_SCREEN_BUFFER_INFO CMD_BUFFER_DATA{};

	public:
		//Default Constructor
		//WARNING: Initializing this class will draw a border in the console which may overwrite preexisting text
		explicit Window(bool is_console_utf_ = true, wchar_t custom_border_ = ' ');

		//Set the Frame Rate limit for the Window
		void set_framerate_limit(size_t limit);

		//Returns window dimensions
		static Vector2<size_t> get_dimensions();
		//Set the Cursor Position on the Window	(Has a corrector)
		void set_cursor_position_in_window(Vector2<SHORT> position) const;
		//Set the Cursor Position on the Console
		void set_cursor_position_abs(Vector2<SHORT> position) const;
		//Get the Cursor Position on the Window	(Has a corrector)
		Vector2<SHORT> get_cursor_position_in_window();
		//Get the Cursor Position on the Console
		Vector2<SHORT> get_cursor_position_abs();
		//Get Raw Window Array
		char* data();

		//Clears the Window
		void clear();
		//Fill the window with a char
		void fill(wchar_t character);
		//Fill a part of the window with a char
		void fill(wchar_t character, Vector2<size_t> top_left, Vector2<size_t> bottom_right);
		//Clear the console
		static void clear_cmd();
		//Display what has been drawn onto the Window
		void display();
	};

#include "Window_src.h"
}


#endif
