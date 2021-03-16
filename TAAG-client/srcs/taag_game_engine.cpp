#include "taag_client.h"

jgl::Sprite_sheet* Account::icon_sprite = nullptr;

Game_engine::Game_engine()
{
	_account = nullptr;
	_client = new Client(this, nullptr);
	_client->activate();

	_title_screen = new Title_screen(this, nullptr);
	_title_screen->set_geometry(0, g_application->size());
	_title_screen->activate();

	_game_menu = new Game_menu(this, nullptr);
	_game_menu->set_geometry(0, g_application->size());

	Account::set_icon_sprite(new jgl::Sprite_sheet("ressources/texture/account_icon.png", 10));
}

Game_engine::~Game_engine()
{
	if (_account != nullptr)
		delete _account;
	if (_client != nullptr)
		delete _client;
	if (_title_screen != nullptr)
		delete _title_screen;
	if (_game_menu != nullptr)
		delete _game_menu;
}

void Game_engine::send_chat_message(jgl::String message)
{
	jgl::Message<Server_message> msg(Server_message::Send_chat_message);

	msg.add_string(message);

	_client->send(msg);
}

void Game_engine::connect()
{
	_title_screen->desactivate();
	_game_menu->activate();
}

void Game_engine::set_client_error_message(jgl::String text)
{
	_title_screen->set_client_error_message(text);
}

void Game_engine::send(jgl::Message<Server_message>& msg)
{
	_client->send(msg);
}