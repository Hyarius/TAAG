#pragma once
#include "taag_common.h"

class Title_screen : public jgl::Widget
{
private:
	jgl::Sprite_sheet* _title_image;

	jgl::Text_label* _username_label;
	jgl::Text_entry* _username_entry;
	jgl::Text_label* _username_error_label;

	jgl::Text_label* _password_label;
	jgl::Text_entry* _password_entry;
	jgl::Text_label* _password_error_label;

	jgl::Button* _connect_button;
	jgl::Button* _signup_button;

	jgl::Text_label* _client_error_label;

	int _label_text_size;
	int _entry_text_size;
	int _error_text_size;
	int _client_error_text_size;

public:
	Title_screen(jgl::Widget* p_parent = nullptr);
	void set_client_error_message(jgl::String text);
	void connect();
	void signup();
	void set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area);
	void render();
};

extern Title_screen* g_title_screen;