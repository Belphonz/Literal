#pragma once				       //makes the compiler only compile the .h file once
#include <chrono>
#include <vector>				   //allows the file to attain access the vector class(a dynamic array)


namespace txt_engine //namespace containing all the in_built features of the Text Engine library
{
	struct vector2 //struct template for a two-dimensional vector
	{
		int x{};
		int y{};
	};

	using image_map = std::vector<std::vector<char>>; //variable type of an array of char arrays
	using image = std::vector<std::vector<char>>; //variable type of an array of char arrays
	namespace measure_time = std::chrono; //variable type of the chrono class's steady_clock
}
