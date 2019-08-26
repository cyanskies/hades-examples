#ifndef SIMPLE_HPP
#define SIMPLE_HPP

#include <vector>

#include "SFML/Graphics/Sprite.hpp"

#include "hades/sprite_batch.hpp"
#include "Hades/State.hpp"
#include "hades/vector_math.hpp"

struct sprite
{
	hades::sprite_batch::sprite_id id;
	hades::vector_float pos;
	hades::vector_float move;
};

class simple_state : public hades::state
{
public:
	void init() override;
	bool handle_event(const hades::event&) override;
	void update(hades::time_duration deltaTime, const sf::RenderTarget&, hades::input_system::action_set) override;

	void draw(sf::RenderTarget& target, hades::time_duration deltaTime) override;
	void reinit() override;
	void pause() override;
	void resume() override;

private:

	void _add_sprite();
	sf::View _view;

	hades::sprite_batch _batch;
	std::vector<sprite> _sprites;
};

#endif // !SIMPLE_HPP