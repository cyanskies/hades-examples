#ifndef BOUNCE_INPUT_HPP
#define BOUNCE_INPUT_HPP

#include "hades/input.hpp"
#include "hades/data.hpp"

namespace input
{
	using action_id = hades::input_system::action_id;
	extern action_id spawn_action;
}

inline void register_bounce_input(hades::input_system& input)
{
	input::spawn_action = hades::data::make_uid("spawn");
	input.create(input::spawn_action, false, "space");
}

#endif // !BOUNCE_INPUT_HPP