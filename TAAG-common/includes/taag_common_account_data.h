#pragma once

#include "jgl.h"

struct Account
{
	static jgl::Sprite_sheet* icon_sprite;
	jgl::String username;
	jgl::String password;
	jgl::Vector2 icon;

	jgl::Connexion<Server_message>* client;

	jgl::Array<jgl::String> friend_list;

	Account(jgl::String p_username, jgl::String p_password)
	{
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