#ifndef LITERAL_CLASS_DRAWABALE_H
#define LITERAL_CLASS_DRAWABALE_H

#include <string>

#include "Arson/Tensor2D.h"
#include "Arson/Struct_Common.h"
#include "Literal/Rect.h"

namespace Lit
{
	//Object which holds an Array of characters which can be displayed onto the Window
	class Drawable
	{
	protected:
		Arn::Vector2<size_t> _size{};
		//Holds the rectangle which is displayed onto the Window
		Lit::Rect<size_t> _display_rect{};
		//Holds the current view position of the top left char
		Arn::Vector2<float> _pos{ 0, 0 };
		//Main Array storing the Drawable
		Arn::Tensor2D<wchar_t> _draw_data{ {_size.x,_size.y} };

		friend class Window;
	public:
		Drawable(Arn::Vector2<size_t> size, Arn::Vector2<float> posistion = { 0,0 });
		Drawable(const std::string& filepath,
			Arn::Vector2<size_t> size,
			Arn::Vector2<float> posistion = { 0,0 });
		//Copy Constructor
		Drawable(const Drawable& delta_drawable);
		//Move Constructor
		Drawable(Drawable&& delta_drawable) noexcept;
		//Copy assignment
		Drawable& operator=(const Drawable& delta_drawable);
		//Move assignment
		Drawable& operator=(Drawable&& delta_drawable) noexcept;

		//Sets a 2D array as the draw_data
		void set_drawable(Arn::Tensor2D<wchar_t> array2D);
		//Sets the contents of a txt file as the draw_data
		void set_drawable(const std::string& filepath);
		//Gets Drawables Position
		Arn::Vector2<float> position() const;
		//Sets Drawables Position
		void set_position(Arn::Vector2<float> new_pos);
		//Gets Drawables Size
		Arn::Vector2<size_t> size() const;
		//Sets Drawables Size
		void set_size(Arn::Vector2<size_t> new_size);
		//Gets Drawables draw_rect
		//(Draw Rect is the surface which is drawn on the Window)
		//(By default this is set to the whole Drawable and is reset at every resize)
		Lit::Rect<size_t> get_display_surface();
		//Gets Drawables draw_rect
		//(Draw Rect is the surface which is drawn on the Window)
		//(By default this is set to the whole Drawable and is reset at every resize)
		void set_display_surface(Lit::Rect<size_t> new_draw_rect);
		//Get Raw Drawable Array
		const Arn::Tensor2D<wchar_t>& data();
		//Get Primitive Raw Drawable Array
		wchar_t* rawdata();

		//Displaces the position by offset
		void move(Arn::Vector2<float> offset);
		//Fill the drawable with a char
		void fill(wchar_t character);
		//Fill a part of the drawable with a char
		void fill(wchar_t character, Arn::Vector2<size_t> top_left, Arn::Vector2<size_t> bottom_right);
		//Fill a part of the drawable with a char
		void fill(wchar_t character, Lit::Rect<size_t> rect);
	};
}

#endif
