#pragma once			           //makes the compiler only compile the .h file once
#include "TXT_engine.h"            //allows the file to gain access to all elements in the Text engine library
#include "txt_engine_elements.h"   //allows the file to attain access the txt_engine_elements.h file


namespace txt_engine //namespace containing all the in_built features of the Text Engine library
{
	//returns a rectangle of type image(variable) of a given size with the given boarder
	image create_rectangle(vector2, char);
	//returns a filled rectangle of type image(variable) of a given size with the given filling
	image create_filled_rectangle(vector2, char);
	//returns the screen width center in function of given sprite size
	int get_screen_center_x(int, const sprite&);
	//returns the screen height center in function of given sprite size
	int get_screen_center_y(int, const sprite&);
	//returns the screen center in function of given sprite size
	vector2 get_screen_center(vector2, const sprite&);
	//returns center width of a sprite in function of given sprite size
	int get_container_center_x(const sprite&, const sprite&);
	//returns center height of a sprite in function of given sprite size
	int get_container_center_y(const sprite&, const sprite&);
	//returns center of a sprite in function of given sprite size
	vector2 get_container_center(const sprite&, const sprite&);
	//function to make 4 chosen keys only activate once when pressed
	void navigation(const std::array<int, 4>&, vector2&, int, int, bool&);
	//function which turns which selects a button at the given position and turns off the rest
	void button_map_selection(button_map, vector2);
}
