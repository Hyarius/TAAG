#include "taag_server.h"

Server* serv;

void save_server(int sig)
{
	serv->save();
	serv->quit();
	exit(0);
}

int main(int argc, char** argv)
{
	serv = new Server();
	signal(SIGINT, save_server);
	signal(SIGQUIT, save_server);
	signal(SIGTSTP, save_server);
	serv->run();
	serv->save();
	return (0);
}
