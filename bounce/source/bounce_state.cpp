#include "bounce_state.hpp"

#include "Hades/Server.hpp"

void bounce_state::init()
{
	hades::level_save l;
	_server = hades::create_server(l);
}