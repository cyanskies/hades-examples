#include "Hades/Main.hpp"

#include "hades/animation.hpp"

#include "simple_state.hpp"

int main(int argc, char** argv)
{
	return hades_main(argc, argv);
}

std::string_view defaultGame()
{
	return "bounce";
}

void resourceTypes(hades::data::data_system& data)
{
	hades::register_animation_resource(data);
}

void hadesMain(hades::StateManager& state, hades::input_system& bindings, hades::command_list& commandLine)
{
	state.push(std::make_unique<simple_state>());
}