#include "Hades/App.hpp"
#include "Hades/Main.hpp"
#include "hades/mouse_input.hpp"

#include "bounce_state.hpp"
#include "bounce_systems.hpp"

int main(int argc, char **argv)
{
	return hades_main(argc, argv);
}

std::string_view defaultGame()
{
	return "bounce";
}

void resourceTypes(hades::data::data_system &data)
{
	//objects::RegisterObjectResources(&data);

	register_bounce_systems(data);
}

void hadesMain(hades::StateManager &state, hades::input_system &bindings, hades::command_list &commandLine)
{
	hades::register_mouse_input(bindings);

	/*const auto editor = hades::LoadCommand(commandLine, "editor", [&state](auto &&args) {
		state.push(std::make_unique<objects::object_editor>());
		return true;
	});

	if (editor)
		state.push(std::make_unique<objects::object_editor>());
	else*/
		state.push(std::make_unique<bounce_state>());
}