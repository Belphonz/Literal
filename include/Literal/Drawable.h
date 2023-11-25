#ifndef LITERAL_CLASS_DRAWABALE_H
#define LITERAL_CLASS_DRAWABALE_H

#include "Arson/Tensor2D.h"
#include "Arson/Struct_Common.h"

namespace Lit
{
	//Object which holds an Array of characters which can be displayed onto the Window
	class Drawable
	{
		Arn::Vector2<size_t> _size{};
		Arn::Tensor2D<wchar_t> _draw_data{ {_size.x,_size.y} };

		//Holds the current view position of the top left char
		Arn::Vector2<int> _pos{ 0, 0 };
	};
}

#endif
