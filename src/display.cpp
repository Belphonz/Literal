#include "display.h"
#include "txt_engine_elements.h"

#include <cstdio>
#include <fstream>
#include <iostream>
#include <Windows.h>

void txt_engine::display::set_display_size(const int width, const int height)
{
	map_.resize(height);
	for (int i{0}; i < height; i++)
	{
		map_[i].resize(width, ' ');
	}
}

bool txt_engine::display::on_screen(const vector2 chr_pixel) const
{
	return chr_pixel.y < get_height() && chr_pixel.y >= 0 && chr_pixel.x < get_width() && chr_pixel.x >= 0;
}

txt_engine::display::display(const int width, const int height, const char width_boarder, const char height_boarder)
{
	set_display_size(width, height);
	set_boarder(width_boarder, height_boarder);
}

void txt_engine::display::set_boarder(const char width_boarder, const char height_boarder)
{
	width_boarder_ = width_boarder;
	height_boarder_ = height_boarder;
}

void txt_engine::display::set_cursor_position(const int x, const int y)
{
	static const HANDLE h_out{GetStdHandle(STD_OUTPUT_HANDLE)};
	std::cout.flush();
	const COORD coordinates = {static_cast<SHORT>(x + 1), static_cast<SHORT>(y + 1)};
	SetConsoleCursorPosition(h_out, coordinates);
}

void txt_engine::display::render() const
{
	set_cursor_position(0, 0);
	for (int i{0}; i < get_height() + 2; i++)
	{
		for (int j{0}; j < get_width() + 2; j++)
		{
			if (i == 0 || i == get_height() + 1)
			{
				printf("%c", width_boarder_);
			}
			else if (j == 0 || j == get_width() + 1)
			{
				printf("%c", height_boarder_);
			}
			else
			{
				printf("%c", map_[i - 1][j - 1]);
			}
		}
		printf("\n");
	}
}

void txt_engine::display::chrit(sprite& sprite)
{
	char element{};
	for (int i{sprite.get_image_position().y}; i < sprite.get_image_position().y + sprite.get_image_height(); i++)
	{
		for (int j{sprite.get_image_position().x}; j < sprite.get_image_position().x + sprite.get_image_width(); j
		     ++)
		{
			if (on_screen({j, i}))
			{
				element = sprite.get_image().at(i - sprite.get_image_position().y).
				                 at(j - sprite.get_image_position().x);
				if (element != sprite.get_image_background_chr())
				{
					map_[i][j] = element;
				}
			}
		}
	}
}

void txt_engine::display::chrit(const std::string& filename, const vector2 initial_position)
{
	std::ifstream in_stream;
	char element{};
	in_stream.open(filename);
	int j{0};
	for (int i{initial_position.x}; !in_stream.eof(); i++)
	{
		if (on_screen({j, i}))
		{
			element = static_cast<char>(in_stream.get());
			if (element != '\n')
			{
				map_[j][i] = element;
			}
			else
			{
				i = initial_position.x - 1;
				j++;
			}
		}
	}
	in_stream.close();
}

void txt_engine::display::clear()
{
	for (int i{0}; i < get_height(); i++)
	{
		map_[i].assign(get_width(), ' ');
	}
}

void txt_engine::display::draw_rectangle(const vector2 size, const vector2 position, const char boarder)
{
	for (int i{position.y}; i < size.y + position.y; i++)
	{
		for (int j{position.x}; j < size.x + position.x; j++)
		{
			if (i == position.y || i == size.y + position.y - 1 || j == position.x || j == size.x + position.x - 1)
			{
				map_[i][j] = boarder;
			}
		}
	}
}

void txt_engine::display::fill_rectangle(const vector2 size, const vector2 position, const char filling)
{
	for (int i{position.y}; i < size.y + position.y; i++)
	{
		for (int j{position.x}; j < size.x + position.x; j++)
		{
			map_[i][j] = filling;
		}
	}
}

txt_engine::image_map txt_engine::display::get_image()
{
	return map_;
}

int txt_engine::display::get_width() const
{
	return static_cast<int>(map_[0].size());
}

int txt_engine::display::get_height() const
{
	return static_cast<int>(map_.size());
}

txt_engine::vector2 txt_engine::display::get_dimensions() const
{
	return {get_width(), get_height()};
}

int txt_engine::display::get_surface_area() const
{
	return get_width() * get_height();
}
