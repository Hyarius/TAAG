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

public:
	Server();
	~Server();

	void save();
	void load();

	void send_friend_list(jgl::Connexion<Server_message>* client);

	bool find_account_in_map(jgl::String username);
	bool find_connected_client(jgl::Connexion<Server_message>* client);

	void quit();
	void run();

	bool client_connect(jgl::Connexion<Server_message>* client);

	void client_disconnect(jgl::Connexion<Server_message>* client);

	void on_message_reception(jgl::Connexion<Server_message>* client, jgl::Message<Server_message>& msg);
};