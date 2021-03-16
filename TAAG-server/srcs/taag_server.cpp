#include "taag_server.h"

Server::Server() : jgl::Server_interface<Server_message>(37500)
{
	_quit = false;
	load();
	start();
}

Server::~Server()
{
	_quit = true;
	save();
	for (auto tmp : _account_map)
	{
		if (tmp.second != nullptr)
			delete tmp.second;
	}
	stop();
}

bool Server::client_connect(jgl::Connexion<Server_message>* client)
{
	LOG_MESSAGE("Starting connection");
	jgl::Message<Server_message> msg(Server_message::Server_ask_magic_number);
	int64_t key = jgl::generate_nbr(0, 150000);
	msg << key;
	client->send(msg);
	return (true);
}

void Server::client_disconnect(jgl::Connexion<Server_message>* client)
{
	LOG_MESSAGE("Starting disconnection");
	if (_client_to_account_map[client->id()] != nullptr)
	{
		_client_to_account_map[client->id()]->client = nullptr;
		_client_to_account_map[client->id()] = nullptr;
	}
}

bool Server::find_connected_client(jgl::Connexion<Server_message>* client)
{
	for (size_t i = 0; i < _connected_client.size(); i++)
	{
		if (_connected_client[i] != nullptr && _connected_client[i]->id() == client->id())
			return (true);
	}
	return (false);
}

bool Server::find_account_in_map(jgl::String username)
{
	if (_account_map.count(username) != 0)
	{
		return (true);
	}
	else
		return (false);
}

void Server::send_friend_list(jgl::Connexion<Server_message>* client)
{
	Account* account = _client_to_account_map[client->id()];

	std::cout << "Account : " << account->username << std::endl;

	jgl::Message<Server_message> to_send(Server_message::Friend_list_content);

	for (size_t i = 0; i < account->friend_list.size(); i++)
	{
		Account* target = _account_map[account->friend_list[i]];

		to_send << target->icon;
		to_send.add_string(target->username);
	}

	client->send(to_send);
}

void Server::on_message_reception(jgl::Connexion<Server_message>* client, jgl::Message<Server_message>& msg)
{
	if (msg.type() != Server_message::Client_awnser_magic_number)
	{
		if (find_connected_client(client) == false)
		{
			LOG_MESSAGE("Received message from rogue client : ");
			return;
		}
	}
	switch (msg.type())
	{
		case Server_message::Client_awnser_magic_number:
		{
			int64_t key;
			int64_t presumed_result;

			msg >> key;
			msg >> presumed_result;
			jgl::String text;
			text = "Client send back key " + jgl::itoa(key) + " and result " + jgl::itoa(presumed_result);

			LOG_MESSAGE(text);
			int64_t result;

			result = compute_magic_number(key);

			if (result == presumed_result)
			{
				msg.header.id = Server_message::Server_accept_connection;
				_connected_client.push_back(client);
				LOG_MESSAGE("Client validation completed");
			}
			else
			{
				msg.header.id = Server_message::Server_refuse_connection;
				LOG_MESSAGE("Client validation failed");
			}
			client->send(msg);
		}
		break;
		case Server_message::Client_ask_sign_in:
		{
			jgl::Message<Server_message> result(Server_message::Server_error_message);
			LOG_MESSAGE("Account data received");

			jgl::String username;
			jgl::String password;

			username = msg.get_string();
			password = msg.get_string();

			LOG_MESSAGE("Username received : " + username);
			LOG_MESSAGE("Password received : " + password);

			if (find_account_in_map(username) == false)
			{
				LOG_MESSAGE("Can't find username : " + username);
				result.header.id = Server_message::Server_refuse_login;
				result.add_string("Can't find username : " + username);
			}
			else
			{
				Account* tmp_account = _account_map[username];
				if (tmp_account->password == password)
				{
					LOG_MESSAGE("Can login " + username);
					result.header.id = Server_message::Server_accept_login;
					add_account_to_message(result, tmp_account);
					_client_to_account_map[client->id()] = tmp_account;
					tmp_account->client = client;
				}
				else
				{
					LOG_MESSAGE("Wrong password to connect to account " + username);
					result.header.id = Server_message::Server_refuse_login;
					result.add_string("Wrong password to connect to account " + username);
				}
			}
			client->send(result);
		}
		break;
		case Server_message::Client_ask_sign_up:
		{
			jgl::Message<Server_message> result(Server_message::Server_error_message);
			LOG_MESSAGE("Client ask for a new account");

			jgl::String username;
			jgl::String password;

			username = msg.get_string();
			password = msg.get_string();

			LOG_MESSAGE("Username given : " + username);
			LOG_MESSAGE("Password given : " + password);

			if (find_account_in_map(username) == true)
			{
				LOG_MESSAGE("Username [" + username + "] already exist !");
				result.header.id = Server_message::Server_refuse_sign_up;
				result.add_string("Username already taken");
			}
			else
			{
				Account* new_account = new Account(username, password);
				_account_map[username] = new_account;
				LOG_MESSAGE("Username [" + username + "] create a new account !");
				result.header.id = Server_message::Server_accept_sign_up;
				result.add_string("Account successfully created");
			}
			client->send(result);
		}
		break;
		case Server_message::Send_chat_message:
		{
			if (_client_to_account_map[client->id()] != nullptr)
			{
				jgl::String username = _client_to_account_map[client->id()]->username;

				jgl::String tmp = msg.get_string();

				jgl::String result = "[" + username + "] : " + tmp;

				jgl::Message<Server_message> to_send(Server_message::Chat_message);

				to_send.add_string(result);

				LOG_MESSAGE("Player " + username + " : " + tmp);

				message_all(to_send, nullptr);
			}
		}
		break;
		case Server_message::Friend_list_content:
		{
			LOG_MESSAGE("Client asked for friend list");
			send_friend_list(client);
		}
		break;
		case Server_message::Add_friend_to_list:
		{
			Account* account = _client_to_account_map[client->id()];

			jgl::String name = msg.get_string();

			Account* other = _account_map[name];

			account->friend_list.push_back(name);
			other->friend_list.push_back(account->username);

			send_friend_list(client);
			if (other->client != nullptr)
				send_friend_list(other->client);
		}
		break;
		case Server_message::Remove_friend_from_list:
		{

		}
		break;
		default:
		{
			LOG_MESSAGE("Error : Bad message receive of type " + jgl::itoa(static_cast<int>(msg.type())));
			break;
		}
	}
}

void Server::quit()
{
	_quit = true;
}

void Server::run()
{
	while (_quit == false)
	{
		jgl::Server_interface<Server_message>::update();
	}
}

void Server::save()
{
	LOG_MESSAGE("Saving server data");
	std::fstream file = jgl::open_file(ACCOUNT_FILE, std::ios_base::out | std::ios_base::trunc);

	for (auto tmp : _account_map)
	{
		if (tmp.second != nullptr)
		{
			tmp.second->save(file);
			file << std::endl;
			LOG_MESSAGE("Saving account [" + tmp.second->username + "]/[" + tmp.second->password + "] to loaded account");
		}
		else
		{
			LOG_MESSAGE("Account named : " + tmp.first + " was nullptr");
		}
	}
}

void Server::load()
{
	static jgl::Array<jgl::String> tab;
	LOG_MESSAGE("Loading server data");

	if (jgl::check_file_exist(ACCOUNT_FILE) == true)
	{
		std::fstream file = jgl::open_file(ACCOUNT_FILE, std::ios_base::in);

		LOG_MESSAGE("Starting parsing account file");
		while (file.eof() == false)
		{
			jgl::String line = jgl::get_str(file);
			if (line.size() != 0)
			{
				tab.clear();
				strsplit(tab, line, ";");
				if (tab.size() >= 4)
				{
					Account* new_account = new Account(tab[0].copy(), tab[1].copy());
					new_account->icon = jgl::Vector2(jgl::stoi(tab[2]), jgl::stoi(tab[3]));
					_account_map[new_account->username] = new_account;
					LOG_MESSAGE("Adding account [" + new_account->username + "]/[" + new_account->password + "] to loaded account");
					for (size_t i = 4; i < tab.size(); i++)
					{
						new_account->friend_list.push_back(tab[i].copy());
					}
				}
			}
		}
	}
}
