#include "taag_client.h"

Game_menu::Game_menu(jgl::Widget* p_parent)
{
	_city = new City_widget(this);
	_city->activate();

	_account = new Account_widget(this);
	_account->activate();

	_team = new Team_widget(this);
	_team->activate();

	_chat = new Chat_widget(this);
	_chat->activate();

	_character_selecter = new Character_selecter_widget(this);
	_character_selecter->activate();

	_menu_contextuel = new Menu_contextuel_widget(this);
	_menu_contextuel->activate();

}

void Game_menu::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	jgl::Vector2 tmp = p_area / jgl::Vector2(192, 108);

	_city->set_geometry(tmp * jgl::Vector2(0, 0), tmp * jgl::Vector2(0, 0));
	_account->set_geometry(tmp * jgl::Vector2(0, 0), tmp * jgl::Vector2(0, 0));
	_team->set_geometry(tmp * jgl::Vector2(0, 0), tmp * jgl::Vector2(0, 0));
	_chat->set_geometry(tmp * jgl::Vector2(0, 74), tmp * jgl::Vector2(0, 0));
	_character_selecter->set_geometry(tmp * jgl::Vector2(58, 23), tmp * jgl::Vector2(0, 0));
	_menu_contextuel->set_geometry(tmp * jgl::Vector2(58, 0), tmp * jgl::Vector2(0, 0));
}

void Game_menu::render()
{

}