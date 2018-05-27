#ifndef BREAKOUT_INPUT_HPP
#define BREAKOUT_INPUT_HPP

#include <array>

#include "Hades/Input.hpp"

namespace input
{
	using input_id = hades::unique_id;
	enum input_names { move_left, move_right };
	constexpr auto names_size = 2;
	extern std::array<input_id, names_size> names;
}

void breakout_input(hades::input_system& input);

#endif // !BREAKOUT_INPUT_HPP
