#include "collision_state.hpp"

#include "Hades/Data.hpp"
#include "Hades/Logging.hpp"

void collision_game::init()
{}

bool collision_game::handleEvent(const hades::event &)
{
	return false;
}

void collision_game::update(sf::Time, const sf::RenderTarget&, hades::input_system::action_set a)
{
	static const auto change_shape = hades::data::GetUid("change_shape");

	const auto ch_shape = a.find(change_shape);
	if (ch_shape->active)
	{
		_shape = static_cast<shapes>((_shape + 1) % max_shape);
		LOG(hades::to_string(_shape));
	}
}

void collision_game::draw(sf::RenderTarget & target, sf::Time deltaTime)
{
}

void collision_game::reinit()
{
}

void collision_game::pause()
{
}

void collision_game::resume()
{
}