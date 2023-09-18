#pragma once
#ifndef LITERAL_WINDOW_H
#define LITERAL_WINDOW_H

#include "Literal/Vector2.h"
#include "Arson/Matrix.h"

namespace lit
{
	
	//Creates a window which can display drawables
	template <size_t Width, size_t Height>
	class Window
	{
		Arn::Matrix<char,Width,Height> _win_data{};
		constexpr static char lit_border{'M'};

		//Holds the current view position of the top left char
		Vector2<int> _view_pos{};

	public:
		//Default Constructor
		Window() = default;

		void clear();
	};
#include "Matrix_src.h"
}


#endif
