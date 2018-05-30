#ifndef COLLISION_STATE_HPP
#define COLLISION_STATE_HPP

#include <chrono>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

#include "hades/collision.hpp"
#include "Hades/State.hpp"
#include "Hades/types.hpp"

using vector_f = hades::vector_t<float>;

enum shapes {rect, circle, point, max_shape};

class collision_game : public hades::State
{
public:
	void init() override;
	bool handleEvent(const hades::event&) override;
	void update(sf::Time deltaTime, const sf::RenderTarget&, hades::input_system::action_set) override;
	void draw(sf::RenderTarget &target, sf::Time deltaTime) override;

	void reinit() override; 
	void pause() override;
	void resume() override;

private:
	sf::RectangleShape	_rect, _my_rect,
		_point, _my_point;
	sf::CircleShape		_circ, _my_circ;

	sf::RectangleShape _backdrop;

	shapes _shape;
	const float _speed = 5.f;
	vector_f _target;

	sf::Time _current = sf::Time::Zero;
	sf::Time _shape_change_time;
};

#endif // !COLLISION
