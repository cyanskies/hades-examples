#include "collision_state.hpp"

#include "hades/collision.hpp"
#include "Hades/Data.hpp"
#include "Hades/Logging.hpp"
#include "Hades/Properties.hpp"

const float margin = 50.f;
const auto safe_col = sf::Color::Green;
const auto hit_col = sf::Color::Red;

void collision_game::init()
{
	const auto width = hades::console::get_int("vid_width", 800);
	const auto height = hades::console::get_int("vid_height", 600);

	const auto w = static_cast<float>(width->load());
	const auto h = static_cast<float>(height->load());

	_backdrop.setFillColor(sf::Color::White);
	_backdrop.setSize({ w, h });

	_rect.setPosition({ w / 2, margin });
	_rect.setSize({ 50.f, 50.f });
	_rect.setFillColor(sf::Color::Black);

	_circ.setOrigin({ 50.f, 50.f });
	_circ.setPosition({ margin, h / 2 });
	_circ.setRadius(50.f);
	_circ.setFillColor(sf::Color::Black);

	_point.setPosition({ w - margin, h / 2 });
	_point.setSize({ 1.f, 1.f });
	_point.setFillColor(sf::Color::Black);

	sf::Vector2f middle{ w / 2, h / 2 };


	_my_circ.setOrigin({ 25.f, 25.f });
	_my_circ.setPosition(middle);
	_my_circ.setRadius(25.f);

	_my_rect.setPosition(middle);
	_my_rect.setSize({ 50.f, 50.f });

	_my_point.setPosition(middle);
	_my_point.setSize({ 2.f, 2.f });
	//_my_point.setSize({ 1.f, 1.f });
	
	_target = { middle.x, middle.y };
}

bool collision_game::handleEvent(const hades::event &)
{
	return false;
}

template<typename T, typename U>
std::tuple<bool, vector_f> move_object(vector_f move, T my_obj, U other)
{
	const auto new_move = hades::safe_move(my_obj, move, other);
	const bool collision{ move != new_move };
	return { collision, new_move };
}

template<typename T, typename U, typename V, typename W>
std::tuple<vector_f, bool> move_object(vector_f target, T o, U a, V b, W c)
{
	constexpr auto speed = 5.f;

	const auto pos_dir = target - vector_f{ o.x, o.y };
	auto move = hades::vector::resize(pos_dir, speed);
	if (const auto dist = hades::vector::distance({ o.x, o.y }, target); dist < speed)
		move = hades::vector::resize(pos_dir, dist);
		
	const auto[collide_a, move_a] = move_object(move, o, a);
	if (collide_a)
		move = move_a;

	const auto[collide_b, move_b] = move_object(move, o, b);
	if (collide_b)
		move = move_b;

	const auto[collide_c, move_c] = move_object(move, o, c);
	if (collide_c)
		move = move_c;

	return { move, collide_a || collide_b || collide_c };
}

void collision_game::update(sf::Time t, const sf::RenderTarget&, hades::input_system::action_set a)
{
	static const auto safe_col = sf::Color::Green;
	static const auto hit_col = sf::Color::Red;

	static const auto change_shape = hades::data::get_uid("change_shape");

	_current += t;

	const auto ch_shape = a.find(change_shape);
	if (ch_shape->active && _current - _shape_change_time > sf::milliseconds(100))
	{
		_shape_change_time = _current;
		_shape = static_cast<shapes>((_shape + 1) % max_shape);
	}

	static const auto mouse = hades::data::get_uid("mouse");

	const auto mouse_move = a.find(mouse);
	if (mouse_move->active)
		_target = { static_cast<float>(mouse_move->x_axis), static_cast<float>(mouse_move->y_axis) };

	//static collider info
	static const hades::circle_t<float> circle{_circ.getPosition().x, _circ.getPosition().y, _circ.getRadius() };
	static const hades::rect_t<float> rect{ _rect.getPosition().x, _rect.getPosition().y, _rect.getSize().x, _rect.getSize().y };
	static const hades::point_t<float> point{ _point.getPosition().x, _point.getPosition().y };

	//move circle
	const hades::circle_t<float> my_circle{ _my_circ.getPosition().x, _my_circ.getPosition().y, _my_circ.getRadius() };
	const auto [moved_circle, circle_hit] = move_object(_target, my_circle, circle, rect, point);
	_my_circ.move(moved_circle.x, moved_circle.y);
	if (circle_hit)
		_my_circ.setFillColor(hit_col);
	else
		_my_circ.setFillColor(safe_col);
	
	//move rect
	const auto rect_bounds = _my_rect.getGlobalBounds();
	const hades::rect_t<float> my_rect{ rect_bounds.left, rect_bounds.top, rect_bounds.width, rect_bounds.height };
	const auto[rect_move, rect_hit] = move_object(_target, my_rect, circle, rect, point);
	_my_rect.move(rect_move.x, rect_move.y);
	if (rect_hit)
		_my_rect.setFillColor(hit_col);
	else
		_my_rect.setFillColor(safe_col);

	//move point
	const vector_f my_point{ _my_point.getPosition().x, _my_point.getPosition().y };
	const auto[point_move, point_hit] = move_object(_target, my_point, circle, rect, point);
	_my_point.move(point_move.x, point_move.y);
	if (point_hit)
		_my_point.setFillColor(hit_col);
	else
		_my_point.setFillColor(safe_col);

}

void collision_game::draw(sf::RenderTarget & target, sf::Time deltaTime)
{
	target.draw(_backdrop);

	target.draw(_rect);
	target.draw(_circ);
	target.draw(_point);

	if (_shape == shapes::circle)
		target.draw(_my_circ);
	else if (_shape == shapes::rect)
		target.draw(_my_rect);
	else if (_shape == shapes::point)
		target.draw(_my_point);
}

void collision_game::reinit()
{
	static const auto width = hades::console::get_int("vid_width", 800);
	static const auto height = hades::console::get_int("vid_height", 600);

	_backdrop.setSize( {static_cast<float>(width->load()), static_cast<float>(height->load())} );
}

void collision_game::pause()
{
}

void collision_game::resume()
{
}