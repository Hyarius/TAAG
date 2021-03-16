#include "taag_client.h"

Friend_list_widget::Friend_list_widget(class Game_engine* p_engine, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_engine = p_engine;
	_box = jgl::w_box_component(this);
	_friend_box_size = 0;
	_nb_friend_on_screen = 10;
	_friend_text_size = 16;

	_add_friend_button = nullptr;
	_remove_friend_button = nullptr;
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
	_box.set_area(p_area);
	_box.set_anchor(p_anchor);
	_friend_box_size = jgl::Vector2(_box.area().x - _box.border() * 4, ((_box.area().y - _box.border() * 4) - 5 * (_nb_friend_on_screen - 1)) / _nb_friend_on_screen);
}

void Friend_list_widget::draw_friend_box(Friend* target, int i)
{
	jgl::Vector2 delta = (_add_friend_button == nullptr ? 0 : jgl::Vector2(0.0f, _add_friend_button->area().y + 5));
	jgl::fill_rectangle(jgl::Vector2(0.0f, (_friend_box_size.y + 5) * i) + _box.border() * 2 + delta, _friend_box_size, jgl::Color(255, 0, 0), _viewport);
}

void Friend_list_widget::render()
{
	_box.render(_viewport);

	for (size_t i = 0; i < _friend_list.size(); i++)
	{
		draw_friend_box(_friend_list[i], i);
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
		jgl::String name = msg.get_string();
		jgl::Vector2 sprite;

		msg >> sprite;

		_friend_list.push_back(new Friend_list_widget::Friend(name, sprite, true));
	}
	for (size_t i = 0; i < _friend_list.size(); i++)
	{
		std::cout << _engine->account()->username << " is friend with " << _friend_list[i]->name << std::endl;
	}
}

void Friend_list_widget::change_friend_state(jgl::Message<Server_message>& msg)
{
	jgl::String name = msg.get_string();
	bool new_state;

	msg >> new_state;

	Friend* target = find_friend(name);
	if (target != nullptr)
		target->connected = new_state;
}

void Friend_list_widget::add_friend_to_list(jgl::Message<Server_message>& msg)
{
	jgl::String name = msg.get_string();
	jgl::Vector2 sprite;

	msg >> sprite;

	_friend_list.push_back(new Friend_list_widget::Friend(name, sprite, true));
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
}
