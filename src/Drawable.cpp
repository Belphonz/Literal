#include <fstream>

#include "Literal/Drawable.h"
#include <iostream>

Lit::Drawable::Drawable(Arn::Vector2<size_t> size, Arn::Vector2<float> posistion) :
	_size{ size },
	_pos{ posistion }
{
	_displayRect = { {0,0},{_size} };
}

Lit::Drawable::Drawable(const std::wstring& filepath, Arn::Vector2<size_t> size, Arn::Vector2<float> posistion) :
	_size{ size },
	_pos{ posistion }
{
	_displayRect = { {0,0},{_size} };
	SetDrawable(filepath);
}

Lit::Drawable::Drawable(const Drawable& deltaDrawable)
{
	_size = deltaDrawable._size;
	_pos = deltaDrawable._pos;
	_displayRect = deltaDrawable._displayRect;
	_drawData = deltaDrawable._drawData;
}

Lit::Drawable::Drawable(Drawable&& deltaDrawable) noexcept
{
	_size = deltaDrawable._size;
	_pos = deltaDrawable._pos;
	_displayRect = deltaDrawable._displayRect;
	_drawData = std::move(deltaDrawable._drawData);
}

Lit::Drawable& Lit::Drawable::operator=(const Drawable& deltaDrawable)
{
	if (this != &deltaDrawable)
	{
		_size = deltaDrawable._size;
		_pos = deltaDrawable._pos;
		_displayRect = deltaDrawable._displayRect;
		_drawData = deltaDrawable._drawData;
	}
	return *this;
}

Lit::Drawable& Lit::Drawable::operator=(Drawable&& deltaDrawable) noexcept
{
	_size = deltaDrawable._size;
	_pos = deltaDrawable._pos;
	_displayRect = deltaDrawable._displayRect;
	_drawData = std::move(deltaDrawable._drawData);

	return *this;
}

void Lit::Drawable::SetDrawable(Arn::Tensor2DVector<wchar_t> array2D)
{
	_drawData = array2D;
	_drawData.Resize(_size);
}

void Lit::Drawable::SetDrawable(const std::wstring& filepath)
{
	_drawData.Fill(L' ');
	std::wifstream in_stream{};
	in_stream.open(filepath);
	if (in_stream.is_open()) {
		for (size_t i{ 0 }; !in_stream.eof() && i < _size.y; ++i)
		{
			in_stream.getline(&_drawData.At({ 0,i }), _size.x);
		}
	}
	else {
		std::wcout << "Error opening file";
	}

	in_stream.close();
}

Arn::Vector2<float> Lit::Drawable::Pos() const
{
	return _pos;
}

void Lit::Drawable::SetPos(Arn::Vector2<float> new_pos)
{
	_pos = new_pos;
}

Arn::Vector2<size_t> Lit::Drawable::Size() const
{
	return _size;
}

Lit::Rect<size_t> Lit::Drawable::GetRect()
{
	return _displayRect;
}

void Lit::Drawable::SetDisplaySurface(Lit::Rect<size_t> new_draw_rect)
{
	_displayRect = new_draw_rect;
}

const Arn::Tensor2DVector<wchar_t>& Lit::Drawable::Data()
{
	return _drawData;
}

wchar_t* Lit::Drawable::RawData()
{
	return _drawData.Data();
}

void Lit::Drawable::Move(Arn::Vector2<float> offset)
{
	_pos = { _pos.x + offset.x,
			 _pos.y + offset.y };
}

void Lit::Drawable::Fill(wchar_t character)
{
	std::fill_n(_drawData.Data(), _drawData.MaxSize(), character);
}

void Lit::Drawable::Fill(wchar_t character,
	Arn::Vector2<size_t> top_left,
	Arn::Vector2<size_t> bottom_right)
{
	for (size_t height{ 0 }; height < bottom_right.y - top_left.y; ++height)
	{
		std::fill_n(&_drawData.At({ top_left.x, top_left.y + height }), bottom_right.x - top_left.x, character);
	}
}

void Lit::Drawable::Fill(wchar_t character, Lit::Rect<size_t> rect)
{
	for (size_t height{ 0 }; height < rect.height - rect.top; ++height)
	{
		std::fill_n(&_drawData.At({ rect.left, rect.top + height }), rect.width - rect.left, character);
	}
}