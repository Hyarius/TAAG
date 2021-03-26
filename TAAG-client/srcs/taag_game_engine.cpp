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

	_account_box_sheet[0] = new jgl::Sprite_sheet("ressources/texture/friend_list_background_disconnected.png", jgl::Vector2(6, 3));
	_account_box_sheet[1] = new jgl::Sprite_sheet("ressources/texture/friend_list_background_connected.png", jgl::Vector2(6, 3));
	_account_box_sheet[2] = new jgl::Sprite_sheet("ressources/texture/friend_list_background_in_room.png", jgl::Vector2(6, 3));
	_account_box_sheet[3] = new jgl::Sprite_sheet("ressources/texture/friend_list_background_in_game.png", jgl::Vector2(6, 3));
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

void Game_engine::create_new_room()
{
	LOG_MESSAGE("Asking for the creation of a new room");
	jgl::Message<Server_message> msg(Server_message::Create_new_room);
	_client->send(msg);
}

void Game_engine::join_room(jgl::String name)
{
	LOG_MESSAGE("Joining existing room of player " + name);
	jgl::Message<Server_message> msg(Server_message::Join_room);
	msg.add_string(name);
	_client->send(msg);
}

void Game_engine::leave_room()
{
	LOG_MESSAGE("Leaving actual room");
	jgl::Message<Server_message> msg(Server_message::Leave_room);
	_client->send(msg);
}

void Game_engine::send_chat_message(jgl::String message)
{
	jgl::Message<Server_message> msg(Server_message::Send_chat_message);

	msg.add_string(message);

	_client->send(msg);
}

void Game_engine::send_delete_friend(jgl::String name)
{
	jgl::Message<Server_message> msg(Server_message::Remove_friend_from_list);

	msg.add_string(name);

	_client->send(msg);
}

void Game_engine::send_new_friend(jgl::String name)
{
	jgl::Message<Server_message> msg(Server_message::Add_friend_to_list);

	msg.add_string(name);

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