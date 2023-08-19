#include "button.h"
#include <fstream>
#include "txt_engine_elements.h"

txt_engine::button::button(const std::string& text, const std::string& text_selected,
                           const bool is_text,
                           const char background): sprite(
	text, is_text, background)
{
	base_image_ = image_;
	if (is_text)
	{
		set_text_as_image(text_selected, true);
	}
	else
	{
		set_file_as_image(text_selected, true);
	}
	selected_image_ = image_;
	image_ = base_image_;
}

txt_engine::button::button(const std::string& text, const std::string& text_selected,
                           const vector2 initial_position,
                           const bool is_text, const char background): sprite(
	text, initial_position, is_text, background)

{
	base_image_ = image_;
	if (is_text)
	{
		set_text_as_image(text_selected, true);
	}
	else
	{
		set_file_as_image(text_selected, true);
	}
	selected_image_ = image_;
	image_ = base_image_;
}


txt_engine::button::button(const image& base_image, image selected_image, const vector2 initial_position):
	sprite{base_image, initial_position}, base_image_{base_image}, selected_image_{std::move(selected_image)}

{
}

txt_engine::image txt_engine::button::get_selected_button_image()
{
	return selected_image_;
}

txt_engine::image txt_engine::button::get_base_button_image()
{
	return base_image_;
}

txt_engine::image txt_engine::button::get_current_button_image()
{
	return image_;
}

bool txt_engine::button::get_current_button_state() const
{
	return is_selected_;
}


void txt_engine::button::set_file_as_image(const std::string& filename, const bool selected)
{
	image& drawing{base_image_};
	if (selected) { drawing = selected_image_; }
	drawing.clear();
	drawing.clear();
	std::ifstream in_stream;
	char temp{};
	int i{0};
	drawing.push_back(std::vector<char>{});
	in_stream.open(filename);
	while (!in_stream.eof())
	{
		temp = static_cast<char>(in_stream.get());

		if (temp != '\n')
		{
			drawing[i].push_back(temp);
		}
		else
		{
			i ++;
			drawing.push_back(std::vector<char>{});
		}
	}
	in_stream.close();
}

void txt_engine::button::set_text_as_image(const std::string& text, const bool selected)
{
	image& drawing{base_image_};
	if (selected) { drawing = selected_image_; }
	drawing.clear();
	int j{0};
	int current_width{0};
	int max_width{0};
	drawing.push_back(std::vector<char>{});
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
			drawing.push_back(std::vector<char>{});
		}
		else
		{
			current_width ++;
			drawing[j].push_back(text[i]);
		}
		if (current_width > max_width)
		{
			max_width = current_width;
		}
	}
	for (int i{0}; i < static_cast<int>(drawing.size()); i++)
	{
		drawing[i].resize(max_width, background_);
	}
}

void txt_engine::button::set_image_as_image(const image& image, const bool selected)
{
	if (selected) { selected_image_ = image; }
	else { base_image_ = image; }
}

void txt_engine::button::switch_state()
{
	is_selected_ = !is_selected_;
	if (is_selected_)
	{
		image_ = selected_image_;
	}
	else
	{
		image_ = base_image_;
	}
}

void txt_engine::button::add_label(const std::string& label)
{
	const int txt_length{static_cast<int>(label.length())};
	int j{0};
	for (int i{get_image_width() / 2 - txt_length / 2}; i <= txt_length + get_image_width() / 2 - txt_length / 2 - 1; i
	     ++)
	{
		image_[get_image_height() / 2][i] = label[j];
		base_image_[get_image_height() / 2][i] = label[j];
		selected_image_[get_image_height() / 2][i] = label[j];
		j++;
	}
}

void txt_engine::button::set_state(const bool selected)
{
	is_selected_ = selected;
	if (is_selected_)
	{
		image_ = selected_image_;
	}
	else
	{
		image_ = base_image_;
	}
}
