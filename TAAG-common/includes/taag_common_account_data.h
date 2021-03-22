#pragma once

#include "jgl.h"

struct Account
{
	static jgl::Sprite_sheet* icon_sprite;
	jgl::String username;
	jgl::String password;
	jgl::Vector2 icon;

	struct Game_room* room;

	jgl::Connexion<Server_message>* client;

	jgl::Array<jgl::String> friend_list;

	Account(jgl::String p_username, jgl::String p_password)
	{
		room = nullptr;
		username = p_username;
		password = p_password;
		icon = 0;
		client = nullptr;
		friend_list.clear();
	}

	static void set_icon_sprite(jgl::Sprite_sheet* new_sprite) { icon_sprite = new_sprite; }

	void save(std::fstream& file)
	{
		file << username << ";" << password << ";" << icon.x << ";" << icon.y;
		for (size_t i = 0; i < friend_list.size(); i++)
		{
			file << ";" << friend_list[i];
		}
	}
};

struct Game_room
{
	Account* players[NB_PLAYER_PER_GAME];
	uint32_t size;

	Game_room()
	{
		size = 0;
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			players[i] = nullptr;
		}
	}

	void add_player(Account* target)
	{
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			if (players[i] == nullptr)
			{
				players[i] = target;
				target->room = this;
				size++;
				return ;
			}
		}
	}

	void remove_player(Account* target)
	{
		for (size_t i = 0; i < NB_PLAYER_PER_GAME; i++)
		{
			if (players[i] != nullptr && players[i]->username == target->username)
			{
				players[i] = nullptr;
				target->room = nullptr;
				size--;
				return;
			}
		}
	}
};