#pragma once

#include "taag_common.h"

class Menu_contextuel_widget : public jgl::Widget
{
private:

public:
	Menu_contextuel_widget(jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Account_widget : public jgl::Widget
{
private:

public:
	Account_widget(jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Chat_widget : public jgl::Widget
{
private:

public:
	Chat_widget(jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Game_menu :public jgl::Widget
{
private:
	jgl::Frame* _background;
	Account_widget* _account_widget;

	Menu_contextuel_widget* _menu_contextuel;
	jgl::Button* _one_v_one_button;
	jgl::Button* _three_v_three_button;
	jgl::Button* _five_v_five_button;

public:
	Game_menu(jgl::Widget *p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

extern Game_menu* g_game_menu;