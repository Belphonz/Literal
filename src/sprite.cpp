#include "sprite.h"
#include "txt_engine_elements.h"
#include <fstream>
#include <iostream>


txt_engine::sprite::sprite(const std::string& text, const bool is_text, const char background)
{
	if (is_text)
	{
		set_text_as_image(text);
	}
	else
	{
		set_file_as_image(text);
	}
	set_image_background_chr(background);
}

txt_engine::sprite::sprite(const std::string& text, const vector2 initial_position, const bool is_text,
                           const char background)
{
	if (is_text)
	{
		set_text_as_image(text);
	}
	else
	{
		set_file_as_image(text);
	}
	set_image_background_chr(background);
	set_image_position(initial_position);
}

txt_engine::sprite::sprite(image base_image, const vector2 initial_position): image_{std::move(base_image)}
{
	set_image_position(initial_position);
}

void txt_engine::sprite::set_file_as_image(const std::string& filename)
{
	image_.clear();
	std::ifstream in_stream;
	char temp{};
	int i{0};
	image_.push_back(std::vector<char>{});
	in_stream.open(filename);
	while (!in_stream.eof())
	{
		temp = static_cast<char>(in_stream.get());

		if (temp != '\n')
		{
			image_[i].push_back(temp);
		}
		else
		{
			i ++;
			image_.push_back(std::vector<char>{});
		}
	}
	in_stream.close();
}

void txt_engine::sprite::set_text_as_image(const std::string& text)
{
	image_.clear();
	int j{0};
	int current_width{0};
	int max_width{0};
	image_.push_back(std::vector<char>{});
	for (int i{0}; i < static_cast<int>(text.length()); i++)
	{
		if (text[i] == '\n')
		{
			j++;
			if (current_width > max_width)
			{
				max_width = current_width;
			}
			current_width = {0};
			image_.push_back(std::vector<char>{});
		}
		else
		{
			current_width ++;
			image_[j].push_back(text[i]);
		}
		if (current_width > max_width)
		{
			max_width = current_width;
		}
	}
	for (int i{0}; i < static_cast<int>(image_.size()); i++)
	{
		image_[i].resize(max_width, background_);
	}
}

void txt_engine::sprite::set_image_as_image(const image& image)
{
	image_ = image;
}

void txt_engine::sprite::set_image_position(vector2 position)
{
	vertex_ = {position};
}

void txt_engine::sprite::set_image_background_chr(const char background_chr)
{
	background_ = background_chr;
}

void txt_engine::sprite::add_label(const std::string& label)
{
	const int txt_length{static_cast<int>(label.length())};
	int j{0};
	for (int i{get_image_width() / 2 - txt_length / 2}; i <= txt_length + get_image_width() / 2 - txt_length / 2 - 1; i
	     ++)
	{
		image_[get_image_height() / 2][i] = label[j];
		j++;
	}
}

txt_engine::image txt_engine::sprite::get_image()
{
	return image_;
}

int txt_engine::sprite::get_image_width() const
{
	return static_cast<int>(image_[0].size());
}

int txt_engine::sprite::get_image_height() const
{
	return static_cast<int>(image_.size());
}

int txt_engine::sprite::get_image_surface_area() const
{
	return get_image_width() * get_image_height();
}

char txt_engine::sprite::get_image_background_chr() const
{
	return background_;
}

txt_engine::vector2 txt_engine::sprite::get_image_position() const
{
	return vertex_;
}
