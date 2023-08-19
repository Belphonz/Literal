#include <chrono>
#include "txt_engine_elements.h"
#include "screen.h"
#include <iostream>


void txt_engine::screen::draw_border() const
{
	set_cursor_position(-1, -1);
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
				set_cursor_position(j, i - 1);
			}
		}
		printf("\n");
	}
}

txt_engine::screen::screen(const int width, const int height, const long double ups, const char width_boarder,
                           const char height_boarder):
	display(
		width, height, width_boarder, height_boarder)
{
	previous_map_ = map_;
	screen_ups_set(ups);
	draw_border();
}

void txt_engine::screen::screen_ups_set(long double ups)
{
	update_time_ = static_cast<long long>(1 / ups * 1000);
}

void txt_engine::screen::set_screen_size(const int width, const int height)
{
	const char temp_width{width_boarder_};
	const char temp_height{height_boarder_};
	width_boarder_ = ' ';
	height_boarder_ = ' ';
	draw_border();

	map_.resize(height);
	for (int i{0}; i < height; i++)
	{
		map_[i].resize(width, ' ');
	}
	previous_map_ = map_;
	width_boarder_ = temp_width;
	height_boarder_ = temp_height;
	draw_border();
}


bool txt_engine::screen::update()
{
	current_time_ = measure_time::steady_clock::now();
	const bool update = measure_time::duration_cast<measure_time::milliseconds>(current_time_ - last_time_).count() >
		update_time_;
	if (update)
	{
		delta_time_ = measure_time::duration_cast<measure_time::milliseconds>(current_time_ - last_time_).count();
		last_time_ = current_time_;
	}
	return update;
}

int txt_engine::screen::get_delta_time() const
{
	return static_cast<int>(delta_time_ / 1000);
}

void txt_engine::screen::render()
{
	for (int y{0}; y < get_height(); y++)
	{
		for (int x{0}; x < get_width(); x++)
		{
			if (map_[y][x] == previous_map_[y][x])
			{
				continue;
			}
			set_cursor_position(x, y);
			printf("%c", map_[y][x]);
		}
	}
	previous_map_ = map_;
	set_cursor_position(get_width(), get_height());
}
