#ifndef LITERAL_CLASS_DRAWABALE_H
#define LITERAL_CLASS_DRAWABALE_H

#include <string>

#include <Arson/Arson.h>
#include <Literal/Rect.h>

namespace Lit
{
	//Object which holds an Array of characters which can be displayed onto the Window
	class Drawable
	{
	protected:
		Arn::Vector2<size_t> _size{};
		//Holds the rectangle which is displayed onto the Window
		Lit::Rect<size_t> _displayRect{};
		//Holds the current view position of the top left char
		Arn::Vector2<float> _pos{ 0, 0 };
		//Main Array storing the Drawable
		Arn::Tensor2DVector<wchar_t> _drawData{ {_size.x,_size.y},true };

		friend class Window;
	public:
		Drawable(Arn::Vector2<size_t> size, Arn::Vector2<float> posistion = { 0,0 });
		Drawable(const std::wstring& filepath,
			Arn::Vector2<size_t> size,
			Arn::Vector2<float> posistion = { 0,0 });
		//Copy Constructor
		Drawable(const Drawable& deltaDrawable);
		//Move Constructor
		Drawable(Drawable&& deltaDrawable) noexcept;
		//Copy assignment
		Drawable& operator=(const Drawable& deltaDrawable);
		//Move assignment
		Drawable& operator=(Drawable&& deltaDrawable) noexcept;

		//Sets a 2D array as the draw_data
		void SetDrawable(Arn::Tensor2DVector<wchar_t> array2D);
		//Sets the contents of a txt file as the draw_data
		void SetDrawable(const std::wstring& filepath);
		//Gets Drawables Position
		Arn::Vector2<float> Pos() const;
		//Sets Drawables Position
		void SetPos(Arn::Vector2<float> newPos);
		//Gets Drawables Size
		Arn::Vector2<size_t> Size() const;
		//Gets Drawables draw_rect
		//(Draw Rect is the surface which is drawn on the Window)
		//(By default this is set to the whole Drawable and is reset at every resize)
		Lit::Rect<size_t> GetRect();
		//Gets Drawables draw_rect
		//(Draw Rect is the surface which is drawn on the Window)
		//(By default this is set to the whole Drawable and is reset at every resize)
		void SetDisplaySurface(Lit::Rect<size_t> newDrawRect);
		//Get Raw Drawable Array
		const Arn::Tensor2DVector<wchar_t>& Data();
		//Get Primitive Raw Drawable Array
		wchar_t* RawData();

		//Displaces the position by offset
		void Move(Arn::Vector2<float> offset);
		//Fill the drawable with a char
		void Fill(wchar_t character);
		//Fill a part of the drawable with a char
		void Fill(wchar_t character, Arn::Vector2<size_t> topLeft, Arn::Vector2<size_t> bottomRight);
		//Fill a part of the drawable with a char
		void Fill(wchar_t character, Lit::Rect<size_t> rect);
	};
}

#endif
