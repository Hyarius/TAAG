#include "taag_client.h"

Client* g_client;
Title_screen* g_title_screen;
Game_menu* g_game_menu;

int main(int argc, char** argv)
{
	jgl::Application app = jgl::Application("TAAG", jgl::Vector2(0.8, 0.8));
	jgl::set_font_path("ressources/font/karma suture.ttf");

	g_client = new Client();
	g_client->activate();

	g_title_screen = new Title_screen();
	g_title_screen->set_geometry(0, app.size());
	g_title_screen->activate();



	return (app.run());
}