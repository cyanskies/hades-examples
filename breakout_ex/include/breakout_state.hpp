#ifndef BREAKOUT_STATE_HPP
#define BREAKOUT_STATE_HPP

#include <array>
#include <tuple>

#include "SFML/Graphics/Sprite.hpp"

#include "hades/collision.hpp"
#include "Hades/State.hpp"
#include "Hades/types.hpp"
#include "hades/vector_math.hpp"

constexpr std::tuple<hades::types::int32, hades::types::int32> screen_size();

void set_window_size();

class breakout_game : public hades::State
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

	struct game_elements;
	//places all the game elements for a new game
	game_elements _prepare_game() const;

	using collision_return = std::tuple<hades::direction, hades::vector_t<float>>;

	collision_return _test_block_collision(hades::rect_t<float> ball);
	collision_return _test_edge_collision(hades::rect_t<float> ball);
	collision_return _test_paddle_collision(hades::rect_t<float> ball, hades::rect_t<float> paddle);

	sf::View _game_view;

	struct game_elements
	{
		//vector of blocks
		std::vector<sf::Sprite> blocks;
		//vector of walls
		std::vector<sf::Sprite> walls;
		//paddle
		sf::Sprite paddle;
		//ball
		sf::Sprite ball;
	};

	game_elements _sprites;
	hades::vector_t<float> _ball_velocity;
};

#endif
