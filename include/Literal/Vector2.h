#pragma once
#ifndef LITERAL_VECTOR2_H
#define LITERAL_VECTOR2_H

#include <ostream>

namespace lit
{
	//General Vector2 struct
	template <typename T>
	struct Vector2
	{
		T x{};
		T y{};

		//Allows the Vector2 class to be outputted in the format : x: {x} , y: {y}
		template <typename T>
		friend std::ostream& operator<<(std::ostream& out, const Vector2<T>& vector);
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& out, const Vector2<T>& vector)
	{
		out << "x: " << vector.x << " , y: " << vector.y;

		return out;
	}
}


#endif
