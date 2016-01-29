/*
 * pavelkolodin@gmail.com
 *
 * fir HTTP server example
 *
 */

#include <signal.h>
#include <fir/net/tcp_server_epoll.h>
#include <fir/net/server_resources.h>
#include <fir/logger/logger.h>
#include "responder.h"

int main(int _argc, char **_argv)
{
	if ( _argc < 4 ) {
		std::cout	<< "Usage:   httpserver <ip> <port> <HTML templates path>\n"
					<< "example: httpserver 127.0.0.1 54321 /var/www/html\n";
		return 1;
	}

	signal(SIGPIPE, SIG_IGN);

	typedef fir::net::ServerResources<int, Responder> SR;
	SR resources(_argv[3], _argv[3]);
    fir::net::TCPServerEpoll<SR> server( resources );

    LOG_PURE ("Listening: " << _argv[1] << ":" << atoi(_argv[2]) );

    server.start( _argv[1], atoi(_argv[2]) );

    return 0;
}
