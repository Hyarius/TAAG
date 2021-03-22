#include "taag_client.h"

void validate(jgl::Data param)
{
	Game_engine* engine = param.acces<Game_engine*>(0);
	jgl::Text_entry* entry = param.acces< jgl::Text_entry*>(1);

	LOG_MESSAGE("Validate adding friend menu");

	engine->send_new_friend(entry->text());

	entry->set_text("");

	engine->close_add_friend_popup();
}

void cancel(jgl::Data param)
{
	Game_engine* engine = param.acces<Game_engine*>(0);
	jgl::Text_entry* entry = param.acces< jgl::Text_entry*>(1);

	LOG_MESSAGE("Cancel adding friend menu");

	entry->set_text("");

	engine->close_add_friend_popup();
}

Adding_friend_menu::Adding_friend_menu(Game_engine* p_engine, jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_engine = p_engine;

	_adding_friend_menu_frame = new jgl::Frame(this);
	_adding_friend_menu_frame->activate();

	_adding_friend_menu_label = new jgl::Text_label("Name of the player :", _adding_friend_menu_frame);
	_adding_friend_menu_label->activate();

	_adding_friend_menu_entry = new jgl::Text_entry("", _adding_friend_menu_frame);
	_adding_friend_menu_entry->activate();

	jgl::Data param = jgl::Data(2, _engine, _adding_friend_menu_entry);

	_adding_friend_menu_validate_button = new jgl::Button(validate, param, _adding_friend_menu_frame);
	_adding_friend_menu_validate_button->set_text("Confirm");
	_adding_friend_menu_validate_button->activate();

	_adding_friend_menu_cancel_button = new jgl::Button(cancel, param, _adding_friend_menu_frame);
	_adding_friend_menu_cancel_button->set_text("Cancel");
	_adding_friend_menu_cancel_button->activate();

}

void Adding_friend_menu::set_geometry_imp(jgl::Vector2 p_anchor, jgl::Vector2 p_area)
{
	jgl::Vector2 tmp = p_area / jgl::Vector2(100, 50);

	_adding_friend_menu_frame->set_geometry(0, p_area);

	_adding_friend_menu_label->set_geometry(jgl::Vector2(2, 2) * tmp, jgl::Vector2(60, 10) * tmp);
	_adding_friend_menu_entry->set_geometry(jgl::Vector2(2, 17) * tmp, jgl::Vector2(96, 10) * tmp);
	_adding_friend_menu_validate_button->set_geometry(jgl::Vector2(72, 37) * tmp, jgl::Vector2(25, 10) * tmp);
	_adding_friend_menu_cancel_button->set_geometry(jgl::Vector2(3, 37) * tmp, jgl::Vector2(25, 10) * tmp);
}

void Adding_friend_menu::render()
{

}