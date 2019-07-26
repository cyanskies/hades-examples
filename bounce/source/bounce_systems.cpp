#include "bounce_systems.hpp"

#include "hades/collision.hpp"
#include "hades/core_curves.hpp"
#include "hades/curve_extra.hpp"
#include "hades/game_system.hpp"
#include "hades/level_interface.hpp"
#include "hades/quad_map.hpp"
#include "Hades/rectangle_math.hpp"
#include "hades/simple_resources.hpp"

namespace global
{
	static auto ball_o = hades::unique_id::zero,
		last_spawn_time_c = hades::unique_id::zero,
		move_d = hades::unique_id::zero;
}

namespace spawn
{
	void on_update()
	{
		//NOTE: object must have last_spawn_time curve
		constexpr hades::time_duration spawn_delay = hades::seconds{ 2 };

		//check when the last spawn time was
		using spawn_time_t = hades::resources::curve_types::int_t;
		auto spawn_time = hades::game::level::get_curve<spawn_time_t>(global::last_spawn_time_c);

		const auto time = hades::game::get_time();
		const auto [last_spawn_time, spawn_count] = spawn_time.getPrevious(time);
	
		if (last_spawn_time + spawn_delay > time)
			return;

		const auto [x_c, y_c] = hades::get_position_curve_id();
		using position_t = hades::resources::curve_types::float_t;
		const auto x = hades::game::level::get_value<position_t>(x_c),
			y = hades::game::level::get_value<position_t>(y_c);

		//create new ball object at x, y with a random move_d
		const auto ball_type = hades::data::get<hades::resources::object>(global::ball_o);
		auto ball = hades::make_instance(ball_type);
		hades::game::level::create_object(std::move(ball));

		//update the last spawn time
		spawn_time.set(time, spawn_count + 1);
		hades::game::level::set_curve(global::last_spawn_time_c, std::move(spawn_time));

		return;
	}
}

namespace move
{
	using namespace hades::resources::curve_types;
	using quad_map = hades::quad_tree<object_ref, hades::rect_t<float_t>>;

	static auto quad_map_id = hades::unique_id{};

	void on_create()
	{
		hades::game::create_system_value(quad_map_id, quad_map{});
		return;
	}

	void on_connect()
	{
		//generate random move
		auto move = vector_float{
			hades::random(-1.f, 1.f),
			hades::random(-1.f, 1.f)
		};

		hades::game::level::set_value<vector_float>(global::move_d, std::move(move));
		
		const auto rect = hades::world_rect_t{
			hades::game::level::get_position(),
			hades::game::level::get_size()
		};

		//TODO: if we are being connected while in a colliding position then
		// we should kill this entity
		//add to quadmap
		auto map = hades::game::get_system_value<quad_map>(quad_map_id);
		map.insert(rect, hades::game::get_object());
		hades::game::set_system_value(quad_map_id, std::move(map));
		return;
	}

	void on_disconnect()
	{
		//remove from quadmap
		auto map = hades::game::get_system_value<quad_map>(quad_map_id);
		map.remove(hades::game::get_object());
		hades::game::set_system_value(quad_map_id, std::move(map));
		return;
	}

	void on_update()
	{
		//TODO: generate move vector
		// try move
		// update variables
		const auto pos = hades::game::level::get_position();
		const auto siz = hades::game::level::get_size();
		const auto move = hades::game::level::get_value<vector_float>(global::move_d);
		assert(std::size(move) == 2);

		const auto current_rect = hades::rect_float{ pos, siz };
		const auto search_area = [move, current_rect]() {
			auto centre_rect = hades::to_rect_centre(current_rect);
			centre_rect.half_width += move[0];
			centre_rect.half_width += move[1];
			return hades::to_rect(centre_rect);
		}();

		//get all nearby collision rects
		auto map = hades::game::get_system_value<quad_map>(quad_map_id);
		const auto other_rects = map.find_collisions(search_area);
		auto others = std::vector<hades::rect_float>{};
		others.reserve(std::size(other_rects));

		std::transform(std::cbegin(other_rects), std::cend(other_rects), 
			std::back_inserter(others), [](auto &&r) {
			return r.rect;
		});

		const auto final_move = hades::safe_move(current_rect, 
			hades::vector_t{ move[0], move[1] }, std::begin(others), std::end(others));

		

		return;
	}

	void on_destroy()
	{
		hades::game::destroy_system_value(quad_map_id);
		return;
	}
}

void register_bounce_resources(hades::data::data_manager &data)
{
	hades::register_objects(data);
	hades::register_game_system_resources(data);

	using namespace hades::resources;
	using namespace std::string_view_literals;

	//create the movement curve
	global::move_d = data.get_uid("move-dt"sv);	
	auto move_curve = data.find_or_create<curve>(global::move_d, hades::unique_id::zero);
	move_curve->c_type = hades::curve_type::step;
	//can be a vector, since we don't need to lerp this value
	move_curve->data_type = curve_variable_type::vector_float;
	move_curve->default_value = curve_types::vector_float{ 0.f, 0.f };

	global::last_spawn_time_c = data.get_uid("last-spawn-time"sv);
	auto last_spawn_curve = data.find_or_create<curve>(global::last_spawn_time_c, hades::unique_id::zero);
	last_spawn_curve->c_type = hades::curve_type::pulse;
	last_spawn_curve->data_type = curve_variable_type::int_t;
	last_spawn_curve->default_value = curve_types::int_t{};

	global::ball_o = data.get_uid("ball"sv);
}

void register_bounce_systems(hades::data::data_manager &data)
{
	hades::make_system("ball-spawn-system", 
		nullptr,
		nullptr,
		nullptr,
		spawn::on_update,
		nullptr,
		data
	);

	hades::make_system("bounce-move",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		data
	);
}
