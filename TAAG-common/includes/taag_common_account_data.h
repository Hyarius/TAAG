#pragma once

#include "jgl.h"

struct Account
{
	jgl::String username;
	jgl::String password;

	Account(jgl::String p_username, jgl::String p_password)
	{
		username = p_username;
		password = p_password;
	}

	void save(std::fstream& file)
	{
		file << username << ";" << password << std::endl;
	}
};