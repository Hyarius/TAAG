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