#include "taag_common.h"

int64_t compute_magic_number(int64_t value)
{
	return (((value ^ (value / 3)) * MAJOR_VERSION_KEY_NUMBER) % MINOR_VERSION_KEY_NUMBER) ^ ABSTRACT_VERSION_KEY_NUMBER;
}

bool check_username_validity(jgl::String username, jgl::String& error)
{
	if (username.empty() == true)
	{
		error = "Please, enter a username";
		return (false);
	}
	if (username.size() <= 5)
	{
		error = "Username too short";
		return (false);
	}
	if (username.compose_only("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") == false)
	{
		error = "Username must contain only letter and number";
		return (false);
	}
	error = "";
	return (true);
}

bool check_password_validity(jgl::String password, jgl::String& error)
{
	if (password.empty() == true)
	{
		error = "Please, enter a password";
		return (false);
	}
	if (password.size() <= 5)
	{
		error = "Password too short";
		return (false);
	}
	bool alpha = password.find("abcdefghijklmnopqrstuvwxyz");
	bool num = password.find("0123456789");
	bool maj = password.find("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	bool special = password.find("*/:.;,?%$^()[]{}+-=#");

	if (alpha == false || num == false || maj == false || special == false)
	{
		error = "Password must contain 1 upper, 1 lower, 1 number and 1 special";
		return (false);
	}
	error = "";
	return (true);
}

void add_account_to_message(jgl::Message<Server_message>& msg, Account* account)
{
	msg << account->icon;
	msg.add_string(account->password);
	msg.add_string(account->pseudo);
	msg.add_string(account->username);
}

Account* get_account_from_message(jgl::Message<Server_message>& msg)
{
	jgl::String username;
	jgl::String pseudo;
	jgl::String password;
	jgl::Vector2 icon;

	username = msg.get_string();
	pseudo = msg.get_string();
	password = msg.get_string();
	msg >> icon;

	Account* account = new Account(username, pseudo, password);
	account->icon = icon;

	return (account);
}

void add_friend_to_message(jgl::Message<Server_message>& msg, Friend* target)
{
	msg << target->state;
	msg << target->icon;
	msg.add_string(target->pseudo);
}

void add_friend_to_message(jgl::Message<Server_message>& msg, Account* target)
{
	msg << target->state;
	msg << target->icon;
	msg.add_string(target->pseudo);
}

Friend* get_friend_from_message(jgl::Message<Server_message>& msg)
{
	jgl::String name;
	jgl::Vector2 sprite;
	Account_state state;

	name = msg.get_string();
	msg >> sprite;
	msg >> state;

	Friend* target = new Friend(name, sprite, state);

	return (target);
}
