#pragma once				       //makes the compiler only compile the .h file once
#include <chrono>
#include "display.h"			   //allows the file to attain access to the display.h file (to get the class)
#include "txt_engine_elements.h"   //allows the file to attain access the txt_engine_elements.h file

namespace txt_engine //namespace containing all the in_built features of the Text Engine library
{
	class screen : public display
		//creates a screen class which allows the user to chrit and display their sprites and images onto a surface with a configurable rendering speed(ups)
	{
	private:
		image_map previous_map_{}; //creates an empty image_map to allow double buffering
		measure_time::steady_clock::time_point last_time_{measure_time::steady_clock::now()};
		//variable containing the time of the last update
		measure_time::steady_clock::time_point current_time_{last_time_}; //variable containing the current time
		long long update_time_{}; // variable containing the ups (updates per second) 
		long long delta_time_{0}; // variable containing time between the last time and the current time
		void draw_border() const; //function which draws the screen boarder

	public:
		//constructor for the class 'screen'
		screen(int width, int height, long double ups = 15, char width_boarder = '=', char height_boarder = '|');

		//sets the screen ups
		void screen_ups_set(long double ups);
		//resizes the screen size (warning : if size set is too big , console will need to be expanded)
		void set_screen_size(int width, int height);
		//displays the screen onto the console 
		void render();
		//returns the time between the last update and the current update 
		int get_delta_time() const;
		//returns true if the screen at the ups set speed
		//(recommended that it should be stored in a if statement with all your main code within the scope) 
		bool update();
	};
}
