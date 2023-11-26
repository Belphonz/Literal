#ifndef LITERAL_CLASS_WINDOW_H
#define LITERAL_CLASS_WINDOW_H

#include <iostream>
#include <Windows.h>

#include "Arson/Struct_Common.h"
#include "Arson/Tensor2D.h"
#include "Literal/Timer.h"
#include "Literal/Rect.h"
#include "Literal/WindowSettings.h"

namespace Lit
{
	//Creates a window which can display drawables
	class Window
	{
	protected:
		size_t WIDTH{};
		size_t HEIGHT{};

		//Main Array storing the current frame
		//To create a double buffer system array is double size
		Arn::Tensor2D<wchar_t> _win_data{ {WIDTH, HEIGHT * 2},TRUE };
		WindowSettings _current_settings{};

		size_t _fps_limit{ 0 };
		Timer _internal_clock{};
		//Draw the border
		void draw_window_border(wchar_t border);
		//Changes the console size , pos and character size in proportion to the Width and Height Given
		void init_console();

		//Holds the current view position of the top left char
		Arn::Vector2<int> _view_pos{ 0, 0 };

	private:
		//Window console variables

		const HWND DESKTOP_WIN_HANDLE{ GetDesktopWindow() };
		const HWND CMD_HANDLE{ GetConsoleWindow() };
		const HANDLE CMD_OUTPUT_OBJ_HANDLE{ GetStdHandle(STD_OUTPUT_HANDLE) };
		RECT DESKTOP_WIN_RECT{};
		CONSOLE_FONT_INFOEX CMD_CHAR_DATA{};
		CONSOLE_SCREEN_BUFFER_INFO CMD_BUFFER_DATA{};

	public:
		//Default Constructor
		//WARNING: Initializing this class will draw a border in the console which may overwrite preexisting text
		explicit Window(Arn::Vector2<size_t> dimensions, WindowSettings configuragation = WindowSettings(), bool is_console_utf_ = true);

		//Set the Frame Rate limit for the Window
		//If Frame-rate is set to 0, then its set to uncapped
		void set_framerate_limit(size_t limit);

		//Returns window dimensions
		[[nodiscard]] Arn::Vector2<size_t> get_dimensions() const;
		//Set the Cursor Position on the Window	(Has a corrector)
		void set_cursor_position_in_window(Arn::Vector2<SHORT> position) const;
		//Set the Cursor Position on the Console
		void set_cursor_position_abs(Arn::Vector2<SHORT> position) const;
		//Get the Cursor Position on the Window	(Has a corrector)
		Arn::Vector2<SHORT> get_cursor_position_in_window();
		//Get the Cursor Position on the Console
		Arn::Vector2<SHORT> get_cursor_position_abs();
		//Get Raw Window Array
		const Arn::Tensor2D<wchar_t>& data();
		//Get Primitive Raw Window Array
		wchar_t* rawdata();
		//Get WindowSettings
		void set_settings(WindowSettings new_configuration);
		//Get WindowSettings
		const WindowSettings& get_settings() const;
		//Sets a new View position (camera)
		void set_view_position(Arn::Vector2<int> new_view_pos);
		//Gets View position (camera)
		Arn::Vector2<int> get_view_position() const;

		//Clears the Window
		void clear();
		//Fill the window with a char
		void fill(wchar_t character);
		//Fill a part of the window with a char
		void fill(wchar_t character, Arn::Vector2<size_t> top_left, Arn::Vector2<size_t> bottom_right);
		//Fill a part of the window with a char
		void fill(wchar_t character, Lit::Rect<size_t> rect);
		//Clear the console
		void clear_cmd();
		//Displaces the View position (camera) by offset
		void move(Arn::Vector2<int> offset);
		//Display what has been drawn onto the Window
		void display();
	};
}

#endif
