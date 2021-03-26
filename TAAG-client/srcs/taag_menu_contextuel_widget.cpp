#include "taag_client.h"

Menu_contextuel_widget::Menu_contextuel_widget(class Game_engine* p_engine, jgl::Widget * p_parent) : jgl::Widget(p_parent)
{
	_engine = p_engine;
	_box = jgl::w_box_component(this);

	_game_room = new jgl::Contener(this);
	for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
	{
		_friend_box_array.push_back(jgl::w_box_component(_game_room));
		_friend_box_array[i].set_tileset(_engine->account_box_sheet(1));
		_friend_box_array[i].set_corner_size(16);
		_friend_text_array.push_back(jgl::w_text_component("", _game_room));
		_friend_text_array[i].set_horizontal_alignment(jgl::Horizontal_alignment::centred);
		_friend_text_array[i].set_vertical_alignment(jgl::Vertical_alignment::centred);
	}
}

void Menu_contextuel_widget::create_new_room()
{
	LOG_MESSAGE("Creating a new room - Menu method");
	_engine->create_new_room();
}

void Menu_contextuel_widget::update_room_menu()
{
	if (_game_room->is_active() == false)
		active_menu_room();
	update_room_menu_data();
}

void Menu_contextuel_widget::set_geometry_room_menu(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	jgl::Vector2 base = jgl::Vector2(p_area.x / 2, p_area.y / (NB_PLAYER_PER_GAME / 2));
	_friend_box_size = base * 0.8f;
	_icon_size = jgl::Vector2(_friend_box_size.y - _box.border() * 2, _friend_box_size.y - _box.border() * 2);
	_friend_text_size = _friend_box_size - jgl::Vector2(_icon_size.x, 0.0f);
	_room_text_size = static_cast<uint32_t>(_friend_box_size.y / 3);

	for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
	{
		jgl::Vector2 tmp_anchor = p_anchor + (base - _friend_box_size) / 2 + base * jgl::Vector2(i % 2, i / 2);
		_friend_box_array[i].set_area(_friend_box_size);
		_friend_box_array[i].set_anchor(tmp_anchor);
		_friend_text_array[i].set_area(_friend_text_size);
		_friend_text_array[i].set_anchor(tmp_anchor + jgl::Vector2(_icon_size.x, 0.0f));
		_friend_text_array[i].set_size(_room_text_size);
	}
}

void Menu_contextuel_widget::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	_box.set_area(p_area);
	_box.set_anchor(p_anchor);

	set_geometry_room_menu(p_anchor, p_area);
}

void Menu_contextuel_widget::update_room_menu_data()
{
	for (int i = 0; i < NB_PLAYER_PER_GAME; i++)
	{
		if (_engine->account() != nullptr && _engine->account()->room != nullptr)
		{
			Account_info* target = _engine->account()->room->players[i];
			if (target != nullptr)
			{
				_friend_text_array[i].set_text(target->pseudo);
				if (_friend_text_array[i].size() > _room_text_size)
					_friend_text_array[i].set_size(_room_text_size);
			}
		}
	}
}

void Menu_contextuel_widget::render_room_menu()
{
	for (int i = 0; i < NB_PLAYER_PER_GAME; i++)
	{
		_friend_box_array[i].render(_viewport);

		if (_engine->account() != nullptr && _engine->account()->room != nullptr)
		{
			Account_info* target = _engine->account()->room->players[i];
			if (target != nullptr)
			{
				_friend_text_array[i].render(_viewport);
				jgl::Vector2 tmp_anchor = _friend_box_array[i].anchor() + _icon_size / 2 + _box.border();
				Account::icon_sprite->draw_centred(target->icon, tmp_anchor, _icon_size - _box.border() * 2, 1.0f);
			}
		}
	}
}

void Menu_contextuel_widget::render()
{
	_box.render(_viewport);

	if (_game_room->is_active() == true)
		render_room_menu();
}
