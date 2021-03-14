#include "taag_client.h"

Game_menu::Game_menu(Game_engine* p_engine, jgl::Widget* p_parent)
{
	_engine = p_engine;
	_background = new jgl::Frame(this);
	_background->activate();

	_contextual_menu = new Menu_contextuel_widget(_engine, this);
	_contextual_menu->activate();
	
	_account_widget = new Account_widget(_engine, this);
	_account_widget->activate();
	
	_friend_list_widget = new Friend_list_widget(_engine, this);
	_friend_list_widget->activate();
	
	_chat_widget = new Chat_widget(_engine, this);
	_chat_widget->activate();
	
	_game_launcher_widget = new Game_launcher_widget(_engine, this);
	_game_launcher_widget->activate();

	_background->set_layer(0);
	_contextual_menu->set_layer(1);
	_account_widget->set_layer(2);
	_friend_list_widget->set_layer(3);
	_chat_widget->set_layer(4);
	_game_launcher_widget->set_layer(5);
}

void Game_menu::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	jgl::Vector2 tmp = p_area / jgl::Vector2(396, 216);

	_background->set_geometry(jgl::Vector2(0, 0) * tmp, jgl::Vector2(396, 216) * tmp);

	_contextual_menu->set_geometry(jgl::Vector2(73, 2) * tmp, jgl::Vector2(321, 158) * tmp);
	_account_widget->set_geometry(jgl::Vector2(2, 2) * tmp, jgl::Vector2(70, 20) * tmp);
	_friend_list_widget->set_geometry(jgl::Vector2(2, 23) * tmp, jgl::Vector2(70, 191) * tmp);
	_chat_widget->set_geometry(jgl::Vector2(73, 161) * tmp, jgl::Vector2(267, 53) * tmp);
	_game_launcher_widget->set_geometry(jgl::Vector2(341, 161) * tmp, jgl::Vector2(53, 53) * tmp);
}

void Game_menu::render()
{

}