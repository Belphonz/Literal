#include "miscellaneous.h"

#include <array>

#include "txt_engine_elements.h"
constexpr int key_down{0x8000};

txt_engine::image txt_engine::create_rectangle(const vector2 size, const char boarder)
{
	image rectangle(size.y);
	for (int i{0}; i < size.y; i++)
	{
		rectangle[i].resize(size.x, ' ');
	}
	for (int i{0}; i < size.y; i++)
	{
		for (int j{0}; j < size.x; j++)
		{
			if (i == 0 || i == size.y - 1 || j == 0 || j == size.x - 1)
			{
				rectangle[i][j] = boarder;
			}
		}
	}
	return rectangle;
}

txt_engine::image txt_engine::create_filled_rectangle(const vector2 size, const char filling)
{
	image rectangle(size.y);
	for (int i{0}; i < size.y; i++)
	{
		rectangle[i].resize(size.x, ' ');
	}
	for (int i{0}; i < size.y; i++)
	{
		for (int j{0}; j < size.x; j++)
		{
			rectangle[i][j] = filling;
		}
	}
	return rectangle;
}

int txt_engine::get_screen_center_x(const int x, const sprite& object)
{
	return x / 2 - object.get_image_width() / 2;
}

int txt_engine::get_screen_center_y(const int y, const sprite& object)
{
	return y / 2 - object.get_image_height() / 2;
}

txt_engine::vector2 txt_engine::get_screen_center(const vector2 position, const sprite& object)
{
	return {get_screen_center_x(position.x, object), get_screen_center_y(position.y, object)};
}

int txt_engine::get_container_center_x(const sprite& container, const sprite& object)
{
	return container.get_image_position().x + container.get_image_width() / 2 - object.get_image_width() / 2;
}

int txt_engine::get_container_center_y(const sprite& container, const sprite& object)
{
	return container.get_image_position().y + container.get_image_height() / 2 - object.get_image_height() / 2;
}

txt_engine::vector2 txt_engine::get_container_center(const sprite& container, const sprite& object)
{
	return {get_container_center_x(container, object), get_container_center_y(container, object)};
}

void txt_engine::navigation(const std::array<int, 4>& navigation_keys, vector2& current_selected, const int max_down,
                            const int max_right, bool& is_current_key_pressed)
{
	if ((GetAsyncKeyState(navigation_keys.at(0)) & key_down || GetAsyncKeyState(navigation_keys.at(1)) & key_down ||
		GetAsyncKeyState(navigation_keys.at(2)) & key_down ||
		GetAsyncKeyState(navigation_keys.at(3)) & key_down) && is_current_key_pressed)
	{
	}
	else if (GetAsyncKeyState(navigation_keys.at(0)) & key_down && current_selected.y - 1 >= 0)
	{
		current_selected.y --;
		is_current_key_pressed = true;
	}
	else if (GetAsyncKeyState(navigation_keys.at(1)) & key_down && current_selected.y + 1 <= max_down)
	{
		current_selected.y ++;
		is_current_key_pressed = true;
	}
	else if (GetAsyncKeyState(navigation_keys.at(2)) & key_down && current_selected.x - 1 >= 0)
	{
		current_selected.x --;
		is_current_key_pressed = true;
	}
	else if (GetAsyncKeyState(navigation_keys.at(3)) & key_down && current_selected.x + 1 <= max_right)
	{
		current_selected.x ++;
		is_current_key_pressed = true;
	}
	else
	{
		is_current_key_pressed = false;
	}
}

void txt_engine::button_map_selection(const button_map map, const vector2 selection)
{
	for (int i{0}; i < static_cast<int>(map.size()); i++)
	{
		for (int j{0}; j < static_cast<int>(map[0].size()); j++)
		{
			map[i][j]->set_state(false);
		}
	}
	map[selection.y][selection.x]->set_state(true);
}
