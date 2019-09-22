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

static auto quad_map_id = hades::unique_id{};
using quad_map_t = hades::quad_tree<hades::game::object_ref, hades::rect_t<float_t>>;

struct quad_map
{
	quad_map_t value{ 50 };
};

namespace spawn
{
	void on_update()
	{
		//NOTE: object must have last_spawn_time curve
		constexpr hades::time_duration spawn_delay = hades::nanoseconds{ 1 };

		const auto& objs = hades::game::get_objects();
		const auto pos = hades::get_position_curve();
		const auto& q_map = hades::game::level::get_level_local_ref<quad_map>(quad_map_id);

		for (const auto o : objs)
		{
			//check when the last spawn time was
			using spawn_time_t = hades::resources::curve_types::int_t;
			const auto spawn_time = hades::game::level::get_keyframe<spawn_time_t>({ o, global::last_spawn_time_c });

			const auto time = hades::game::get_time();

			if (spawn_time.first + spawn_delay > time)
				continue;

			if (spawn_time.second > 500)
				continue;

			const auto p = hades::game::level::get_position(o);
			
			//create new ball object at x, y
			const auto ball_type = hades::data::get<hades::resources::object>(global::ball_o);
			auto ball = hades::make_instance(ball_type);
			hades::set_position(ball, p);
			const auto s = hades::get_size(ball);

			//check for collision
			const auto rect = hades::rect_float{ p, s };
			const auto others = q_map.value.find_collisions(rect);
			
			//if we collide, then skip until next tick
			if (std::any_of(std::begin(others), std::end(others), [rect](auto&& o) {
				return hades::collision_test(rect, o.rect);
			}))
			{
				continue;
			}

			//update the last spawn time
			hades::game::level::set_value({ o, global::last_spawn_time_c }, time, spawn_time.second + 1);
			hades::game::level::create_object(std::move(ball));
		}

		return;
	}
}

namespace move
{
	using namespace hades::resources::curve_types;
	
	static const auto quad_map_id = hades::unique_id{};

	void on_connect()
	{
		const auto &objs = hades::game::get_objects();

		for (auto o : objs)
		{
			//we must get a value before we can set it
			std::ignore = hades::game::level::get_value<vec2_float>({ o, global::move_d });

			//generate random move
			auto move = vec2_float{
				hades::random(-1.f, 1.f),
				hades::random(-1.f, 1.f)
			};

			hades::game::level::set_value({ o, global::move_d }, std::move(move));

			const auto rect = hades::world_rect_t{
				hades::game::level::get_position(o),
				hades::game::level::get_size(o)
			};

			//we must begin within the world bounds
			const auto world_bounds = hades::game::level::get_world_bounds();
			assert(hades::is_within(rect, world_bounds));

			//add to quadmap
			auto& map = hades::game::level::get_level_local_ref<quad_map>(quad_map_id);

			//TODO: if any_of collide, then quick kill this ent

			map.value.insert(rect, o);
		}
		return;
	}

	void on_disconnect()
	{
		const auto& objs = hades::game::get_objects();

		auto& map = hades::game::level::get_level_local_ref<quad_map>(quad_map_id);

		for (auto o : objs)
			map.value.remove(o);
		
		return;
	}

	void on_update()
	{
		const auto& objs = hades::game::get_objects();
		const auto time = hades::game::get_time();

		auto& map = hades::game::level::get_level_local_ref<quad_map>(quad_map_id);

		//TODO: generate move vector
		// try move
		// update variables
		for (auto o : objs)
		{
			const auto pos = hades::game::level::get_position(o);
			const auto siz = hades::game::level::get_size(o);
			const auto move = hades::game::level::get_value<vec2_float>({ o, global::move_d });

			const auto current_rect = hades::rect_float{ pos, siz };
			const auto search_area = [&move, &current_rect]() {
				auto centre_rect = hades::to_rect_centre(current_rect);
				centre_rect.half_width += move.x;
				centre_rect.half_height += move.y;
				return hades::to_rect(centre_rect);
			}();

			////get all nearby collision rects
			//auto map = hades::game::get_system_value<quad_map>(quad_map_id);
			//const auto other_rects = map.find_collisions(search_area);
			//auto others = std::vector<hades::rect_float>{};
			//others.reserve(std::size(other_rects));

			//std::transform(std::cbegin(other_rects), std::cend(other_rects), 
			//	std::back_inserter(others), [](auto &&r) {
			//	return r.rect;
			//});

			const auto full_move = move;
			//const auto [final_move, iter] = hades::safe_move(current_rect,
			//	full_move, std::begin(others), std::end(others));

			////TODO: handle collisions
			//if (iter != std::end(others))
			//{
			//	//compare magnitudes and then perform the bounce
			//}

			const auto [x, y] = pos + full_move;
			hades::game::level::set_position(o, x, y, time);

			//update collision box pos
		}
		return;
	}

	void on_destroy()
	{
		hades::game::destroy_system_data();
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
	move_curve->data_type = curve_variable_type::vec2_float;
	move_curve->default_value = curve_types::vec2_float{ 0.f, 0.f };

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
		move::on_connect,
		move::on_disconnect,
		move::on_update,
		move::on_destroy,
		data
	);
}
