#include "taag_client.h"

Game_launcher_widget::Game_launcher_widget(class Game_engine* p_engine, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_engine = p_engine;
	_box = jgl::w_box_component(this);
}

void Game_launcher_widget::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_area(p_area);
	_box.set_anchor(p_anchor);
}

void Game_launcher_widget::render()
{
	_box.render(_viewport);
}
