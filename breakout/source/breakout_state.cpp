#include "breakout_state.hpp"

#include "Hades/Animation.hpp"
#include "Hades/Data.hpp"

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

constexpr std::tuple<hades::types::int32, hades::types::int32> screen_size()
{
	return { screen_x, screen_y };
}

void breakout_game::init()
{
	const auto screen = screen_size();
	_game_view = sf::View{ {0.f, 0.f, static_cast<float>(std::get<0>(screen)), static_cast<float>(std::get<1>(screen))} };

	_sprites = _prepare_game();
}

bool breakout_game::handleEvent(const hades::Event &)
{
	return false;
}

void breakout_game::update(sf::Time deltaTime, const sf::RenderTarget &, hades::InputSystem::action_set)
{
	//move ball
	//if collide with wall then invert x/y velocity depending on direction

	//if collide with block then remove and invert velocity

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
	std::array<const hades::resources::animation*, 6> out{ nullptr };

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

breakout_game::game_elements breakout_game::_prepare_game() const
{
	game_elements e{};

	using anim = hades::resources::animation;

	//create walls
	const auto wall_id = hades::data::GetUid("wall-anim");
	const auto wall_anim = hades::data::Get<anim>(wall_id);

	//create paddle
	const auto paddle_id = hades::data::GetUid("paddle-anim");
	const auto paddle_anim = hades::data::Get<anim>(paddle_id);

	hades::animation::Apply(paddle_anim, 0.f, e.paddle);

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
			const auto y = row * block_size_y + block_size_y;

			block.setPosition({ static_cast<float>(x), static_cast<float>(y) });
			e.blocks.emplace_back(block);
		}

		++row;
	}

	//create ball
	const auto ball_id = hades::data::GetUid("ball-anim");
	const auto ball_anim = hades::data::Get<anim>(ball_id);

	hades::animation::Apply(ball_anim, 0.f, e.ball);

	return e;
}
