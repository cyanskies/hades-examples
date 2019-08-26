#include "simple_state.hpp"

#include <tuple>

#include "hades/animation.hpp"
#include "hades/camera.hpp"
#include "hades/console_variables.hpp"
#include "hades/data.hpp"
#include "hades/properties.hpp"
#include "hades/time.hpp"

void simple_state::init()
{}

bool simple_state::handle_event(const hades::event&)
{
	return false;
}

void simple_state::update(hades::time_duration, const sf::RenderTarget&, hades::input_system::action_set)
{
	for (auto &s : _sprites)
	{
		s.pos += s.move;
		_batch.set_position(s.id, s.pos);
	}

	_add_sprite();
	_add_sprite();
	_add_sprite();

	LOG("balls: " + hades::to_string(std::size(_sprites)));

	return;
}

void simple_state::draw(sf::RenderTarget& target, hades::time_duration deltaTime)
{
	target.setView(_view);
	_batch.apply();
	target.draw(_batch);

	return;
}

void simple_state::reinit()
{
	const auto w = hades::console::get_int(hades::cvars::video_width,
		hades::cvars::default_value::video_width);
	const auto h = hades::console::get_int(hades::cvars::video_height,
		hades::cvars::default_value::video_height);

	_view.setSize({ static_cast<float>(*w), static_cast<float>(*h) });

	return;
}

void simple_state::pause()
{
}

void simple_state::resume()
{
}

void simple_state::_add_sprite()
{
	constexpr auto anim_name = "ball-anim";
	constexpr auto spawn_pos = hades::vector_float{ 88.f, 88.f };
	constexpr auto ball_size = hades::vector_float{ 25.f, 25.f };
	static const auto anim = hades::data::get<hades::resources::animation>(hades::data::get_uid(anim_name));

	sprite next{};
	next.id = _batch.create_sprite(anim, hades::time_point{},
		hades::sprite_utility::layer_t{}, spawn_pos, ball_size);
	next.pos = spawn_pos;
	next.move = { hades::random(-1.f, 1.f), hades::random(-1.f, 1.f) };

	_sprites.emplace_back(next);
	return;
}
