#include "taag_client.h"

Menu_contextuel_widget::Menu_contextuel_widget(class Game_engine* p_engine, jgl::Widget * p_parent) : jgl::Widget(p_parent)
{
	_engine = p_engine;
	_box = jgl::w_box_component(this);
}

void Menu_contextuel_widget::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_area(p_area);
	_box.set_anchor(p_anchor);
}

void Menu_contextuel_widget::render()
{
	_box.render(_viewport);
}
