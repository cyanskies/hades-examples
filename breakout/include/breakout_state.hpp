#ifndef BREAKOUT_STATE_HPP
#define BREAKOUT_STATE_HPP

#include <array>
#include <tuple>

#include "SFML/Graphics/Sprite.hpp"

#include "Hades/State.hpp"
#include "Hades/types.hpp"

constexpr std::tuple<hades::types::int32, hades::types::int32> screen_size();

class breakout_game : public hades::State
{
public:
	void init() override;
	bool handleEvent(const hades::Event&) override;
	void update(sf::Time deltaTime, const sf::RenderTarget&, hades::InputSystem::action_set) override;
	void draw(sf::RenderTarget &target, sf::Time deltaTime) override;

	void reinit() override; 
	void pause() override;
	void resume() override;

private:

	struct game_elements;
	//places all the game elements for a new game
	game_elements _prepare_game() const;

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
	sf::Vector2i _ball_velocity;
};

#endif
