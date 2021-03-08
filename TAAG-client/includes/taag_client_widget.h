#pragma once

#include "jgl.h"

#define SERVER_HOST "127.0.0.1"
#define SERVER_PORT 37500

class Client : public jgl::Widget, public jgl::Client_interface<Server_message>
{
private:
	bool _connected_to_server;

public:
	Client(jgl::Widget* p_parent = nullptr);
	void connect_to_server();
	bool handle_keyboard();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
	void update();
};

extern Client* g_client;