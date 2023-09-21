#ifndef LITERAL_CLASS_WINDOW_SCR_H
#define LITERAL_CLASS_WINDOW_SCR_H


template <size_t Width, size_t Height>
void Window<Width, Height>::set_cursor_position(Vector2<SHORT> position)
{
	static const HANDLE h_out{GetStdHandle(STD_OUTPUT_HANDLE)};
	std::cout.flush();
	//Correction for the screen border 
	const COORD coordinates = {++position.x, ++position.y};
	SetConsoleCursorPosition(h_out, coordinates);
}

template <size_t Width, size_t Height>
Window<Width, Height>::Window(const bool is_console_utf_, wchar_t custom_border_)
{
	if (is_console_utf_)
	{
		_setmode(_fileno(stdout), _O_U16TEXT);
		custom_border_ = win_border;

	}

	set_cursor_position({-1, -1});
	for (size_t i{0}; i <= Height + 1; ++i)
	{
		for (size_t j{0}; j <= Width + 1; ++j)
		{
			if (i == 0 || i == Height + 1
				|| j == 0 || j == Width + 1)
			{
				wprintf(L"%c", custom_border_);
				continue;
			}
			wprintf(L" ");
		}
		wprintf(L"\n");
	}
}

template <size_t Width, size_t Height>
void Window<Width, Height>::clear()
{
	_win_data.data() = {};
}

template <size_t Width, size_t Height>
void Window<Width, Height>::display()
{
}


#endif
