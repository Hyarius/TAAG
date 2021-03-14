#pragma once

#include "taag_common.h"

class Menu_contextuel_widget : public jgl::Widget
{
private:
	class Game_engine* _engine;

	jgl::w_box_component _box;

public:
	Menu_contextuel_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
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

	jgl::w_box_component _box;

public:
	Friend_list_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
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

public:
	Game_launcher_widget(class Game_engine *p_engine, jgl::Widget* p_parent = nullptr);
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
	Game_launcher_widget* _game_launcher_widget;

public:
	Game_menu(class Game_engine* p_engine, jgl::Widget *p_parent = nullptr);
	void add_chat_line(jgl::String text) { _chat_widget->add_line(text); }
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};