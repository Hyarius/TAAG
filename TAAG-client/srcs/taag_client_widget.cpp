#include "taag_client.h"

Client::Client(Game_engine* p_engine, jgl::Widget* p_parent) : jgl::Widget(p_parent), jgl::Client_interface<Server_message>()
{
	_engine = p_engine;
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
			_engine->set_client_error_message(text);
		}
		break;
		case Server_message::Server_refuse_sign_up:
		{
			LOG_MESSAGE("Server refused sign up");
			jgl::String text;

			text = msg.get_string();
			LOG_MESSAGE("Server send msg : " + text);
			_engine->set_client_error_message(text);
		}
		break;
		case Server_message::Server_accept_login:
		{
			LOG_MESSAGE("Server accepted login");
			Account* new_account = get_account_from_message(msg);
			LOG_MESSAGE("Server send account " + new_account->username + " with icon " + new_account->icon.str());
			_engine->set_client_error_message("");
			_engine->set_account(new_account);
			_engine->connect();

			jgl::Message<Server_message> result(Server_message::Friend_list_content);

			send(result);
		}
		break;
		case Server_message::Server_refuse_login:
		{
			LOG_MESSAGE("Server refused login");
			jgl::String text;

			text = msg.get_string();
			LOG_MESSAGE("Server send msg : " + text);
			_engine->set_client_error_message(text);
		}
		break;
		case Server_message::Chat_message:
		{
			jgl::String text;

			text = msg.get_string();
			_engine->add_chat_line(text);
		}
		break;
		case Server_message::Friend_list_content:
		{
			LOG_MESSAGE("Server send friend list content");
			_engine->parse_friend_list(msg);
		}
		break;
		case Server_message::Add_friend_to_list:
		{
			_engine->add_friend_to_list(msg);
		}
		break;
		case Server_message::Remove_friend_from_list:
		{
			_engine->remove_friend_from_list(msg);
		}
		break;
		case Server_message::Change_friend_state:
		{
			_engine->change_friend_state(msg);
		}
		break;
		case Server_message::Game_room_information:
		{
			Game_room* room = _engine->account()->room;

			if (room == nullptr)
			{
				room = new Game_room();
				_engine->account()->room = room;
			}

			_engine->account()->room->clean();
			while (msg.empty() == false)
			{
				jgl::String pseudo;
				jgl::Vector2 icon;

				msg >> icon;
				pseudo = msg.get_string();

				_engine->account()->room->add_player_info(pseudo, icon);
				std::cout << "Adding roommate " << pseudo << " with icon " << icon << std::endl;
			}
			_engine->update_room_menu();
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
