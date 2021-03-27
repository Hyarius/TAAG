#pragma once

#include "jgl.h"
#include "taag_common.h"

class Server : jgl::Server_interface<Server_message>
{
private:
	bool _quit;
	jgl::Array<jgl::Connexion<Server_message>*> _connected_client;

	std::map<int64_t, Account*> _client_to_account_map;

	std::map<jgl::String, Account*> _account_map;
	std::map<jgl::String, Account*> _pseudo_map;

	jgl::Array<Game_room *> _room_array[NB_PLAYER_PER_GAME];
	jgl::Array<Game_room *> _waiting_room_array[NB_PLAYER_PER_GAME];

public:
	Server();
	~Server();

	void save();
	void load();

	void send_room_information(Game_room* room);
	void send_friend_list(jgl::Connexion<Server_message>* client);
	void actualize_status(Account* account);

	void connect_account(jgl::Connexion<Server_message>* client, Account* account);
	void disconnect_account(jgl::Connexion<Server_message>* client, Account* account);

	bool find_account_in_map(jgl::String username);
	bool find_connected_client(jgl::Connexion<Server_message>* client);

	void send_chat_line_to_player(jgl::Connexion<Server_message>* client, jgl::String text);
	void send_chat_line(jgl::String text);

	void quit();
	void run();

	bool client_connect(jgl::Connexion<Server_message>* client);

	void client_disconnect(jgl::Connexion<Server_message>* client);

	void on_message_reception(jgl::Connexion<Server_message>* client, jgl::Message<Server_message>& msg);
};