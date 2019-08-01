#include "bounce_state.hpp"

//#include "yaml-cpp/yaml.h"

#include "hades/camera.hpp"
#include "hades/console_variables.hpp"
#include "hades/files.hpp"
#include "hades/level.hpp"
#include "hades/properties.hpp"
#include "Hades/Server.hpp"
#include "hades/time.hpp"

void bounce_state::init()
{
	const auto level_str = hades::files::as_string("bounce", "bounce.lvl");
	auto l = hades::deserialise(level_str);
	auto sv = hades::make_save_from_level(std::move(l));
	_server = hades::create_server(std::move(sv));
	_level = _server->connect_to_level(hades::unique_id::zero);
	assert(_level);
}

bool bounce_state::handle_event(const hades::event &)
{
	return false;
}

void bounce_state::update(hades::time_duration deltaTime, const sf::RenderTarget&, hades::input_system::action_set)
{
	_server->update(deltaTime);
}

void bounce_state::draw(sf::RenderTarget &target, hades::time_duration deltaTime)
{
	const auto changes = _level->get_changes();
	_client_level.input_updates(changes);
	_current_time += deltaTime;
	_client_level.make_frame_at(_current_time, nullptr, _render_output);
	_render_output.prepare();
	target.setView(_view);
	target.draw(_render_output);
}

void bounce_state::reinit()
{
	const auto w = hades::console::get_int(hades::cvars::video_width, 
		hades::cvars::default_value::video_width);
	const auto h = hades::console::get_int(hades::cvars::video_height,
		hades::cvars::default_value::video_height);

	_view.setSize({ static_cast<float>(*w), static_cast<float>(*h) });
}

void bounce_state::pause()
{
}

void bounce_state::resume()
{
}
