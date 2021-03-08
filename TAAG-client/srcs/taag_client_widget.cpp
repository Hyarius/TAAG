#include "taag_client.h"

Client::Client(jgl::Widget* p_parent) : jgl::Widget(p_parent), jgl::Client_interface<Server_message>()
{
	connect(SERVER_HOST, SERVER_PORT);
	_connected_to_server = false;
}
void Client::connect_to_server()
{
	if (is_connected() == true)
		disconnect();
	connect(SERVER_HOST, SERVER_PORT);
	_connected_to_server = false;
}

void Client::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{

}

void Client::render()
{

}

bool Client::handle_keyboard()
{
	if (_connected_to_server == false && jgl::get_key(jgl::key::F2) == jgl::key_state::release)
	{
		reconnect(SERVER_HOST, SERVER_PORT);
	}
	return(false);
}

void Client::update()
{
	if (_input.empty() == false)
	{
		auto msg = _input.pop_front().msg;

		switch (msg.type())
		{
		case Server_message::Server_ask_magic_number:
		{
			int64_t key;

			msg >> key;
			LOG_MESSAGE("Server started connection and send key " + std::to_string(key));

			int64_t result;

			result = compute_magic_number(key);

			msg.header.id = Server_message::Client_awnser_magic_number;
			msg << result;
			msg << key;
			send(msg);
		}
		break;
		case Server_message::Server_accept_connection:
		{
			LOG_MESSAGE("Server accepted connection");
			_connected_to_server = true;
		}
		break;
		case Server_message::Server_refuse_connection:
		{
			LOG_MESSAGE("Server refused connection");
			_connected_to_server = false;
		}
		break;
		case Server_message::Server_accept_sign_up:
		{
			LOG_MESSAGE("Server accepted sign up");
			jgl::String text;

			text = msg.get_string();
			LOG_MESSAGE("Server send msg : " + text);
			g_title_screen->set_client_error_message(text);
		}
		break;
		case Server_message::Server_refuse_sign_up:
		{
			LOG_MESSAGE("Server refused sign up");
			jgl::String text;

			text = msg.get_string();
			LOG_MESSAGE("Server send msg : " + text);
			g_title_screen->set_client_error_message(text);
		}
		break;
		case Server_message::Server_accept_login:
		{
			LOG_MESSAGE("Server accepted login");

			g_title_screen->set_client_error_message("");
		}
		break;
		case Server_message::Server_refuse_login:
		{
			LOG_MESSAGE("Server refused login");
			jgl::String text;

			text = msg.get_string();
			LOG_MESSAGE("Server send msg : " + text);
			g_title_screen->set_client_error_message(text);
		}
		break;
		default:
		{
			std::cout << "Error : Bad message receive of type " << static_cast<int>(msg.type()) << std::endl;
			break;
		}
		}
	}
}
