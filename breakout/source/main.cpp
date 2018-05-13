#include "Hades/Main.hpp"

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

void hadesMain(hades::StateManager &state, hades::InputSystem &bindings, hades::CommandList &commandLine)
{
	breakout_input(bindings);

	//create breakout state;
}