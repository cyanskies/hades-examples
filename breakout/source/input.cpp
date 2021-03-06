#include "input.hpp"

#include "Hades/Data.hpp"

namespace input
{
	std::array<input_id, names_size> names{input_id::zero};
}

void breakout_input(hades::input_system &inputsys)
{
	using namespace input;
	using namespace hades::data;
	names[move_left] = make_uid("move_left");
	names[move_right] = make_uid("move_right");

	inputsys.create(names[move_left], false, "left");
	inputsys.create(names[move_right], false, "right");
}