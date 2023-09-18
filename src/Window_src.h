#ifndef LITERAL_CLASS_WINDOW_SCR_H
#define LITERAL_CLASS_WINDOW_SCR_H


template <size_t Width, size_t Height>
void Window<Width, Height>::clear()
{
	_win_data.data() = {};
}

#endif
