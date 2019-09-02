#ifndef BOUNCE_HPP
#define BOUNCE_HPP

#include <memory>

#include "Hades/render_instance.hpp"
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
	sf::View _view;

	hades::render_interface _render_output;
	hades::render_instance _client_level{ nullptr };
	hades::server_level* _level = nullptr;
	hades::time_point _current_time;
	std::unique_ptr<hades::server_hub> _server;
};

#endif // !BOUNCE_HPP