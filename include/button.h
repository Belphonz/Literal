#pragma once			         //makes the compiler only compile the .h file once
#include "txt_engine_elements.h" //allows the file to attain access the txt_engine_elements.h file
#include "sprite.h"	             //allows the file to attain access to the sprite.h file (to get the class)

namespace txt_engine //namespace containing all the in_built features of the Text Engine library
{
	class button : public sprite
		//creates a button class, usable as a button with a base and selected state, cannot directly activate a function
	{
	private:
		image base_image_{}; //creates an empty image for the button when unselected
		image selected_image_{}; //creates an empty image for the button when selected
		bool is_selected_{}; //boolean returning if the button is selected 

	public:
		//constructors for the class 'button'
		button() = delete;
		button(const std::string& text, const std::string& text_selected, bool is_text = false,
		       char background = '\0');
		button(const std::string& text, const std::string& text_selected,
		       vector2 initial_position = {0, 0},
		       bool is_text = false,
		       char background = '\0');
		button(const image& base_image, image selected_image, vector2 initial_position = {0, 0});

		//returns true if the button is currently selected, false if not
		bool get_current_button_state() const;
		//changes the current button state from one to the other
		void switch_state();
		//adds a phrase to the center of the button
		void add_label(const std::string& label);
		//changes the current state of the button, true -> selected, false -> unselected
		void set_state(bool selected);
		//changes the chosen image to the sprite stored in the .txt file passed as an argument, chosen image is dependent on the selected argument
		void set_file_as_image(const std::string& filename, bool selected);
		//changes the chosen image to the string passed as an argument, chosen image is dependent on the selected argument
		void set_text_as_image(const std::string& text, bool selected);
		//changes the chosen image to the image(variable type) passed as an argument, chosen image is dependent on the selected argument
		void set_image_as_image(const image& image, bool selected);
		//returns the image(variable type) for the selected image
		image get_selected_button_image();
		//returns the image(variable type) for the base image
		image get_base_button_image();
		//returns the image(variable type) for the current button image
		image get_current_button_image();
	};

	//variable type of an array of button address arrays
	using button_map = std::vector<std::vector<button*>>;
}
