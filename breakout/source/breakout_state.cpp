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
	//draw all
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

std::array<const hades::resources::animation*, 10> get_block_types()
{
	using anim = hades::resources::animation;
	std::array<const hades::resources::animation*, 10> out{ nullptr };

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
	game_elements e;

	//create walls
	
	//create paddle

	//create blocks
	const auto rows = 10;
	const auto block_types = get_block_types();

	//create ball

	return game_elements{};
}
