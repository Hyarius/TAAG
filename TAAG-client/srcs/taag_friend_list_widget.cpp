#include "taag_client.h"

void popup_add_friend_to_list(jgl::Data param)
{
	Game_engine* engine = param.acces<Game_engine*>(0);

	engine->popup_add_friend_to_list();
}

void delete_friend(jgl::Data param)
{
	Game_engine* engine = param.acces<Game_engine*>(0);
	jgl::w_text_component* name = param.acces<jgl::w_text_component*>(1);

	LOG_MESSAGE("Deleting one friend : " + name->text());

	engine->send_delete_friend(name->text());
}

Friend_list_widget::Friend_list_widget(class Game_engine* p_engine, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_engine = p_engine;
	_box = jgl::w_box_component(this);
	_friend_box_size = 0;
	_nb_friend_on_screen = 10;
	_friend_text_size = 16;
	_index = 0;

	_friend_box_sheet = new jgl::Sprite_sheet("ressources/texture/friend_list_background.png", jgl::Vector2(6, 3));

	_add_friend_button = new jgl::Button(popup_add_friend_to_list, _engine, this);
	_add_friend_button->set_corner_size(16);
	_add_friend_button->activate();
	_add_friend_button->set_text("Adding friend");

	for (size_t i = 0; i < _nb_friend_on_screen; i++)
	{
		_friend_box_array.push_back(jgl::w_box_component(this));
		_friend_box_array[i].set_tileset(_friend_box_sheet);
		_friend_box_array[i].set_corner_size(16);
		_friend_text_array.push_back(jgl::w_text_component("", this));
		_friend_text_array[i].set_horizontal_alignment(jgl::Horizontal_alignment::centred);
		_friend_text_array[i].set_vertical_alignment(jgl::Vertical_alignment::centred);
		_friend_delete_button_array.push_back(new jgl::Button(delete_friend, jgl::Data(2, _engine, &(_friend_text_array[i])), this));
		_friend_delete_button_array[i]->set_text("X");
		_friend_delete_button_array[i]->activate();
	}
	_set_friend_name_in_box();
}

void Friend_list_widget::_set_friend_name_in_box()
{
	for (size_t i = 0; i < _nb_friend_on_screen; i++)
	{
		Friend* target = nullptr;

		if (i + _index < _friend_list.size())
			target = _friend_list[i + _index];

		_friend_text_array[i].set_text(target != nullptr ? target->name : "");
		if (_friend_text_array[i].size() > _text_size)
			_friend_text_array[i].set_size(_text_size);

		_friend_delete_button_array[i]->activate();
		if (target != nullptr && target->connected == false)
		{
			_friend_box_array[i].set_delta(jgl::Color(0, 0, 0, 255));
		}
		else
		{
			_friend_box_array[i].set_delta(jgl::Color(0, 0, 0, 0));
			if (target == nullptr)
				_friend_delete_button_array[i]->desactivate();
		}
	}
}

Friend_list_widget::Friend* Friend_list_widget::find_friend(jgl::String name)
{
	for (size_t i = 0; i < _friend_list.size(); i++)
	{
		if (_friend_list[i]->name == name)
			return (_friend_list[i]);
	}
	return (nullptr);
}

void Friend_list_widget::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	jgl::Vector2 button_size = jgl::Vector2((p_area.x - 10), p_area.y / 16);
	_add_friend_button->set_geometry(5, button_size);

	_box.set_area(p_area);
	_box.set_anchor(p_anchor);
	_friend_box_size = jgl::Vector2(_box.area().x - _box.border() * 4, ((_box.area().y - _box.border() * 4 - _add_friend_button->area().y - 5) - 5 * (_nb_friend_on_screen - 1)) / _nb_friend_on_screen);
	_icon_size = jgl::Vector2(_friend_box_size.y, _friend_box_size.y);
	_delta = (_add_friend_button == nullptr ? 0 : jgl::Vector2(0.0f, _add_friend_button->area().y + 5));
	_text_size = _friend_box_size.y / 3;

	jgl::Vector2 delete_size = _friend_box_size.y / 3;
	for (size_t i = 0; i < _nb_friend_on_screen; i++)
	{
		jgl::Vector2 tmp_anchor = p_anchor + jgl::Vector2(5.0f, 5.0f + _add_friend_button->area().y + 5) + jgl::Vector2(0.0f, _friend_box_size.y + 5.0f) * i;

		_friend_box_array[i].set_area(_friend_box_size);
		_friend_box_array[i].set_anchor(tmp_anchor);
		_friend_text_array[i].set_area(_friend_box_size - jgl::Vector2(_icon_size.x + _friend_box_array[i].border() * 2, 0.0f));
		_friend_text_array[i].set_anchor(tmp_anchor + jgl::Vector2(_icon_size.x + 5, 0.0f));
		_friend_text_array[i].set_size(_text_size);
		_friend_delete_button_array[i]->set_geometry(jgl::Vector2(5.0f, 5.0f + _add_friend_button->area().y + 5) + jgl::Vector2(0.0f, _friend_box_size.y + 5.0f) * i + jgl::Vector2(_friend_box_size.x - delete_size.x, 0.0f), delete_size);
	}
	_add_friend_button->set_size(_text_size * 0.8);
}

void Friend_list_widget::render()
{
	_box.render(_viewport);

	for (size_t i = 0; i < _nb_friend_on_screen; i++)
	{
		Friend* target = nullptr;

		if (i + _index < _friend_list.size())
			target = _friend_list[i + _index];

		if (target != nullptr)
		{
			_friend_box_array[i].render(_viewport);
			_friend_text_array[i].render(_viewport);
		}
	}

	for (size_t i = 0; i < _nb_friend_on_screen; i++)
	{

		Friend* target = nullptr;

		if (i + _index < _friend_list.size())
			target = _friend_list[i + _index];

		if (target != nullptr)
		{
			jgl::Vector2 tmp_anchor = jgl::Vector2(5.0f, 5.0f + _add_friend_button->area().y + 5) + jgl::Vector2(0.0f, _friend_box_size.y + 5.0f) * i;
			Account::icon_sprite->draw(_friend_list[i + _index]->sprite, tmp_anchor + _friend_box_array[i].border() * 2, _icon_size - _friend_box_array[i].border() * 4, 1.0f, _viewport);
		}
	}
}

void Friend_list_widget::parse_friend_list(jgl::Message<Server_message>& msg)
{
	for (size_t i = 0; i < _friend_list.size(); i++)
		if (_friend_list[i] != nullptr)
			delete _friend_list[i];
	
	_friend_list.clear();

	while (msg.empty() == false)
	{
		jgl::String name;
		jgl::Vector2 sprite;
		bool is_connected = false;

		name = msg.get_string();
		msg >> sprite;
		msg >> is_connected;

		_friend_list.push_back(new Friend_list_widget::Friend(name, sprite, is_connected));
	}
	_set_friend_name_in_box();
}

void Friend_list_widget::change_friend_state(jgl::Message<Server_message>& msg)
{
	jgl::String name = msg.get_string();
	bool new_state;

	msg >> new_state;

	Friend* target = find_friend(name);
	if (target != nullptr)
		target->connected = new_state;
	_set_friend_name_in_box();
}

void Friend_list_widget::add_friend_to_list(jgl::Message<Server_message>& msg)
{
	jgl::String name = msg.get_string();
	jgl::Vector2 sprite;

	msg >> sprite;

	_friend_list.push_back(new Friend_list_widget::Friend(name, sprite, true));
	_set_friend_name_in_box();
}

void Friend_list_widget::remove_friend_from_list(jgl::Message<Server_message>& msg)
{
	jgl::String name = msg.get_string();

	Friend* target = find_friend(name);
	if (target != nullptr)
	{
		_friend_list.erase(_friend_list.find(target));
		delete target;
	}
	_set_friend_name_in_box();
}

bool Friend_list_widget::handle_mouse()
{
	if (is_pointed() == true)
	{
		if (g_mouse->wheel != 0)
		{
			_index -= g_mouse->wheel;

			int max_size = (_friend_list.size() > _nb_friend_on_screen ? static_cast<int>(_friend_list.size()) - _nb_friend_on_screen : 0);

			if (_index > max_size)
				_index = max_size;
			else if (_index < 0)
				_index = 0;

			_set_friend_name_in_box();
		}
	}
	return (false);
}