#ifndef BREAKOUT_STATE_HPP
#define BREAKOUT_STATE_HPP

#include "Hades/State.hpp"

class breakout_game : public State
{
public:
	void init() override;
	bool handleEvent(const Event&) override;
	void update(sf::Time deltaTime, const sf::RenderTarget&, InputSystem::action_set) override;
	void draw(sf::RenderTarget &target, sf::Time deltaTime) override;

	void reinit() override; 
	void pause() override;
	void resume() override;
};

#endif
