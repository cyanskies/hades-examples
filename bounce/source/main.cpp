#include "Hades/App.hpp"
#include "hades/level_editor.hpp"
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
	register_bounce_resources(data);
	register_bounce_systems(data);
	hades::create_editor_console_variables();
	hades::register_level_editor_resources(data);
}

void hadesMain(hades::StateManager &state, hades::input_system &bindings, hades::command_list &commandLine)
{
	hades::register_mouse_input(bindings);

	using bounce_editor = hades::basic_level_editor<
		hades::level_editor_level_props,
		hades::level_editor_objects,
		hades::level_editor_grid
	>;

	const auto editor = hades::LoadCommand(commandLine, "editor", [&state](auto &&args) {
		state.push(std::make_unique<bounce_editor>());
		return true;
	});

	if (!editor)
		state.push(std::make_unique<bounce_state>());
}