#pragma once

enum class Server_message
{
	Server_error_message = -1,
	Server_ask_magic_number = 0,
	Client_awnser_magic_number = 1,
	Server_accept_connection = 2,
	Server_refuse_connection = 3,
	Client_ask_sign_in = 4,
	Client_ask_sign_up = 5,
	Server_accept_sign_up = 6,
	Server_refuse_sign_up = 7,
	Server_accept_login = 8,
	Server_refuse_login = 9,
	Send_chat_message = 10,
	Chat_message = 11,
	Friend_list_content = 12,
	Add_friend_to_list = 13,
	Remove_friend_from_list = 14,
	Change_friend_state = 15
};