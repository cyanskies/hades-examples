#include "bounce_state.hpp"

#include "yaml-cpp/yaml.h"

#include "hades/objects.hpp"
#include "Hades/files.hpp"
#include "Hades/Server.hpp"
#include "hades/sf_time.hpp"
#include "hades/time.hpp"

void bounce_state::init()
{
	const auto level_str = hades::files::as_string("bounce", "bounce.lvl");
	const auto level_yaml = YAML::Load(level_str);
	hades::level l;
	//objects::ReadObjectsFromYaml(level_yaml, l);
	const auto sv = hades::make_save_from_level(l);
	_server = hades::create_server(sv);
}

bool bounce_state::handle_event(const hades::event &)
{
	return false;
}

void bounce_state::update(hades::time_duration deltaTime, const sf::RenderTarget&, hades::input_system::action_set)
{
	_server->update(deltaTime);
}

void bounce_state::draw(sf::RenderTarget & target, hades::time_duration deltaTime)
{
}

void bounce_state::reinit()
{
}

void bounce_state::pause()
{
}

void bounce_state::resume()
{
}
