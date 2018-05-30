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
	const auto width = hades::console::GetInt("vid_width", 800);
	const auto height = hades::console::GetInt("vid_height", 600);

	const auto w = static_cast<float>(width->load());
	const auto h = static_cast<float>(height->load());

	_backdrop.setFillColor(sf::Color::White);
	_backdrop.setSize({ w, h });

	_rect.setPosition({ w / 2, margin });
	_rect.setSize({ 50.f, 50.f });
	_rect.setFillColor(sf::Color::Black);

	_circ.setPosition({ margin, h / 2 });
	_circ.setRadius(50.f);
	_circ.setFillColor(sf::Color::Black);

	_point.setPosition({ w - margin, h / 2 });
	_point.setSize({ 1.f, 1.f });
	_point.setFillColor(sf::Color::Black);

	sf::Vector2f middle{ w / 2, h / 2 };

	_my_circ.setPosition(middle);
	_my_circ.setRadius(25.f);
	_my_circ.setFillColor(safe_col);

	_my_rect.setPosition(middle);
	_my_rect.setSize({ 50.f, 50.f });
	_my_rect.setFillColor(safe_col);

	_my_point.setPosition(middle);
	_my_point.setSize({ 1.f, 1.f });
	_my_point.setFillColor(safe_col);

	_target = { middle.x, middle.y };
}

bool collision_game::handleEvent(const hades::event &)
{
	return false;
}

void collision_game::update(sf::Time t, const sf::RenderTarget&, hades::input_system::action_set a)
{
	static const auto change_shape = hades::data::GetUid("change_shape");

	_current += t;

	const auto ch_shape = a.find(change_shape);
	if (ch_shape->active && _current - _shape_change_time > sf::milliseconds(100))
	{
		_shape_change_time = _current;
		_shape = static_cast<shapes>((_shape + 1) % max_shape);
		LOG(hades::to_string(_shape));
	}

	static const auto mouse = hades::data::GetUid("mouse");

	const auto mouse_move = a.find(mouse);
	if (mouse_move->active)
		_target = { static_cast<float>(mouse_move->x_axis), static_cast<float>(mouse_move->y_axis) };

	//static collider info

	static const hades::circle_t<float> circle{_circ.getPosition().x, _circ.getPosition().y, _circ.getRadius() };
	static const hades::rect_t<float> rect{ _rect.getPosition().x, _rect.getPosition().y, _rect.getSize().x, _rect.getSize().y };
	static const hades::point_t<float> point{ _point.getPosition().x, _point.getPosition().y };

	//move circle
	const auto cir_pos = _my_circ.getPosition();
	const auto cir_rad = _my_circ.getRadius();

	const auto cir_pos_dir = _target - vector_f{ cir_pos.x, cir_pos.y };
	const auto cir_move = hades::vector::resize(cir_pos_dir, _speed);

	const hades::circle_t<float> my_circle_cur{ cir_pos.x, cir_pos.y, cir_rad };
	const hades::circle_t<float> my_circle_next{ cir_pos.x + cir_move.x, cir_pos.y + cir_move.y, cir_rad };

	const auto[circle_circle_collide, circle_circle_fix] = hades::collision_test(my_circle_cur, my_circle_next, circle);
	if (circle_circle_collide)
	{
		_my_circ.setPosition({ my_circle_next.x, my_circle_next.y });
		_my_circ.setFillColor(hit_col);
	}
	else
	{
		_my_circ.setPosition({ my_circle_next.x, my_circle_next.y });
		_my_circ.setFillColor(safe_col);
	}
	//move rect

	//move point


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
	static const auto width = hades::console::GetInt("vid_width", 800);
	static const auto height = hades::console::GetInt("vid_height", 600);

	_backdrop.setSize( {static_cast<float>(width->load()), static_cast<float>(height->load())} );
}

void collision_game::pause()
{
}

void collision_game::resume()
{
}