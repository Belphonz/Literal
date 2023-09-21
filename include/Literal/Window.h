#ifndef LITERAL_CLASS_WINDOW_H
#define LITERAL_CLASS_WINDOW_H

#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <iostream>

#include "Literal/Vector2.h"
#include "Arson/Matrix.h"

namespace Lit
{

	//Creates a window which can display drawables
	template <size_t Width, size_t Height>
	class Window
	{
		Arn::Matrix<char,Width,Height> _win_data{};
		constexpr static wchar_t win_border{L'\u2588'};

		//Set the Cursor Position on the Window
		static void set_cursor_position(Vector2<SHORT> position);

		//Holds the current view position of the top left char
		Vector2<int> _view_pos{};
	public:
		//Default Constructor
		//WARNING: Initlizing this class will draw a border in the console which may overwrite preexisting text
		explicit Window(bool is_console_utf_ = true, wchar_t custom_border_ = ' ');

		//Clears the Window
		void clear();
		//Display what has been drawn onto the Window
		void display();
	};
#include "Window_src.h"
}


#endif
