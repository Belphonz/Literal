#include <fcntl.h>
#include <io.h>
#include <stdio.h>

#include <Literal/Window.h>

Arn::Vector2<size_t> Lit::Window::GetDimensions() const
{
	return { _width, _height };
}

void Lit::Window::SetCursorPos(Arn::Vector2<SHORT> position) const
{
	std::wcout.flush();
	//Correction for the screen border
	const COORD coordinates = { ++position.x, ++position.y };
	SetConsoleCursorPosition(CMD_OUTPUT_OBJ_HANDLE, coordinates);
}

void Lit::Window::SetCursorAbsPos(const Arn::Vector2<SHORT> position) const
{
	std::wcout.flush();
	const COORD coordinates = { position.x, position.y };
	SetConsoleCursorPosition(CMD_OUTPUT_OBJ_HANDLE, coordinates);
}

Arn::Vector2<SHORT> Lit::Window::GetCursorPos()
{
	std::wcout.flush();
	GetConsoleScreenBufferInfo(CMD_OUTPUT_OBJ_HANDLE, &CMD_BUFFER_DATA);

	return { --CMD_BUFFER_DATA.dwCursorPosition.X, --CMD_BUFFER_DATA.dwCursorPosition.Y };
}

Arn::Vector2<SHORT> Lit::Window::GetCursorAbsPos()
{
	std::wcout.flush();
	GetConsoleScreenBufferInfo(CMD_OUTPUT_OBJ_HANDLE, &CMD_BUFFER_DATA);

	return { CMD_BUFFER_DATA.dwCursorPosition.X, CMD_BUFFER_DATA.dwCursorPosition.Y };
}

const Arn::Tensor2DVector<wchar_t>& Lit::Window::Data()
{
	return _winData;
}

wchar_t* Lit::Window::RawData()
{
	return _winData.Data();
}

void Lit::Window::SetSettings(WindowSettings newConfiguration)
{
	_currentSettings = newConfiguration;
	SetWindowLongPtr(GetConsoleWindow(), GWL_STYLE, newConfiguration.WIN_STYLE);
	SetConsoleTitleW(newConfiguration.WIN_TITLE);
}

const Lit::WindowSettings& Lit::Window::GetSettings()	const
{
	return _currentSettings;
}

void Lit::Window::SetViewPos(Arn::Vector2<float> newViewPos)
{
	_viewPos = newViewPos;
}

Arn::Vector2<float> Lit::Window::GetViewPos() const
{
	return _viewPos;
}

void Lit::Window::_DrawWindowBorder(const wchar_t border)
{
	SetCursorAbsPos({ 0, 0 });
	for (size_t i{ 0 }; i <= _width + 1; ++i)
	{
		std::fwrite(&border, sizeof(wchar_t), 1, stdout);
	}
	std::fwrite(L"\n", sizeof(wchar_t), 1, stdout);

	for (size_t i{ 0 }; i <= 2 * _height; ++i)
	{
		//Print the Border on One side than jump to the Other Side
		if (i % 2 == 0)
		{
			std::fwrite(&border, sizeof(wchar_t), 1, stdout);
			SetCursorAbsPos({ static_cast<short>(_width + 1), GetCursorAbsPos().y });
		}
		else
		{
			std::fwrite(&border, sizeof(wchar_t), 1, stdout);
			std::fwrite(L"\n", sizeof(wchar_t), 1, stdout);
		}
	}
	SetCursorAbsPos({ 0, GetCursorAbsPos().y });

	for (size_t i{ 0 }; i <= _width + 1; ++i)
	{
		std::fwrite(&border, sizeof(wchar_t), 1, stdout);
	}
	std::fwrite(L"\n", sizeof(wchar_t), 1, stdout);
}

void Lit::Window::_InitConsole()
{
	//Corrector Exists so that 1x1 squares can work correctly
	constexpr int size_corrector{ 7 };
	Arn::Vector2<int> cmd_dimensions_px = {
		static_cast<int>(_width + size_corrector) * (CMD_CHAR_DATA.dwFontSize.Y / 2 + 1),
		static_cast<int>(_height + size_corrector) * CMD_CHAR_DATA.dwFontSize.Y
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

Lit::Window::Window(Arn::Vector2<size_t> dimensions, WindowSettings configuragation, bool isConsolUtf) :
	_width{ dimensions.x }, _height{ dimensions.y }
{
	//Sets the Console Style
	SetWindowLongPtr(CMD_HANDLE, GWL_STYLE, configuragation.WIN_STYLE);
	SetConsoleTitleW(configuragation.WIN_TITLE);
	//Initializes the Console variables
	CMD_CHAR_DATA.cbSize = sizeof(CONSOLE_FONT_INFOEX); //Setting size of struct
	GetWindowRect(DESKTOP_WIN_HANDLE, &DESKTOP_WIN_RECT);
	GetCurrentConsoleFontEx(CMD_OUTPUT_OBJ_HANDLE, FALSE, &CMD_CHAR_DATA);

	std::fill_n(_winData.Data(), _winData.MaxSize(), ' ');
	if (isConsolUtf)
	{
		_setmode(_fileno(stdout), _O_U16TEXT);
	}
	_currentSettings = configuragation;
	_InitConsole();
	_DrawWindowBorder(_currentSettings.WIN_BORDER);
}

void Lit::Window::SetFramerateLimit(const size_t limit)
{
	_fpsCap = limit;
}

void Lit::Window::Clear()
{
	std::fill_n(_winData.Data(), _winData.MaxSize() / 2, ' ');
}

void Lit::Window::Fill(const wchar_t character)
{
	std::fill_n(_winData.Data(), _winData.MaxSize() / 2, character);
}

void Lit::Window::Fill(const wchar_t character,
	const Arn::Vector2<size_t> topLeft,
	const Arn::Vector2<size_t> bottomRight)
{
	for (size_t height{ 0 }; height < bottomRight.y - topLeft.y; ++height)
	{
		std::fill_n(&_winData.At({ topLeft.x, topLeft.y + height }), bottomRight.x - topLeft.x, character);
	}
}

void Lit::Window::Fill(wchar_t character, Lit::Rect<size_t> rect)
{
	for (size_t height{ 0 }; height < rect.height - rect.top; ++height)
	{
		std::fill_n(&_winData.At({ rect.left, rect.top + height }), rect.width - rect.left, character);
	}
}

void Lit::Window::ClearCMD()
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

void Lit::Window::MoveView(Arn::Vector2<float> offset)
{
	_viewPos = { _viewPos.x + offset.x,
				  _viewPos.y + offset.y };
}

void Lit::Window::Draw(Drawable& drawable)
{
	using namespace Arn;

	Arn::Vector2<int> drawablePos{ TCast<int>(floor(drawable._pos.x)),TCast<int>(floor(drawable._pos.y)) };
	Arn::Vector2<int> viewPos{ TCast<int>(floor(_viewPos.x)),TCast<int>(floor(_viewPos.y)) };
	if (drawablePos.x >= viewPos.x + TCast<int>(_width)
		|| drawablePos.y >= viewPos.y + TCast<int>(_height)
		|| drawablePos.x + TCast<int>(drawable._displayRect.width) <= viewPos.x
		|| drawablePos.y + TCast<int>(drawable._displayRect.height) <= viewPos.y)
	{
		return;
	}

	size_t newSizeY{
		drawablePos.y + TCast<int>(drawable._displayRect.height) > drawablePos.y + TCast<int>(_height) ?
		(viewPos.y + TCast<int>(_height)) - drawablePos.y :
		drawable._displayRect.height };

	size_t newSizeX{
	   drawable._pos.x + TCast<int>(drawable._displayRect.width) > viewPos.x + TCast<int>(_width) ?
	   (viewPos.x + TCast<int>(_width)) - drawablePos.x :
	   drawable._displayRect.width };

	size_t beginDrawingX{
		viewPos.x > drawablePos.x ?
		TCast<size_t>(viewPos.x) - drawablePos.x :
		0 };

	size_t beginDrawingY{
		viewPos.y > drawablePos.y ?
		TCast<size_t>(viewPos.y) - drawablePos.y :
		0 };

	size_t posAbsX{ viewPos.x > drawablePos.x ?
		0 :
		TCast<size_t>(drawablePos.x) - viewPos.x };

	size_t posAbsY{ viewPos.y > drawablePos.y ?
		0 :
		TCast<size_t>(drawablePos.y) - viewPos.y };

	for (size_t y{ beginDrawingY }, index{ 0 }; y < newSizeY; ++y, ++index) {
		// x2 because memcpy copies in bytes and wchar is 2 bytes
		std::memcpy(
			&_winData.At({ posAbsX, posAbsY + index }),
			&drawable._drawData.At({ drawable._displayRect.left + beginDrawingX ,y }),
			(newSizeX - beginDrawingX) * sizeof(wchar_t));
	}
}

void Lit::Window::Display()
{
	const float current_time = _internalClock.GetElapsedTime().Seconds();
	const float fps_frequency = 1 / Arn::TCast<float>(_fpsCap);
	const float pure_duration_milliseconds = fps_frequency - current_time;
	const float pure_duration = pure_duration_milliseconds * 1000;
	const int duration = static_cast<int>(pure_duration);
	const DWORD duration_dword = duration;

	if (_fpsCap && current_time < fps_frequency)
	{
		return;
	}

	for (size_t y{ 0 }; y < _height; ++y)
	{
		for (size_t x{ 0 }; x < _width; ++x)
		{
			//Treating latter half of the array as a 2nd buffer, to reduce print calls
			if (_winData.At({ x, y }) == _winData.At({ x, _height + y }))
			{
				continue;
			}
			SetCursorPos({ static_cast<short>(x), static_cast<short>(y) });
			std::fwrite(&_winData.At({ x, y }), sizeof(wchar_t), 1, stdout);
		}
	}
	std::wmemmove(&_winData.At({ 0, _height }), &_winData.At({ 0, 0 }), _winData.MaxSize() / 2);
	SetCursorAbsPos({ 0, static_cast<short>(_height + 3) });
	_internalClock.Restart();
}