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

class Character_selecter_widget : public jgl::Widget
{
private:
public:
	Character_selecter_widget(jgl::Widget* p_parent = nullptr);
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

class Team_widget : public jgl::Widget
{
private:
public:
	Team_widget(jgl::Widget* p_parent = nullptr);
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

class City_widget : public jgl::Widget
{
private:
public:
	City_widget(jgl::Widget* p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

class Game_menu :public jgl::Widget
{
private:
	City_widget* _city;
	Account_widget* _account;
	Team_widget* _team;
	Chat_widget* _chat;
	Character_selecter_widget* _character_selecter;
	Menu_contextuel_widget* _menu_contextuel;

public:
	Game_menu(jgl::Widget *p_parent = nullptr);
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

extern Game_menu* g_game_menu;