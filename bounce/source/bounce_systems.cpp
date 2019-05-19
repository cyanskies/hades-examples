#include "bounce_systems.hpp"

#include "hades/curve_extra.hpp"
#include "hades/level_interface.hpp"
#include "Hades/simple_resources.hpp"

namespace global
{
	static hades::unique_id position = hades::unique_id::zero,
		ball = hades::unique_id::zero;

	static float pos_x = 0.f, pos_y = 0.f;
}

void on_create_spawn(hades::system_job_data)
{
	//position the ball will be created in
	global::position = hades::data::get_uid("position");
	//the id of the ball type to create
	global::ball = hades::data::get_uid("ball");
}

void on_connect_spawn(hades::system_job_data game_data)
{
	const auto &curves = game_data.level_data->get_curves();
	const auto pos = curves.float_vector_curves.get({ game_data.entity, global::position });
	const auto fpos = pos.get(game_data.current_time);
	global::pos_x = fpos[0];
	global::pos_y = fpos[1];
	return;
}

void spawn_system(hades::system_job_data game_data)
{
	//spawn a new ball at entities location 




	return;
}

void move_system(hades::system_job_data)
{
	return;
}

void create_curves(hades::data::data_manager &data)
{
	//create the movement curve
	const auto id = data.get_uid("move_d");

	using namespace hades::resources;

	auto move_curve = data.find_or_create<curve>(id, hades::unique_id::zero);

	move_curve->c_type = hades::curve_type::const_c;
	move_curve->data_type = hades::resources::curve_variable_type::vector_float;

	move_curve->default_value = curve_types::vector_float{ 0.f, 0.f };
}

void register_bounce_systems(hades::data::data_manager &data)
{
	//create a system that does nothing;
	hades::make_system("spawn_system", 
		on_create_spawn,
		on_connect_spawn,
		nullptr,
		spawn_system,
		nullptr,
		data);
}
