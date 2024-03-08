#ifndef SERVER_HPP
# define SERVER_HPP

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/poll.h>
#include "utils.h"
#include <map>
#include <vector>
#include "Client.hpp"
#include <fcntl.h>
#include <sys/time.h>
#include <time.h>

#define SERVER_NAME "Minou.IRC"


#define USERLEN 31
#define NICKLEN 10

class Client;

using std::string;

/*
the server MUST keep track of the channel members, as
	well as the channel modes.

   When a client connection unexpectedly closes, a QUIT message is
   generated on behalf of the client by the server to which the client
   was connected.  No other message is to be generated or used.
*/

class Server
{
	private:
	const string _serverName;
	int _port;
	int _sockfd;
	string _password;
	std::map<int, Client*> _clients;
	std::vector<struct pollfd> _pollfd;
	std::vector<Channel> _channels;
	std::map<string, int (Server::*)(Client*, std::vector<string>)> _commandsMap;
	// sockaddr_in serverAddr;

	bool digitsCheck(const std::string &arg) const;
	bool nameCheck(const std::string &arg) const;
	void SetupServer();
	double getTime();

	int nick(Client*client, std::vector<string>);
	int user(Client*client, std::vector<string>);
	int pass(Client*client, std::vector<string>);
	int quit(Client*client, std::vector<string>);
	int join(Client*client, std::vector<string>);
	int topic(Client*client, std::vector<string>);
	int names(Client*client, std::vector<string>);
	int invite(Client*client, std::vector<string>);
	int kick(Client*client, std::vector<string>);
	int mode(Client*client, std::vector<string>);

	void init(void);

	public:
	Server(const string& port_str,  const string& password);
	~Server();

	int Run();
};

#endif