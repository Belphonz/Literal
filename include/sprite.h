#pragma once				       //makes the compiler only compile the .h file once
#include <string>
#include <vector>				   //allows the file to attain access the vector class(a dynamic array)
#include "txt_engine_elements.h"   //allows the file to attain access the txt_engine_elements.h file

namespace txt_engine //namespace containing all the in_built features of the Text Engine library
{
	class sprite
		//creates the sprites class which can store images from .txt files or given by the user and pure text allowing them to then be chriited and subsequently displayed on a surface
	{
	protected:
		image image_{}; //creates an empty image for the sprite
		vector2 vertex_{}; //variable storing the position of the sprite (will start drawing from this position)
		char background_{};
		//variable storing the background of the sprite (won't get drawn when chritted onto screen/display)

	public:
		//constructors for the class 'sprite'
		explicit sprite(const std::string& text, bool is_text = false, char background = '\0');
		explicit sprite(const std::string& text, vector2 initial_position = {0, 0}, bool is_text = false,
		                char background = '\0');
		explicit sprite(image base_image, vector2 initial_position = {0, 0});

		//changes the sprite image to the sprite stored in the .txt file passed as an argument,
		void set_file_as_image(const std::string& filename);
		//changes the sprite image to the string passed as an argument
		//('\n' will make the text go to a newline)
		void set_text_as_image(const std::string& text);
		//changes the sprite image to the image(variable type) passed as an argument
		void set_image_as_image(const image& image);
		//changes the current image position
		//(the image will be drawn starting from this position, from left to down)
		void set_image_position(vector2 position);
		//changes the image background
		//(the image background will not be chritted onto the display/screen)
		void set_image_background_chr(char background_chr);
		//adds a phrase to the center of the sprite 
		void add_label(const std::string& label);
		//returns the image(variable type) of the sprite
		image get_image();
		//returns the image width
		int get_image_width() const;
		//returns the image height
		int get_image_height() const;
		//returns the image surface area (total amount of characters)
		int get_image_surface_area() const;
		//returns the image background character
		char get_image_background_chr() const;
		//returns the current image position 
		vector2 get_image_position() const;
	};
}
