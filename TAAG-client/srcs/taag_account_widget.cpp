#include "taag_client.h"

Account_widget::Account_widget(class Game_engine* p_engine, jgl::Widget * p_parent) : jgl::Widget(p_parent)
{
	_engine = p_engine;
	_box = jgl::w_box_component(this);

	_prev_username = "";
	_border = 7;
	_text_size = 16u;
	_text_pos = 0;
	_icon_size = 0;
}

void Account_widget::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_area(p_area);
	_box.set_anchor(p_anchor);
	calc_account_data_position(p_area);
}

void Account_widget::calc_account_data_position(jgl::Vector2 p_area)
{
	if (_engine->account() == nullptr)
		return;

	_border = (p_area.y - _box.border() * 2) / 18;
	_icon_pos = _border + _box.border();
	_icon_size = (p_area.y - _box.border() * 2) - _border * 2;
	_text_pos = (p_area - _box.border() * 2 - (_border * 2) - jgl::Vector2(_icon_size.x, 0.0f)) / 2 + jgl::Vector2(_icon_size.x, 0.0f) + _box.border();
	_text_size = ((p_area.y - _box.border() * 2) - _border * 3) / 2;
	_prev_username = _engine->account()->username;
}

void Account_widget::render()
{
	_box.render(_viewport);

	if (_engine->account() != nullptr && Account::icon_sprite != nullptr)
	{
		if (_prev_username != _engine->account()->username)
			calc_account_data_position(_viewport->area());

		Account::icon_sprite->draw(_engine->account()->icon, _icon_pos, _icon_size, 1.0f, _viewport);
		jgl::draw_centred_text(_engine->account()->username, _text_pos / jgl::Vector2(1, 2), _text_size, 0, 1.0, jgl::text_color::black, jgl::text_style::normal, _viewport);
	}
}
