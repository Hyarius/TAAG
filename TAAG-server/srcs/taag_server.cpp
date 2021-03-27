#include "taag_server.h"

Server::Server() : jgl::Server_interface<Server_message>(37500)
{
	_quit = false;
	load();
	start();
	for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
	{
		_room_array[i].clear();
	}
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
	for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
	{
		for (size_t j = 0; j < _room_array[i].size(); j++)
		{
			if (_room_array[i][j] != nullptr)
				delete _room_array[i][j];
		}
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
		disconnect_account(client, _client_to_account_map[client->id()]);
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

void Server::connect_account(jgl::Connexion<Server_message>* client, Account* account)
{
	_client_to_account_map[client->id()] = account;
	account->client = client;
	account->state = Account_state::In_lobby;

	jgl::Message<Server_message> msg(Server_message::Change_friend_state);

	actualize_status(account);
}

void Server::disconnect_account(jgl::Connexion<Server_message>* client, Account* account)
{
	jgl::Message<Server_message> msg(Server_message::Change_friend_state);

	Game_room* room = account->room;
	if (room != nullptr)
	{
		_room_array[room->size].erase(_room_array[room->size].find(room));
		if (room != nullptr)
			room->remove_player(account);
		send_room_information(room);
		if (room->size == 0)
		{
			delete room;
		}
		else
		{
			_room_array[room->size].push_back(room);
		}
	}
	

	account->state = Account_state::Disconnected;
	actualize_status(account);

	account->client = nullptr;
}

void Server::send_chat_line(jgl::String text)
{
	jgl::Message<Server_message> to_send(Server_message::Chat_message);

	to_send.add_string(text);

	LOG_MESSAGE("Message send : " + text);

	message_all(to_send, nullptr);
}

void Server::send_chat_line_to_player(jgl::Connexion<Server_message>* client, jgl::String text)
{
	jgl::Message<Server_message> to_send(Server_message::Chat_message);

	to_send.add_string(text);

	LOG_MESSAGE("Message send : " + text + " to client " + _client_to_account_map[client->id()]->pseudo);

	client->send(to_send);
}

void Server::actualize_status(Account *account)
{
	jgl::Message<Server_message> to_send(Server_message::Friend_list_content);

	add_friend_to_message(to_send, account);

	for (size_t i = 0; i < account->link_list.size(); i++)
	{
		Account* target = _account_map[account->link_list[i]];
		if (target != nullptr && target->client != nullptr)
			target->client->send(to_send);
	}
}


void Server::send_friend_list(jgl::Connexion<Server_message>* client)
{
	Account* account = _client_to_account_map[client->id()];

	jgl::Message<Server_message> to_send(Server_message::Friend_list_content);

	for (size_t i = 0; i < account->friend_list.size(); i++)
	{
		Account* target = _account_map[account->friend_list[i]];

		if (target != nullptr)
			add_friend_to_message(to_send, target);
	}

	client->send(to_send);
}

void Server::send_room_information(Game_room* room)
{
	room->arrange();
	jgl::Message<Server_message> result(Server_message::Game_room_information);

	for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
	{
		if (room->players[i] != nullptr)
		{
			result.add_string(room->players[i]->pseudo);
			result << room->players[i]->icon;
		}
	}

	if (room->size > 0)
	{
		result.add_string(room->leader->pseudo);
		result << room->in_search;

		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			if (room->players[i] != nullptr)
			{
				_account_map[room->players[i]->pseudo]->client->send(result);
			}
		}
	}
	
}

void Server::on_message_reception(jgl::Connexion<Server_message>* client, jgl::Message<Server_message>& msg)
{
	Account* account = _client_to_account_map[client->id()];

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
				if (tmp_account != nullptr && tmp_account->password == password && tmp_account->client == nullptr)
				{
					LOG_MESSAGE("Can login " + username);
					connect_account(client, tmp_account);
					result.header.id = Server_message::Server_accept_login;
					add_account_to_message(result, tmp_account);
				}
				else if (tmp_account != nullptr && tmp_account->password != password)
				{
					LOG_MESSAGE("Wrong password to connect to account " + username);
					result.header.id = Server_message::Server_refuse_login;
					result.add_string("Wrong password to connect to account " + username);
				}
				else if (tmp_account != nullptr && tmp_account->client != nullptr)
				{
					LOG_MESSAGE("Username " + username + " is already connected");
					result.header.id = Server_message::Server_refuse_login;
					result.add_string("Username " + username + " is already connected");
				}
				else
				{
					LOG_MESSAGE("Unkonw error with username : " + username);
					result.header.id = Server_message::Server_refuse_login;
					result.add_string("Unkonw error with username : " + username);
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
			jgl::String pseudo;

			username = msg.get_string();
			password = msg.get_string();
			pseudo = username;

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
				Account* new_account = new Account(username, pseudo, password);
				_account_map[username] = new_account;
				_pseudo_map[pseudo] = new_account;
				LOG_MESSAGE("Username [" + username + "] create a new account !");
				result.header.id = Server_message::Server_accept_sign_up;
				result.add_string("Account successfully created");
			}
			client->send(result);
		}
		break;
		case Server_message::Send_chat_message:
		{
			if (account != nullptr)
			{
				jgl::String username = account->username;

				jgl::String tmp = msg.get_string();

				jgl::String result = "[" + username + "] : " + tmp;

				send_chat_line(result);
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
			jgl::String name = msg.get_string();

			Account* other = _account_map[name];

			if (other != nullptr && account->friend_list.find(name) == account->friend_list.end())
			{
				account->friend_list.push_back(name);
				other->link_list.push_back(account->username);

				send_friend_list(client);
				send_chat_line_to_player(client, "[Server] : Friend add succesfully");
			}
		}
		break;
		case Server_message::Remove_friend_from_list:
		{
			jgl::String name = msg.get_string();

			Account* other = _account_map[name];

			if (other != nullptr && account->friend_list.find(name) != account->friend_list.end())
			{
				account->friend_list.erase(account->friend_list.find(name));
				other->link_list.erase(account->friend_list.find(name));

				send_friend_list(client);
				send_chat_line_to_player(client, "[Server] : Removing friend succesfully");
			}
		}
		break;
		case Server_message::Create_new_room:
		{			
			LOG_MESSAGE("Creating room case");

			Game_room *new_room = new Game_room();

			if (account->room != nullptr)
				account->room->remove_player(account);

			account->room = new_room;
			new_room->add_player(account);

			_room_array[new_room->size].push_back(new_room);

			send_room_information(new_room);
			actualize_status(account);
			send_chat_line_to_player(client, "[Server] : Room created succesfully");
		}
		break;
		case Server_message::Join_room:
		{
			LOG_MESSAGE("Joining room case");

			jgl::String other = msg.get_string();

			Account* master = _account_map[other];

			if (account->room != nullptr)
				account->room->remove_player(account);

			account->room = master->room;

			if (master != nullptr && master->room != nullptr && master->state == Account_state::In_room)
			{
				_room_array[master->room->size].erase(_room_array[master->room->size].find(master->room));
				if (master->room->add_player(account) == true)
				{
					send_chat_line_to_player(client, "[Server] : Room joined succesfully");
				}
				else
				{
					send_chat_line_to_player(client, "[Server] : Can't join this game room");
				}
				_room_array[master->room->size].push_back(master->room);
				send_room_information(master->room);
				actualize_status(account);
			}
		}
		break;
		case Server_message::Leave_room:
		{
			LOG_MESSAGE("Leaving room case");

			if (account->room != nullptr && account->state == Account_state::In_room)
			{
				Game_room* room = account->room;

				_room_array[room->size].erase(_room_array[room->size].find(room));
				room->remove_player(account);
				_room_array[room->size].push_back(room);

				send_room_information(room);
				actualize_status(account);
				send_chat_line_to_player(client, "[Server] : Room leaved succesfully");
			}
		}
		break;
		case Server_message::Launch_game_request:
		{
			LOG_MESSAGE(account->pseudo + " asked for starting matchmaking");
			LOG_MESSAGE("Room size : " + jgl::itoa(account->room->size) + " -> looking for " + jgl::itoa(NB_PLAYER_PER_GAME - account->room->size) + " other players");

			account->room->arrange();
			for (size_t i = 0; i < account->room->size; i++)
			{
				Account* tmp = _pseudo_map[account->room->players[i]->pseudo];
				send_chat_line_to_player(tmp->client, "[Server] : Matchmaking started");
			}
			account->room->in_search = true;
			send_room_information(account->room);
		}
		break;
		case Server_message::Stop_matchmaking:
		{
			LOG_MESSAGE(account->pseudo + " asked for stop matchmaking");

			account->room->arrange();
			for (size_t i = 0; i < account->room->size; i++)
			{
				Account* tmp = _pseudo_map[account->room->players[i]->pseudo];
				send_chat_line_to_player(tmp->client, "[Server] : Matchmaking stoped");
			}
			account->room->in_search = false;
			send_room_information(account->room);
		}
		break;
		case Server_message::Server_joker_message:
		{
			std::cout << "Game room waiting" << std::endl;
			for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
			{
				std::cout << "Rooms size : [" << i << "] players" << std::endl;
				jgl::Array<Game_room*>& tmp_array = _room_array[i];
				if (tmp_array.size() != 0)
				{
					for (size_t j = 0; j < tmp_array.size(); i++)
					{
						std::cout << "Game room [" << i << "]";
						for (size_t k = 0; k < tmp_array[j]->size; k++)
						{
							if (k != 0)
								std::cout << " - ";
							std::cout << "[" << tmp_array[j]->players[k] << "]";
						}
						std::cout << std::endl;
					}
				}
				else
				{
					std::cout << "No game room with such number of player" << std::endl;
				}
			}
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
	uint32_t last_second = 0;
	while (_quit == false)
	{
		uint32_t ticks = SDL_GetTicks() / (1000 * 60 * 60);
		if (ticks != last_second)
		{
			save();
			last_second = ticks;
		}
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
				Account* tmp = new Account(line);
				LOG_MESSAGE("New account : " + tmp->username + " - " + tmp->pseudo + " - " + tmp->password);
				_account_map[tmp->username] = tmp;
				_pseudo_map[tmp->pseudo] = tmp;
			}
		}
	}
}
