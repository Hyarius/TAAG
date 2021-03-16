#pragma once

#include "jgl.h"

#include "taag_common.h"

#include "taag_client_widget.h"
#include "taag_title_screen_widget.h"
#include "taag_game_menu_widget.h"

class Game_engine
{
private:
	Account* _account;
	Client* _client;
	Title_screen* _title_screen;
	Game_menu* _game_menu;

public:
	Game_engine();
	~Game_engine();

	Account* account() { return (_account); }

	void send_chat_message(jgl::String message);
	void add_chat_line(jgl::String text) { _game_menu->add_chat_line(text); }

	void parse_friend_list(jgl::Message<Server_message>& msg) { _game_menu->parse_friend_list(msg); }
	void add_friend_to_list(jgl::Message<Server_message>& msg) { _game_menu->add_friend_to_list(msg); }
	void remove_friend_from_list(jgl::Message<Server_message>& msg) { _game_menu->remove_friend_from_list(msg); }
	void change_friend_state(jgl::Message<Server_message>& msg) { _game_menu->change_friend_state(msg); }

	void connect();
	void set_account(Account* p_account) { _account = p_account; }
	void set_client_error_message(jgl::String text);
	void send(jgl::Message<Server_message>& msg);
};