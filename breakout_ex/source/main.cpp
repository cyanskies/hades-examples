#include "Hades/Main.hpp"

#include "breakout_state.hpp"
#include "input.hpp"

int main(int argc, char **argv)
{
	return hades_main(argc, argv);
}

std::string_view defaultGame()
{
	return "breakout";
}

void resourceTypes(hades::data::data_system &data)
{}

void hadesMain(hades::StateManager &state, hades::input_system &bindings, hades::command_list &commandLine)
{
	breakout_input(bindings);

	set_window_size();

	state.push(std::make_unique<breakout_game>());
}