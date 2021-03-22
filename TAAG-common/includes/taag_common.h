#pragma once

#include "jgl.h"
#include < stdint.h >

#include "taag_common_messaging.h"

#define MAJOR_VERSION_KEY_NUMBER 154612
#define MINOR_VERSION_KEY_NUMBER 61564825
#define ABSTRACT_VERSION_KEY_NUMBER 948

#define NB_PLAYER_PER_GAME 10 

#define LOG_MESSAGE(text) std::cout << "[" << jgl::normalize_string(__FUNCTION__, ' ', 45) << "][" << jgl::normalize_string(jgl::itoa(__LINE__), '0', 5) << "] - " << text << std::endl;

int64_t compute_magic_number(int64_t value);
bool check_username_validity(jgl::String username, jgl::String& error);
bool check_password_validity(jgl::String password, jgl::String& error);

#include "taag_common_account_data.h"

void add_account_to_message(jgl::Message<Server_message>& msg, Account* p_account);
Account* get_account_from_message(jgl::Message<Server_message>& msg);