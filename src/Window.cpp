#include <fcntl.h>
#include <io.h>

#include "Literal/Window.h"

Arn::Vector2<size_t> Lit::Window::get_dimensions() const
{
	return { WIDTH, HEIGHT };
}

void Lit::Window::set_cursor_position_in_window(Arn::Vector2<SHORT> position) const
{
	std::wcout.flush();
	//Correction for the screen border
	const COORD coordinates = { ++position.x, ++position.y };
	SetConsoleCursorPosition(CMD_OUTPUT_OBJ_HANDLE, coordinates);
}

void Lit::Window::set_cursor_position_abs(const Arn::Vector2<SHORT> position) const
{
	std::wcout.flush();
	const COORD coordinates = { position.x, position.y };
	SetConsoleCursorPosition(CMD_OUTPUT_OBJ_HANDLE, coordinates);
}

Arn::Vector2<SHORT> Lit::Window::get_cursor_position_in_window()
{
	std::wcout.flush();
	GetConsoleScreenBufferInfo(CMD_OUTPUT_OBJ_HANDLE, &CMD_BUFFER_DATA);

	return { --CMD_BUFFER_DATA.dwCursorPosition.X, --CMD_BUFFER_DATA.dwCursorPosition.Y };
}

Arn::Vector2<SHORT> Lit::Window::get_cursor_position_abs()
{
	std::wcout.flush();
	GetConsoleScreenBufferInfo(CMD_OUTPUT_OBJ_HANDLE, &CMD_BUFFER_DATA);

	return { CMD_BUFFER_DATA.dwCursorPosition.X, CMD_BUFFER_DATA.dwCursorPosition.Y };
}

const Arn::Tensor2D<wchar_t>& Lit::Window::data()
{
	return _win_data;
}

wchar_t* Lit::Window::rawdata()
{
	return _win_data.data();
}

void Lit::Window::set_settings(WindowSettings new_configuration)
{
	_current_settings = new_configuration;
	SetWindowLongPtr(GetConsoleWindow(), GWL_STYLE, new_configuration.WIN_STYLE);
	SetConsoleTitleW(new_configuration.WIN_TITLE);
}

const Lit::WindowSettings& Lit::Window::get_settings()	const
{
	return _current_settings;
}

void Lit::Window::set_view_position(Arn::Vector2<float> new_view_pos)
{
	_view_pos = new_view_pos;
}

Arn::Vector2<float> Lit::Window::get_view_position() const
{
	return _view_pos;
}

void Lit::Window::draw_window_border(const wchar_t border)
{
	set_cursor_position_abs({ 0, 0 });
	for (size_t index{ 0 }; index <= WIDTH + 1; ++index)
	{
		fwrite(&border, sizeof(wchar_t), 1, stdout);
	}
	fwrite(L"\n", sizeof(wchar_t), 1, stdout);
	for (size_t delta_index{ 0 }; delta_index <= 2 * HEIGHT; ++delta_index)
	{
		//Print the Border on One side than jump to the Other Side
		if (delta_index % 2 == 0)
		{
			fwrite(&border, sizeof(wchar_t), 1, stdout);
			set_cursor_position_abs({ static_cast<short>(WIDTH + 1), get_cursor_position_abs().y });
		}
		else
		{
			fwrite(&border, sizeof(wchar_t), 1, stdout);
			fwrite(L"\n", sizeof(wchar_t), 1, stdout);
		}
	}
	set_cursor_position_abs({ 0, get_cursor_position_abs().y });
	for (size_t index{ 0 }; index <= WIDTH + 1; ++index)
	{
		fwrite(&border, sizeof(wchar_t), 1, stdout);
	}
	fwrite(L"\n", sizeof(wchar_t), 1, stdout);
}

void Lit::Window::init_console()
{
	//Corrector Exists so that 1x1 squares can work correctly
	constexpr int size_corrector{ 7 };
	Arn::Vector2<int> cmd_dimensions_px = {
		static_cast<int>(WIDTH + size_corrector) * (CMD_CHAR_DATA.dwFontSize.Y / 2 + 1),
		static_cast<int>(HEIGHT + size_corrector) * CMD_CHAR_DATA.dwFontSize.Y
	};

	const HDC CMD_GRAPHICS_HANDLER = GetDC(CMD_HANDLE);
	//LP == LOGICAL COORDINATES	 , DP == PIXELS
	POINT CMD_CHAR_SIZE_LP = { CMD_CHAR_DATA.dwFontSize.X, CMD_CHAR_DATA.dwFontSize.Y };
	LPtoDP(CMD_GRAPHICS_HANDLER, &CMD_CHAR_SIZE_LP, 1);

	POINT MODIFIED_CMD_CHAR_SIZE_LP = { CMD_CHAR_SIZE_LP };
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
	const Arn::Vector2<int> middle_pos{
		DESKTOP_WIN_RECT.right / 2 - cmd_dimensions_px.x / 2,
		DESKTOP_WIN_RECT.bottom / 2 - cmd_dimensions_px.y / 2
	};
	SetWindowPos(CMD_HANDLE, nullptr, middle_pos.x, middle_pos.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

Lit::Window::Window(Arn::Vector2<size_t> dimensions, WindowSettings configuragation, bool is_console_utf_) :
	WIDTH{ dimensions.x }, HEIGHT{ dimensions.y }
{
	//Sets the Console Style
	SetWindowLongPtr(GetConsoleWindow(), GWL_STYLE, configuragation.WIN_STYLE);
	SetConsoleTitleW(configuragation.WIN_TITLE);
	//Initializes the Console variables
	CMD_CHAR_DATA.cbSize = sizeof(CONSOLE_FONT_INFOEX); //Setting size of struct
	GetWindowRect(DESKTOP_WIN_HANDLE, &DESKTOP_WIN_RECT);
	GetCurrentConsoleFontEx(CMD_OUTPUT_OBJ_HANDLE, FALSE, &CMD_CHAR_DATA);

	std::fill_n(_win_data.data(), _win_data.max_size(), ' ');
	if (is_console_utf_)
	{
		_setmode(_fileno(stdout), _O_U16TEXT);
	}
	_current_settings = configuragation;
	init_console();
	draw_window_border(_current_settings.WIN_BORDER);
}

void Lit::Window::set_framerate_limit(const size_t limit)
{
	_fps_limit = limit;
}

void Lit::Window::clear()
{
	std::fill_n(_win_data.data(), _win_data.max_size() / 2, ' ');
}

void Lit::Window::fill(const wchar_t character)
{
	std::fill_n(_win_data.data(), _win_data.max_size() / 2, character);
}

void Lit::Window::fill(const wchar_t character,
	const Arn::Vector2<size_t> top_left,
	const Arn::Vector2<size_t> bottom_right)
{
	for (size_t height{ 0 }; height < bottom_right.y - top_left.y; ++height)
	{
		std::fill_n(&_win_data.at({ top_left.x, top_left.y + height }), bottom_right.x - top_left.x, character);
	}
}

void Lit::Window::fill(wchar_t character, Lit::Rect<size_t> rect)
{
	for (size_t height{ 0 }; height < rect.height - rect.top; ++height)
	{
		std::fill_n(&_win_data.at({ rect.left, rect.top + height }), rect.width - rect.left, character);
	}
}

void Lit::Window::clear_cmd()
{
	constexpr COORD top_left = { 0, 0 };
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

void Lit::Window::move(Arn::Vector2<float> offset)
{
	_view_pos = { _view_pos.x + offset.x,
				  _view_pos.y + offset.y };
}

void Lit::Window::draw(Drawable& drawable)
{
	Arn::Vector2<int> cast_drawable_pos{ floor(drawable._pos.x),floor(drawable._pos.y) };
	Arn::Vector2<int> cast_view_pos{ floor(_view_pos.x),floor(_view_pos.y) };
	if (cast_drawable_pos.x >= cast_view_pos.x + static_cast<int>(WIDTH)
		|| cast_drawable_pos.y >= cast_view_pos.y + static_cast<int>(HEIGHT)
		|| cast_drawable_pos.x + static_cast<int>(drawable._display_rect.width) <= cast_view_pos.x
		|| cast_drawable_pos.y + static_cast<int>(drawable._display_rect.height) <= cast_view_pos.y)
	{
		return;
	}

	size_t new_size_y{
		cast_drawable_pos.y + static_cast<int>(drawable._display_rect.height) > cast_drawable_pos.y + static_cast<int>(HEIGHT) ?
		(cast_view_pos.y + static_cast<int>(HEIGHT)) - cast_drawable_pos.y :
		drawable._display_rect.height };

	size_t new_size_x{
	   drawable._pos.x + static_cast<int>(drawable._display_rect.width) > cast_view_pos.x + static_cast<int>(WIDTH) ?
	   (cast_view_pos.x + static_cast<int>(WIDTH)) - cast_drawable_pos.x :
	   drawable._display_rect.width };

	size_t begin_drawing_x{
		cast_view_pos.x > cast_drawable_pos.x ?
		static_cast<size_t>(cast_view_pos.x) - cast_drawable_pos.x :
		0 };

	size_t begin_drawing_y{
		cast_view_pos.y > cast_drawable_pos.y ?
		static_cast<size_t>(cast_view_pos.y) - cast_drawable_pos.y :
		0 };

	size_t abs_pos_x{ cast_view_pos.x > cast_drawable_pos.x ?
		0 :
		static_cast<size_t>(cast_drawable_pos.x) - cast_view_pos.x };

	size_t abs_pos_y{ cast_view_pos.y > cast_drawable_pos.y ?
		0 :
		static_cast<size_t>(cast_drawable_pos.y) - cast_view_pos.y };

	for (size_t y{ begin_drawing_y }, index{ 0 }; y < new_size_y; ++y, ++index) {
		// x2 because memcpy copies in bytes and wchar is 2 bytes
		std::memcpy(
			&_win_data.at({ abs_pos_x, abs_pos_y + index }),
			&drawable._draw_data.at({ drawable._display_rect.left + begin_drawing_x ,y }),
			(new_size_x - begin_drawing_x) * sizeof(wchar_t));
	}
}

void Lit::Window::display()
{
	const float current_time = _internal_clock.get_elapsed_time().as_seconds();
	const float fps_frequency = 1 / static_cast<float>(_fps_limit);
	const float pure_duration_milliseconds = fps_frequency - current_time;
	const float pure_duration = pure_duration_milliseconds * 1000;
	const int duration = static_cast<int>(pure_duration);
	const DWORD duration_dword = duration;

	if (_fps_limit && current_time < fps_frequency)
	{
		Sleep(duration_dword);
	}
	for (size_t y{ 0 }; y < HEIGHT; ++y)
	{
		for (size_t x{ 0 }; x < WIDTH; ++x)
		{
			//Treating latter half of the array as a 2nd buffer, to reduce print calls
			if (_win_data.at({ x, y }) == _win_data.at({ x, HEIGHT + y }))
			{
				continue;
			}
			set_cursor_position_in_window({ static_cast<short>(x), static_cast<short>(y) });
			fwrite(&_win_data.at({ x, y }), sizeof(wchar_t), 1, stdout);
		}
	}
	std::wmemmove(&_win_data.at({ 0, HEIGHT }), &_win_data.at({ 0, 0 }), _win_data.max_size() / 2);
	set_cursor_position_abs({ 0, static_cast<short>(HEIGHT + 3) });
	_internal_clock.restart();
}