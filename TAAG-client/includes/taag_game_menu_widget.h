#pragma once

#include "taag_common.h"

class Menu_contextuel_widget : public jgl::Widget
{
private:
	class Game_engine* _engine;

	jgl::w_box_component _box;

	jgl::Contener* _game_room;
	int _room_text_size;
	jgl::Vector2 _icon_size;
	jgl::Vector2 _friend_box_size;
	jgl::Vector2 _friend_text_size;
	jgl::Array<jgl::w_box_component> _friend_box_array;
	jgl::Array<jgl::w_text_component> _friend_text_array;
	jgl::Button* _start_game;
	jgl::Button* _leave_room;
	jgl::Button* _stop_matchmaking_button;

	jgl::Contener* _active_menu;

public:
	Menu_contextuel_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);

	void active_menu_room()
	{
		LOG_MESSAGE("Activing game menu");
		if (_active_menu != nullptr)
			_active_menu->desactivate();

		_game_room->activate();
		_active_menu = _game_room;
	}

	void set_room_leader() { _start_game->activate(); }

	void leave_room();

	void update_room_menu_data();
	void update_room_menu();
	void create_new_room();

	void set_geometry_room_menu(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);

	void render_room_menu();
	void render();
};

class Account_widget : public jgl::Widget
{
private:
	class Game_engine* _engine;

	jgl::w_box_component _box;

	jgl::String _prev_username;
	float _border;
	uint32_t _text_size;
	jgl::Vector2 _text_pos;
	jgl::Vector2 _icon_pos;
	jgl::Vector2 _icon_size;

public:
	Account_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);
	void calc_account_data_position(jgl::Vector2 p_area);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Friend_list_widget: public jgl::Widget
{
private:
	class Game_engine* _engine;

	Friend* find_friend(jgl::String name);

	jgl::Array<Friend *> _friend_list;

	jgl::Button* _add_friend_button;
	jgl::Button* _remove_friend_button;

	int _index;
	int _text_size;
	jgl::w_box_component _box;
	jgl::Vector2 _icon_size;
	jgl::Vector2 _delta;
	jgl::Vector2 _friend_box_size;
	int _nb_friend_on_screen;
	int _friend_text_size;

	jgl::Array<jgl::w_box_component> _friend_box_array;
	jgl::Array<jgl::w_text_component> _friend_text_array;
	jgl::Array<jgl::Button*> _friend_delete_button_array;

	void _set_friend_name_in_box();

public:
	Friend_list_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);

	void parse_friend_list(jgl::Message<Server_message>&msg);
	void change_friend_state(jgl::Message<Server_message>& msg);
	void add_friend_to_list(jgl::Message<Server_message>& msg);
	void remove_friend_from_list(jgl::Message<Server_message>& msg);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);

	bool handle_mouse();

	void render();
};

class Chat_widget : public jgl::Widget
{
private:
	class Game_engine* _engine;

	struct Chat_line
	{
		jgl::String line;
		jgl::Array<jgl::Image*> text_images;

		Chat_line()
		{
			line = "";
		}
		Chat_line(jgl::String text)
		{
			line = text;
		}
	};

	int _starting_line;
	Uint32 _next_input;
	Uint32 _input_delay;
	jgl::Array<Chat_line> _lines;

	size_t _text_size;
	size_t _nb_line;
	jgl::w_box_component _box;

	void generate(size_t index);

	void reset_line();

	void clear_line();

	bool _console_selected;
	jgl::w_box_component _entry_box;
	jgl::w_text_entry_component _entry;
	jgl::Vector2 _start_pos;
public:
	Chat_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);
	void add_line(jgl::String new_line);

	bool handle_keyboard();
	bool handle_mouse();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Game_launcher_widget : public jgl::Widget
{
private:
	class Game_engine* _engine;

	jgl::w_box_component _box;
	jgl::w_text_component _label;

public:
	Game_launcher_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Adding_friend_menu : public jgl::Widget
{
private:
	class Game_engine* _engine;

	jgl::Frame* _adding_friend_menu_frame;
	jgl::Text_label* _adding_friend_menu_label;
	jgl::Text_entry* _adding_friend_menu_entry;
	jgl::Button* _adding_friend_menu_validate_button;
	jgl::Button* _adding_friend_menu_cancel_button;

public:
	Adding_friend_menu(class Game_engine* p_engine, jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Game_menu :public jgl::Widget
{
private:
	class Game_engine* _engine;
	jgl::Frame* _background;

	Menu_contextuel_widget* _contextual_menu;
	Account_widget* _account_widget;
	Friend_list_widget* _friend_list_widget;
	Chat_widget* _chat_widget;
	jgl::Button* _game_launcher_button;

	Adding_friend_menu* _adding_friend_menu;

public:
	Game_menu(class Game_engine* p_engine, jgl::Widget *p_parent = nullptr);

	void close_add_friend_popup();
	void popup_add_friend_to_list();
	void active_menu_contextuel_room() { _contextual_menu->active_menu_room(); }

	void leave_room() { _contextual_menu->leave_room(); }
	void update_room_menu() { _contextual_menu->update_room_menu(); }

	void parse_friend_list(jgl::Message<Server_message>& msg) { _friend_list_widget->parse_friend_list(msg); }
	void add_friend_to_list(jgl::Message<Server_message>& msg) { _friend_list_widget->add_friend_to_list(msg); }
	void remove_friend_from_list(jgl::Message<Server_message>& msg) { _friend_list_widget->remove_friend_from_list(msg); }
	void change_friend_state(jgl::Message<Server_message>& msg) { _friend_list_widget->change_friend_state(msg); }

	void add_chat_line(jgl::String text) { _chat_widget->add_line(text); }

	bool handle_keyboard();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};