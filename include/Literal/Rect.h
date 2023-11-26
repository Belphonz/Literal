#ifndef LITERAL_STRUCT_WINDOW_H
#define LITERAL_STRUCT_WINDOW_H

#include "Arson/Struct_Common.h"

namespace Lit {
	//Object which holds a 2D rectangle structure
	template<typename T>
	struct Rect
	{
		T left{};
		T top{};
		T width{};
		T height{};

		Rect() = default;
		Rect(const Arn::Vector2<T>& position, const Arn::Vector2<T>& size) {
			left = position.x;
			top = position.y;
			width = size.x;
			height = size.y;
		}

		Arn::Vector2<T> topleft() const {
			return { left,top };
		}
		Arn::Vector2<T> position() const {
			return { left,top };
		}
		Arn::Vector2<T> bottomright() const {
			return { width,height };
		}
		Arn::Vector2<T> size() const {
			return { width,height };
		}
	};
}
#endif