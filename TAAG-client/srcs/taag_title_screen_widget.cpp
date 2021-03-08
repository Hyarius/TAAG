#include "taag_client.h"

void connect_account(jgl::Data param)
{
	Title_screen* title_screen;

	title_screen = param.acces<Title_screen*>(0);
	title_screen->connect();
}

void signup_account(jgl::Data param)
{
	Title_screen* title_screen;

	title_screen = param.acces<Title_screen*>(0);
	title_screen->signup();
}

Title_screen::Title_screen(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_title_image = new jgl::Sprite_sheet("ressources/texture/titlescreen.png", jgl::Vector2(8, 1));

	// USERNAME WIDGETS
	// ----
	_username_label = new jgl::Text_label(" Username : ", this);
	_username_label->set_horizontal_alignment(jgl::Horizontal_alignment::left);
	_username_label->set_vertical_alignment(jgl::Vertical_alignment::top);
	_username_label->set_back(jgl::Color(0, 0, 0, 0));
	_username_label->set_front(jgl::Color(0, 0, 0, 0));
	_username_label->activate();

	_username_entry = new jgl::Text_entry("Hyarius", this);
	_username_entry->set_horizontal_alignment(jgl::Horizontal_alignment::centred);
	_username_entry->set_vertical_alignment(jgl::Vertical_alignment::centred);
	_username_entry->set_back(jgl::Color(0, 0, 0, 0));
	_username_entry->set_front(jgl::Color(0, 0, 0, 0));
	_username_entry->activate();

	_username_error_label = new jgl::Text_label("", this);
	_username_error_label->set_color(jgl::text_color::dark_red);
	_username_error_label->set_horizontal_alignment(jgl::Horizontal_alignment::centred);
	_username_error_label->set_vertical_alignment(jgl::Vertical_alignment::bottom);
	_username_error_label->set_back(jgl::Color(0, 0, 0, 0));
	_username_error_label->set_front(jgl::Color(0, 0, 0, 0));
	_username_error_label->activate();
	// ----


	// PASSWORD WIDGETS
	// ----
	_password_label = new jgl::Text_label(" Password : ", this);
	_password_label->set_horizontal_alignment(jgl::Horizontal_alignment::left);
	_password_label->set_vertical_alignment(jgl::Vertical_alignment::top);
	_password_label->set_back(jgl::Color(0, 0, 0, 0));
	_password_label->set_front(jgl::Color(0, 0, 0, 0));
	_password_label->activate();

	_password_entry = new jgl::Text_entry("*oUhzk2jg", this);
	_password_entry->set_horizontal_alignment(jgl::Horizontal_alignment::centred);
	_password_entry->set_vertical_alignment(jgl::Vertical_alignment::centred);
	_password_entry->set_back(jgl::Color(0, 0, 0, 0));
	_password_entry->set_front(jgl::Color(0, 0, 0, 0));
	_password_entry->set_masked(true);
	_password_entry->activate();

	_password_error_label = new jgl::Text_label("", this);
	_password_error_label->set_color(jgl::text_color::dark_red);
	_password_error_label->set_horizontal_alignment(jgl::Horizontal_alignment::centred);
	_password_error_label->set_vertical_alignment(jgl::Vertical_alignment::bottom);
	_password_error_label->set_back(jgl::Color(0, 0, 0, 0));
	_password_error_label->set_front(jgl::Color(0, 0, 0, 0));
	_password_error_label->activate();
	// ----

	_connect_button = new jgl::Button(connect_account, this, this);
	_connect_button->set_tileset("ressources/texture/button_1.png");
	_connect_button->set_text("Connect");
	_connect_button->activate();

	_signup_button = new jgl::Button(signup_account, this, this);
	_signup_button->set_tileset("ressources/texture/button_1.png");
	_signup_button->set_text("Sign up");
	_signup_button->activate();

	_client_error_label = new jgl::Text_label("", this);
	_client_error_label->set_color(jgl::text_color::dark_red);
	_client_error_label->set_horizontal_alignment(jgl::Horizontal_alignment::centred);
	_client_error_label->set_vertical_alignment(jgl::Vertical_alignment::centred);
	_client_error_label->set_back(jgl::Color(0, 0, 0, 0));
	_client_error_label->set_front(jgl::Color(0, 0, 0, 0));
	_client_error_label->activate();

}

void Title_screen::set_client_error_message(jgl::String text)
{
	_client_error_label->set_text(text);
	if (_client_error_label->size() > _client_error_text_size)
		_client_error_label->set_size(_client_error_text_size);
}

void Title_screen::connect()
{
	set_client_error_message("");
	jgl::String username = _username_entry->text();
	jgl::String password = _password_entry->text();

	// - On creer une string pour stocker le message d'erreur du username si besoin
	jgl::String username_error_str = "";
	bool username_validity = check_username_validity(username, username_error_str);

	// - On creer une string pour stocker le message d'erreur du password si besoin
	jgl::String password_error_str = "";
	bool password_validity = check_password_validity(password, password_error_str);

	// - Apres les deux checks password/username du dessus, on a dans password_validity et dans username_validity l'etat de notre compte : est-ce qu'il est acceptable ?

	_username_error_label->set_text(username_error_str);
	if (_username_error_label->size() > _error_text_size)
		_username_error_label->set_size(_error_text_size);

	_password_error_label->set_text(password_error_str);
	if (_password_error_label->size() > _error_text_size)
		_password_error_label->set_size(_error_text_size);


	if (password_validity == true && username_validity == true)
	{
		jgl::Message<Server_message> msg(Server_message::Client_account_data);

		msg.add_string(password);
		msg.add_string(username);

		g_client->send(msg);
	}
}

void Title_screen::signup()
{
	set_client_error_message("");

	jgl::String username = _username_entry->text();
	jgl::String password = _password_entry->text();

	// - On creer une string pour stocker le message d'erreur du username si besoin
	jgl::String username_error_str = "";
	bool username_validity = check_username_validity(username, username_error_str);

	// - On creer une string pour stocker le message d'erreur du password si besoin
	jgl::String password_error_str = "";
	bool password_validity = check_password_validity(password, password_error_str);

	// - Apres les deux checks password/username du dessus, on a dans password_validity et dans username_validity l'etat de notre compte : est-ce qu'il est acceptable ?
	_username_error_label->set_text(username_error_str);
	if (_username_error_label->size() > _error_text_size)
		_username_error_label->set_size(_error_text_size);

	_password_error_label->set_text(password_error_str);
	if (_password_error_label->size() > _error_text_size)
		_password_error_label->set_size(_error_text_size);

	if (password_validity == true && username_validity == true)
	{
		jgl::Message<Server_message> msg(Server_message::Client_ask_sign_up);

		msg.add_string(password);
		msg.add_string(username);

		g_client->send(msg);
	}
}

void Title_screen::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	int text_size = p_area.y / 20;

	_label_text_size = text_size * 0.75f;
	_entry_text_size = text_size;
	_error_text_size = text_size * 0.75f;
	_client_error_text_size = text_size * 0.75f;

	jgl::Vector2 entry_size = p_area * jgl::Vector2(57.0f / 149.0f, 15.0f / 97.0f);
	jgl::Vector2 button_size = p_area * jgl::Vector2(20.0f / 149.0f, 6.0f / 97.0f);

	jgl::Vector2 username_pos = p_area * jgl::Vector2(7.0f / 149.0f, 29.0f / 97.0f);

	jgl::Vector2 password_pos = p_area * jgl::Vector2(7.0f / 149.0f, 46.0f / 97.0f);

	jgl::Vector2 connect_pos = p_area * jgl::Vector2(49.0f / 149.0f, 71.0f / 97.0f);
	jgl::Vector2 signup_pos = p_area * jgl::Vector2(25.0f / 149.0f, 71.0f / 97.0f);
	jgl::Vector2 client_error_pos = p_area * jgl::Vector2(5.0f / 149.0f, 64.0f / 97.0f);
	jgl::Vector2 client_error_size = p_area * jgl::Vector2(61.0f / 149.0f, 4.0f / 97.0f);

	_username_label->set_geometry(username_pos, entry_size);
	_username_label->set_size(_label_text_size);
	_username_entry->set_geometry(username_pos, entry_size);
	_username_entry->set_size(_entry_text_size);
	_username_error_label->set_geometry(username_pos, entry_size);
	_username_error_label->set_size(_error_text_size);

	_password_label->set_geometry(password_pos, entry_size);
	_password_label->set_size(_label_text_size);
	_password_entry->set_geometry(password_pos, entry_size);
	_password_entry->set_size(_entry_text_size);
	_password_error_label->set_geometry(password_pos, entry_size);
	_password_error_label->set_size(_error_text_size);

	_connect_button->set_geometry(connect_pos, button_size);

	_signup_button->set_geometry(signup_pos, button_size);

	_client_error_label->set_geometry(client_error_pos, client_error_size);
	_client_error_label->set_size(_client_error_text_size);
}

void Title_screen::render()
{
	int time_per_image = 350;
	int index = (g_application->time() % (time_per_image * 8)) / time_per_image;
	_title_image->draw(index, jgl::Vector2(0, 0), jgl::Vector2(_viewport->area()), 1.0f, _viewport);
}