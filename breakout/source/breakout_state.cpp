#include "breakout_state.hpp"

#include "Hades/Animation.hpp"
#include "hades/collision.hpp"
#include "Hades/Data.hpp"
#include "Hades/Properties.hpp"
#include "hades/utility.hpp"
#include "Hades/System.hpp"

#include "input.hpp"

constexpr auto wall_size_x = 8;
constexpr auto wall_size_y = 8;

constexpr auto block_size_x = 16;
constexpr auto block_size_y = 8;
constexpr auto blocks_hori = 13;
constexpr auto blocks_vert = 30;

//max screen size is as many blocks can be placed horizontally
// times the size of the blocks, and adding a extra block(the extra block is the half sized wall
constexpr auto screen_x = block_size_x * blocks_hori + wall_size_x * 2;
//note: there is no wall at the bottom of the screen,
//the extra wall space is placed at the top, and used for score
constexpr auto screen_y = block_size_y * blocks_vert + wall_size_y * 2;

//play area limits, the ball cannot leave this rectangle
constexpr auto game_top = wall_size_y * 2;
constexpr auto game_bottm = screen_y;

constexpr auto game_left = wall_size_x;
constexpr auto game_right = screen_x - wall_size_x;

constexpr auto paddle_move_speed = 5.f;

constexpr std::tuple<hades::types::int32, hades::types::int32> screen_size()
{
	return { screen_x, screen_y };
}

void set_window_size()
{
	hades::console::SetProperty<hades::types::int32>("vid_width", screen_x);
	hades::console::SetProperty<hades::types::int32>("vid_height", screen_y);
	hades::console::SetProperty<bool>("vid_resizable", false);

	hades::console::RunCommand({ "vid_reinit" });
}

void breakout_game::init()
{
	_game_view = sf::View{ {0.f, 0.f, static_cast<float>(screen_x), static_cast<float>(screen_y)} };

	_sprites = _prepare_game();

	_ball_velocity.x = static_cast<float>(hades::random(-1, 1));
	_ball_velocity.y = -1;
}

bool breakout_game::handleEvent(const hades::event &)
{
	return false;
}

sf::Vector2i invert_direction(sf::FloatRect intersection, sf::Vector2i velocity)
{
	if (intersection.width > intersection.height)
		velocity.x *= -1;
	else
		velocity.y *= -1;

	return velocity;
}

void breakout_game::update(sf::Time, const sf::RenderTarget&, hades::input_system::action_set a)
{
	//apply user input
	auto move_left = a.find(input::names[input::move_left]);
	if (move_left != std::end(a) && move_left->active)
	{
		//move paddle left
		_sprites.paddle.move({ -paddle_move_speed, 0.f });
	}

	if (_sprites.paddle.getPosition().x < game_left)
		_sprites.paddle.setPosition({ static_cast<float>(game_left), _sprites.paddle.getPosition().y });

	auto move_right = a.find(input::names[input::move_right]);
	if (move_right != std::end(a) && move_right->active)
	{
		//move paddle right
		_sprites.paddle.move({ paddle_move_speed, 0.f });
	}

	const auto paddle_max = game_right - _sprites.paddle.getLocalBounds().width;
	if (_sprites.paddle.getPosition().x + _sprites.paddle.getLocalBounds().width > game_right)
		_sprites.paddle.setPosition({ static_cast<float>(paddle_max), _sprites.paddle.getPosition().y });
	
	//=========
	//move ball	
	//=========

	//generate ball collision data
	const hades::rect_t<float> ball_bounds{
		_sprites.ball.getPosition().x,
		_sprites.ball.getPosition().y,
		_sprites.ball.getLocalBounds().width,
		_sprites.ball.getLocalBounds().height
	};

	//find safest move
	auto &blocks = _sprites.blocks;
	auto iter = std::begin(blocks);
	hades::direction hit_direction{ hades::direction::last };
	auto move = _ball_velocity;
	while (iter != std::end(blocks))
	{
		const auto sf_bounds = iter->getGlobalBounds();
		const hades::rect_t<float> bounds{ sf_bounds.left, sf_bounds.top, sf_bounds.width, sf_bounds.height };
		const auto safe_move = hades::safe_move(ball_bounds, _ball_velocity, bounds);

		if (safe_move == _ball_velocity)
			++iter;
		else
		{
			iter = blocks.erase(iter);

			if (safe_move.x < move.x
				&& safe_move.y < move.y)
				move = safe_move;

			hit_direction = hades::collision_direction(ball_bounds, _ball_velocity, bounds);
		}
	}

	//move is now the longest safe move
	_sprites.ball.move({ move.x, move.y });

	using hades::direction;
	//direction is the edge we hit
	switch (hit_direction)
	{
	case direction::left:
	case direction::right:
		_ball_velocity.x *= -1.f;
		break;
	case direction::top:
	case direction::bottom:
		_ball_velocity.y *= -1.f;
		break;
	}

	//bounce off walls

	//bounce off roof

	//if collide with paddle then apply apply reflect logic

	//if touch bottom of game area then end.
}

void breakout_game::draw(sf::RenderTarget & target, sf::Time deltaTime)
{
	target.setView(_game_view);
	//draw all
	for (const auto w : _sprites.walls)
		target.draw(w);

	target.draw(_sprites.paddle);

	for (const auto b : _sprites.blocks)
		target.draw(b);

	target.draw(_sprites.ball);
}

void breakout_game::reinit()
{
}

void breakout_game::pause()
{
}

void breakout_game::resume()
{
}

std::array<const hades::resources::animation*, 6> get_block_types()
{
	using anim = hades::resources::animation;
	std::array<const anim*, 6> out{ nullptr };

	const auto red_block = hades::data::GetUid("block-red-anim");
	out[0] = hades::data::Get<anim>(red_block);

	const auto blue_block = hades::data::GetUid("block-blue-anim");
	out[1] = hades::data::Get<anim>(blue_block);

	const auto orange_block = hades::data::GetUid("block-orang-anim");
	out[2] = hades::data::Get<anim>(orange_block);

	const auto green_block = hades::data::GetUid("block-green-anim");
	out[3] = hades::data::Get<anim>(green_block);

	const auto purple_block = hades::data::GetUid("block-purpl-anim");
	out[4] = hades::data::Get<anim>(purple_block);

	const auto white_block = hades::data::GetUid("block-white-anim");
	out[5] = hades::data::Get<anim>(white_block);

	return out;
}

std::vector<sf::Sprite> make_walls(const hades::resources::animation &wall_anim)
{
	std::vector<sf::Sprite> out;

	const auto wall_length = wall_anim.width;

	//top
	sf::Sprite top_wall;
	hades::animation::Apply(&wall_anim, 0.f, top_wall);

	for(auto i = 0; i < screen_x; i += wall_length)
	{
		auto bit = top_wall;
		bit.setPosition({ static_cast<float>(i), 0.f });
		out.emplace_back(std::move(bit));
	}

	//left and right walls

	auto left_wall = top_wall;
	left_wall.rotate(90.f);
	auto right_wall = top_wall;
	right_wall.rotate(-90.f);

	const auto wall_height = wall_length;

	for (auto i = 0; i <= screen_y; i += wall_height)
	{
		auto lbit = left_wall;
		auto rbit = right_wall;
		
		const auto fi = static_cast<float>(i);

		lbit.setPosition({ static_cast<float>(wall_anim.height), fi });
		out.emplace_back(std::move(lbit));

		rbit.setPosition({ static_cast<float>(screen_x) - static_cast<float>(wall_anim.height), fi });
		out.emplace_back(std::move(rbit));
	}

	return out;
}

breakout_game::game_elements breakout_game::_prepare_game() const
{
	game_elements e{};

	using anim = hades::resources::animation;

	//create walls
	const auto wall_id = hades::data::GetUid("wall-anim");
	const auto wall_anim = hades::data::Get<anim>(wall_id);

	e.walls = make_walls(*wall_anim);

	//create paddle
	const auto paddle_id = hades::data::GetUid("paddle-anim");
	const auto paddle_anim = hades::data::Get<anim>(paddle_id);

	hades::animation::Apply(paddle_anim, 0.f, e.paddle);

	const auto screen_xf = static_cast<float>(screen_x);
	const auto screen_yf = static_cast<float>(screen_y);

	e.paddle.setPosition({ screen_xf / 2.f - e.paddle.getLocalBounds().width / 2.f,
		screen_yf - e.paddle.getLocalBounds().height * 2.f });

	//create blocks
	const auto rows = 10;
	const auto block_types = get_block_types();

	std::size_t row = 0;
	while (row < block_types.size())
	{
		const auto block_anim = block_types[row];
		for (auto i = 0; i < blocks_hori; ++i)
		{
			sf::Sprite block;
			hades::animation::Apply(block_anim, 0.f, block);

			const auto x = i * block_size_x + wall_size_x;
			const auto y = row * block_size_y + block_size_y * 3;

			block.setPosition({ static_cast<float>(x), static_cast<float>(y) });
			e.blocks.emplace_back(block);
		}

		++row;
	}

	//create ball
	const auto ball_id = hades::data::GetUid("ball-anim");
	const auto ball_anim = hades::data::Get<anim>(ball_id);

	hades::animation::Apply(ball_anim, 0.f, e.ball);

	const auto ball_start_x = screen_x - screen_x / 2;
	const auto ball_start_y = screen_y - screen_y / 2;

	e.ball.setPosition({ static_cast<float>(ball_start_x), static_cast<float>(ball_start_y) });

	return e;
}
