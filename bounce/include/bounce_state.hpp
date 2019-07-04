#ifndef BOUNCE_HPP
#define BOUNCE_HPP

#include <memory>

#include "hades/render_interface.hpp"
#include "Hades/Server.hpp"
#include "Hades/State.hpp"

class bounce_state : public hades::state
{
public:
	void init() override;
	bool handle_event(const hades::event&) override;
	void update(hades::time_duration deltaTime, const sf::RenderTarget&, hades::input_system::action_set) override;

	void draw(sf::RenderTarget &target, hades::time_duration deltaTime) override;
	void reinit() override;
	void pause() override;
	void resume() override;

private:
	hades::render_interface _render_output;
	std::unique_ptr<hades::server_hub> _server;
};

#endif // !BOUNCE_HPP