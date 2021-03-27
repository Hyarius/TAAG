#include "taag_server.h"

Server* serv;

void save_server(int sig)
{
	serv->save();
	serv->quit();
	//delete serv;
	exit(0);
}

int main(int argc, char** argv)
{
	jgl::start_jgl();
	serv = new Server();
	signal(SIGINT, save_server);
	
	#if !defined(WIN32) && !defined(_WIN32) && !defined(__WIN32)
		signal(SIGQUIT, save_server);
		signal(SIGTSTP, save_server);
	#else
		signal(SIGBREAK, save_server);
	#endif
	
	serv->run();
	//serv->save();
	return (0);
}
