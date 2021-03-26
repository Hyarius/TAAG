#pragma once

#include "jgl.h"

enum class Account_state
{
	Disconnected = 0,
	In_lobby = 1,
	In_room = 2,
	In_game = 3
};

struct Account_info
{
	jgl::String pseudo;
	jgl::Vector2 icon;
	Account_state state;

	Account_info(jgl::String p_name = "", jgl::Vector2 p_icon = 0, Account_state p_state = Account_state::Disconnected)
	{
		pseudo = p_name;
		icon = p_icon;
		state = p_state;
	}
};

struct Account : public Account_info
{
	static jgl::Sprite_sheet* icon_sprite;

	jgl::String username;
	jgl::String pseudo;
	jgl::String password;
	jgl::Vector2 icon;
	Account_state state;

	struct Game_room* room;

	jgl::Connexion<Server_message>* client;

	jgl::Array<jgl::String> friend_list;
	jgl::Array<jgl::String> link_list;

	Account() : Account_info()
	{
		username = "";
		password = "";
		pseudo = "";
		room = nullptr;
		state = Account_state::Disconnected;
		icon = 0;
		client = nullptr;
		friend_list.clear();
		link_list.clear();
	}
	Account(jgl::String line) : Account()
	{
		static jgl::Array<jgl::String> tab;
		static jgl::Array<jgl::String> tab2;

		tab.clear();
		strsplit(tab, line, ";", false);
		for (size_t i = 0; i < tab.size(); i++)
		{
			std::cout << "tab [" << i << "] = [" << tab[i] << "]" << std::endl;
		}
		if (tab.size() >= 5)
		{
			username = tab[0].copy();
			pseudo = tab[1].copy();
			password = tab[2].copy();
			this->icon = jgl::Vector2(jgl::stoi(tab[3]), jgl::stoi(tab[4]));
			LOG_MESSAGE("Adding account [" + this->username + "]/[" + this->password + "] to loaded account");

			if (tab[5].size() != 0)
			{
				tab2.clear();
				strsplit(tab2, tab[5], "/");
				for (size_t i = 0; i < tab2.size(); i++)
				{
					LOG_MESSAGE("Adding friend [" + tab2[i] + "]");
					this->friend_list.push_back(tab2[i].copy());
				}
			}

			if (tab[6].size() != 0)
			{
				tab2.clear();
				strsplit(tab2, tab[6], "/");
				for (size_t i = 0; i < tab2.size(); i++)
				{
					LOG_MESSAGE("Adding link [" + tab2[i] + "]");
					this->link_list.push_back(tab2[i].copy());
				}
			}
		}
	}
	Account(jgl::String p_username, jgl::String p_pseudo, jgl::String p_password) : Account_info()
	{
		username = p_username;
		password = p_password;
		pseudo = p_pseudo;
		room = nullptr;
		client = nullptr;
		friend_list.clear();
		link_list.clear();
	}

	static void set_icon_sprite(jgl::Sprite_sheet* new_sprite) { icon_sprite = new_sprite; }

	void save(std::fstream& file)
	{
		file << username << ";" << pseudo << ";" << password << ";" << icon.x << ";" << icon.y << ";";
		for (size_t i = 0; i < friend_list.size(); i++)
		{
			if (i != 0)
				file << "/";
			file << friend_list[i];
		}
		file << ";";
		for (size_t i = 0; i < link_list.size(); i++)
		{
			if (i != 0)
				file << "/";
			file << link_list[i];
		}
		file << ";";
	}
};


struct Friend : public Account_info
{
	int text_size;

	Friend(jgl::String p_name = "", jgl::Vector2 p_icon = 0, Account_state p_state = Account_state::Disconnected)
	{
		pseudo = p_name;
		icon = p_icon;
		state = p_state;
		text_size = -1;
	}
	Friend(Account *account)
	{
		pseudo = account->pseudo;
		icon = account->icon;
		state = account->state;
		text_size = -1;
	}
};

struct Game_room
{
	Account_info* players[NB_PLAYER_PER_GAME];
	uint32_t size;

	Game_room()
	{
		size = 0;
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			players[i] = nullptr;
		}
	}

	void clean()
	{
		size = 0;
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			if (players[i] != nullptr)
			{
				delete players[i];
				players[i] = nullptr;
			}
		}
	}

	bool add_player_info(jgl::String name, jgl::Vector2 icon)
	{
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			if (players[i] == nullptr)
			{
				players[i] = new Account_info(name, icon, Account_state::In_room);
				size++;
				return (true);
			}
		}
		return (false);
	}

	bool add_player(Account* target)
	{
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			if (players[i] == nullptr)
			{
				players[i] = new Account_info(target->pseudo, target->icon, target->state);
				target->room = this;
				target->state = Account_state::In_room;
				size++;
				return (true);
			}
		}
		return (false);
	}

	bool remove_player(Account* target)
	{
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			if (players[i]->pseudo == target->pseudo)
			{
				delete players[i];
				players[i] = nullptr;
				target->room = nullptr;
				target->state = Account_state::In_lobby;
				size--;
				return (true);
			}
		}
		return (false);
	}
};

void add_account_to_message(jgl::Message<Server_message>& msg, Account* account);
Account* get_account_from_message(jgl::Message<Server_message>& msg);

void add_friend_to_message(jgl::Message<Server_message>& msg, Friend* account);
Friend* get_friend_from_message(jgl::Message<Server_message>& msg);

void add_friend_to_message(jgl::Message<Server_message>& msg, Account* account);