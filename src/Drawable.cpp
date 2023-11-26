#include <fstream>

#include "Literal/Drawable.h"
#include <iostream>

Lit::Drawable::Drawable(Arn::Vector2<size_t> size, Arn::Vector2<int> posistion) :
	_size{ size },
	_pos{ posistion }
{
	_display_rect = { {0,0},{_size} };
}

Lit::Drawable::Drawable(const std::string& filepath, Arn::Vector2<size_t> size, Arn::Vector2<int> posistion) :
	_size{ size },
	_pos{ posistion }
{
	_display_rect = { {0,0},{_size} };
	set_drawable(filepath);
}

Lit::Drawable::Drawable(const Drawable& delta_drawable)
{
	_size = delta_drawable._size;
	_pos = delta_drawable._pos;
	_display_rect = delta_drawable._display_rect;
	_draw_data = delta_drawable._draw_data;
}

Lit::Drawable::Drawable(Drawable&& delta_drawable) noexcept
{
	_size = delta_drawable._size;
	_pos = delta_drawable._pos;
	_display_rect = delta_drawable._display_rect;
	_draw_data = std::move(delta_drawable._draw_data);
}

Lit::Drawable& Lit::Drawable::operator=(const Drawable& delta_drawable)
{
	if (this != &delta_drawable)
	{
		_size = delta_drawable._size;
		_pos = delta_drawable._pos;
		_display_rect = delta_drawable._display_rect;
		_draw_data = delta_drawable._draw_data;
	}
	return *this;
}

Lit::Drawable& Lit::Drawable::operator=(Drawable&& delta_drawable) noexcept
{
	_size = delta_drawable._size;
	_pos = delta_drawable._pos;
	_display_rect = delta_drawable._display_rect;
	_draw_data = std::move(delta_drawable._draw_data);

	return *this;
}

void Lit::Drawable::set_drawable(Arn::Tensor2D<wchar_t> array2D)
{
	_draw_data = array2D;
	_draw_data.resize(_size);
}

void Lit::Drawable::set_drawable(const std::string& filepath)
{
	_draw_data.fill(L' ');
	std::wifstream in_stream{};
	in_stream.open(filepath);
	if (in_stream.is_open()) {
		for (size_t i{ 0 }; !in_stream.eof() && i < _size.y; ++i)
		{
			in_stream.getline(&_draw_data.at({ 0,i }), _size.x);
		}
	}
	else {
		std::wcout << "Error opening file";
	}

	in_stream.close();
}

Arn::Vector2<int> Lit::Drawable::position() const
{
	return _pos;
}

void Lit::Drawable::set_position(Arn::Vector2<int> new_pos)
{
	_pos = new_pos;
}

Arn::Vector2<size_t> Lit::Drawable::size() const
{
	return _size;
}

void Lit::Drawable::set_size(Arn::Vector2<size_t> new_size)
{
	_size = new_size;
	_display_rect = { {0,0},{_size.x,_size.y} };
	_draw_data.resize(_size);
}

Lit::Rect<size_t> Lit::Drawable::get_display_surface()
{
	return _display_rect;
}

void Lit::Drawable::set_display_surface(Lit::Rect<size_t> new_draw_rect)
{
	_display_rect = new_draw_rect;
}

const Arn::Tensor2D<wchar_t>& Lit::Drawable::data()
{
	return _draw_data;
}

wchar_t* Lit::Drawable::rawdata()
{
	return _draw_data.data();
}

void Lit::Drawable::move(Arn::Vector2<int> offset)
{
	_pos = { _pos.x + offset.x,
			 _pos.y + offset.y };
}

void Lit::Drawable::fill(wchar_t character)
{
	std::fill_n(_draw_data.data(), _draw_data.max_size(), character);
}

void Lit::Drawable::fill(wchar_t character,
	Arn::Vector2<size_t> top_left,
	Arn::Vector2<size_t> bottom_right)
{
	for (size_t height{ 0 }; height < bottom_right.y - top_left.y; ++height)
	{
		std::fill_n(&_draw_data.at({ top_left.x, top_left.y + height }), bottom_right.x - top_left.x, character);
	}
}

void Lit::Drawable::fill(wchar_t character, Lit::Rect<size_t> rect)
{
	for (size_t height{ 0 }; height < rect.height - rect.top; ++height)
	{
		std::fill_n(&_draw_data.at({ rect.left, rect.top + height }), rect.width - rect.left, character);
	}
}