#include "taag_client.h"

int main(int argc, char** argv)
{
	float ratio = 0.9f;
	jgl::Application app = jgl::Application("TAAG", jgl::Vector2(ratio * 0.8f, ratio));
	jgl::set_font_path("ressources/font/karma suture.ttf");

	Game_engine* engine = new Game_engine();

	return (app.run());
}