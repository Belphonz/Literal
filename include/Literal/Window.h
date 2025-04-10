#ifndef LITERAL_CLASS_WINDOW_H
#define LITERAL_CLASS_WINDOW_H

#include <iostream>
#include <Windows.h>

#include <Arson/Structure.h>
#include <Literal/Drawable.h>
#include <Literal/Timer.h>
#include <Literal/Rect.h>
#include <Literal/WindowSettings.h>

namespace Lit
{
	//Creates a window which can display drawables
	class Window
	{
	protected:
		size_t _width{};
		size_t _height{};

		//Main Array storing the current frame
		//To create a double buffer system array is double size
		Arn::Tensor2DVector<wchar_t> _winData{ {_width, _height * 2},TRUE };
		WindowSettings _currentSettings{};

		size_t _fpsCap{ 0 };
		Timer _internalClock{};
		//Draw the border
		void _DrawWindowBorder(wchar_t border);
		//Changes the console size , pos and character size in proportion to the Width and Height Given
		void _InitConsole();

		//Holds the current view position of the top left char
		Arn::Vector2<float> _viewPos{ 0, 0 };

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
		explicit Window(Arn::Vector2<size_t> dimensions, WindowSettings configuragation = WindowSettings(), bool isConsoleUtf_ = true);

		//Set the Frame Rate limit for the Window
		//If Frame-rate is set to 0, then its set to uncapped
		void SetFramerateLimit(size_t limit);

		//Returns window dimensions
		[[nodiscard]] Arn::Vector2<size_t> GetDimensions() const;
		//Set the Cursor Position on the Window	(Has a corrector)
		void SetCursorPos(Arn::Vector2<SHORT> position) const;
		//Set the Cursor Position on the Console
		void SetCursorAbsPos(Arn::Vector2<SHORT> position) const;
		//Get the Cursor Position on the Window	(Has a corrector)
		Arn::Vector2<SHORT> GetCursorPos();
		//Get the Cursor Position on the Console
		Arn::Vector2<SHORT> GetCursorAbsPos();
		//Get Raw Window Array
		const Arn::Tensor2DVector<wchar_t>& Data();
		//Get Primitive Raw Window Array
		wchar_t* RawData();
		//Get WindowSettings
		void SetSettings(WindowSettings new_configuration);
		//Get WindowSettings
		const WindowSettings& GetSettings() const;
		//Sets a new View position (camera)
		void SetViewPos(Arn::Vector2<float> new_view_pos);
		//Gets View position (camera)
		Arn::Vector2<float> GetViewPos() const;

		//Clears the Window
		void Clear();
		//Fill the window with a char
		void Fill(wchar_t character);
		//Fill a part of the window with a char
		void Fill(wchar_t character, Arn::Vector2<size_t> top_left, Arn::Vector2<size_t> bottom_right);
		//Fill a part of the window with a char
		void Fill(wchar_t character, Lit::Rect<size_t> rect);
		//Clear the console
		void ClearCMD();
		//Displaces the View position (camera) by offset
		void MoveView(Arn::Vector2<float> offset);
		//Draws a drawable onto the Window
		void Draw(Drawable& drawable);
		//Display what has been drawn onto the Window
		void Display();
	};
}

#endif
