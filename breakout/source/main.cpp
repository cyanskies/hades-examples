#include "Hades/Main.hpp"

int main(int argc, char **argv)
{
	return hades_main(argc, argv);
}

std::string_view defaultGame()
{
	return "ex-breakout";
}

void resourceTypes(hades::data::data_system &data)
{
	//breakout_resources();
}

void hadesMain(hades::StateManager &state, hades::InputSystem &bindings, hades::CommandList &commandLine)
{
	//breakout_input();

	//create breakout state;
}