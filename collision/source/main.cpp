#include "Hades/Main.hpp"
#include "hades/mouse_input.hpp"

#include "collision_state.hpp"

int main(int argc, char **argv)
{
	return hades_main(argc, argv);
}

std::string_view defaultGame()
{
	return "collision";
}

hades::unique_id change_shape = hades::unique_id::zero;

void resourceTypes(hades::data::data_system &data)
{
	change_shape = data.get_uid("change_shape");
}

void hadesMain(hades::StateManager &state, hades::input_system &bindings, hades::command_list &commandLine)
{
	hades::register_mouse_input(bindings);
	bindings.create(change_shape, false, "space");
	state.push(std::make_unique<collision_game>());
}