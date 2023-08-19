#pragma once				       //makes the compiler only compile the .h file once
#include <vector>				   //allows the file to attain access the vector class(a dynamic array)
#include "txt_engine_elements.h"   //allows the file to attain access the txt_engine_elements.h file
#include "sprite.h"				   //allows the file to attain access to the sprite.h file (to get the class)


namespace txt_engine //namespace containing all the in_built features of the Text Engine library
{
	class display
		//creates the display class which allows the user to chrit and display sprites and images onto a surface
	{
	protected:
		image_map map_{}; //creates an empty image_map for the display
		//variable storing the character which will be drawn on the display boarder(top and bottom)
		char width_boarder_{};
		//variable storing the character which will be drawn on the display boarder(left and right)
		char height_boarder_{};

	public:
		//constructor for the class 'display'
		display(int width, int height, char width_boarder = '=', char height_boarder = '|');

		//resizes the display size (warning : if size set is too big , console will need to be expanded)
		void set_display_size(int width, int height);
		//changes the characters in-which the display is drawn
		void set_boarder(char width_boarder, char height_boarder);
		//manually set the console cursor position to a coordinate on the display
		static void set_cursor_position(int x, int y);
		//displays the display image onto the console
		void render() const;
		//adds a sprite image onto the display image (position is dependent on current sprite position variable)
		void chrit(sprite& sprite);
		//adds a sprite in a .txt file onto the display image at the given position
		void chrit(const std::string& filename, vector2 initial_position = {0, 0});
		//resets the image to be empty (will not change image size)
		void clear();
		//draws a empty rectangle of a given size onto the display at a given position with the given boarder
		void draw_rectangle(vector2 size, vector2 position = {0, 0}, char boarder = '#');
		//draws a filled rectangle of a given size onto the display at a given position with the given filling
		void fill_rectangle(vector2 size, vector2 position = {0, 0}, char filling = '#');
		//returns the current display image
		image_map get_image();
		//returns the current display width
		int get_width() const;
		//returns the current display height
		int get_height() const;
		//returns the current display dimensions (width and height)
		vector2 get_dimensions() const;
		//returns the current display surface area (the total amount of characters which can be displayed)
		int get_surface_area() const;
		//returns true if the given pixel position is on the display screen
		bool on_screen(vector2 chr_pixel) const;
	};
}
