#include "taag_server.h"

Server* serv;

void save_server(int sig)
{
	serv->save();
	serv->quit();
}

int main(int argc, char** argv)
{
	serv = new Server();
	signal(SIGINT, save_server);
	signal(SIGBREAK, save_server);
	serv->run();
	serv->save();
	return (0);
}