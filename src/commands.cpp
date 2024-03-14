#include "Server.hpp"

int Server::user(Client*client, std::vector<string>arg)
{
	string username;

	if(arg.size() == 0 || arg.empty())
	{
		sendMessage(client, this->_serverName, client->getUsername(), USER_USAGE);
		return 0;
	}
	if(!client->getUsername().empty())
	{
		sendMessage(client, this->_serverName, client->getUsername(), ALREADY_USER);
		return 0;
	}
	if(!nameCheck(arg[0]))
	{
		sendMessage(client, this->_serverName, client->getUsername(), NOT_ALPHA);
		return 0;
	}

	if(username.size() > USERLEN)
		username = arg[0].substr(0,USERLEN);
	else
		username = arg[0];
	
	if(arg.size() == 4 && arg[0] == arg[3] && arg[1] == "0" && arg[2] == "*")
		client->setLimeState(true);
	// else
	// 	client->setLimeState(false);
	client->setUsername(username);
	sendMessage(client, SERVER_NAME, client->getNickname(), USER_SUCCESS(client->getUsername()));
	checkIdentified(client);
	return 0;
}

int Server::nick(Client*client, std::vector<string>arg)
{

	if(arg.size() == 0)
	{
		sendMessage(client, _serverName, client->getUsername(), NICK_USAGE);
		return 0;
	}
	
	if(!nameCheck(arg[0]) || arg[0].size() > NICKLEN)
	{
		sendMessage(client, _serverName, client->getUsername(), NOT_ALPHA);
		return 0;
	}

	std::map<int,Client*>::iterator it;
	for(it = _clients.begin(); it != _clients.end(); it++)
	{
		if(it->second->getNickname() == arg[0])
		{
			sendMessage(client, _serverName, client->getNickname(), ALREADY_NICK);
			return 0;
		}
	}
	client->setNickname(arg[0]);
	checkIdentified(client);
	if(!client->getNickname().empty()){
		sendMessage(client, _serverName, client->getUsername(), NICK_SUCCESS(client->getNickname()));
	}
	else
	{
		sendMessage(client, _serverName, client->getUsername(), NICK_CHANGE(client->getNickname()));	
		print(SERVER_SEND + client->getUsername() + " has changed his nickname to " + client->getNickname() + "\n");
	}

	return 0;
}

int Server::pass(Client*client, std::vector<string>arg)
{

	// std::cout << _serverName << std::endl;

	if(arg.size() == 0){
		sendMessage(client, _serverName, client->getUsername(), PASS_USAGE);
		return 0;
	}


	if(arg[0] == _password){
		if(client->getState() == AUTHENTICATION){
			client->setState(IDENTIFICATION);
			identificationMessage(client);
		}
		else
			sendMessage(client, _serverName, client->getUsername(), ALREADY_IN);
		return 1;
	}
	else{
		sendMessage(client, _serverName, client->getUsername(), ERROR_PASSWORD);
		return 0; //and throw error and then close connection for that client
	}
		//throw CustomException::WrongPassword();
}

int Server::quit(Client*client, std::vector<string>arg)
{
	if(!arg[0].empty()){
		print(QUIT_MESS(client->getUsername(), arg[0]));
		sendMessage(client, _serverName, client->getNickname(), QUIT_MESS(client->getUsername(), arg[0]));
	}
	else{

		print(QUIT_MESS(client->getUsername(), DEFAULT_REASON));
		sendMessage(client, _serverName, client->getNickname(), QUIT_MESS(client->getUsername(), DEFAULT_REASON));
	}
	disconnectUser(client, _pollfd);
	return 0;
}

/*
	Steps of the JOIN command: 
	1. Take the first channel passed as argument
	2. Look for the channel in the list of channels
	3. If the channel exists, look if there is a key associated to the channel, check if the key is correct.
	4. If the channel does not exist, create the channel with its associated key (if there is one)
	5. Add the client to the channel.
	6. If there is a channel topic, send the topic to the client. (Same as TOPIC command I think)
	7. Send the list of users in the channel to the client. (Same as NAMES command, maybe)

	Channels names are strings (beginning with a '&', '#', '+' or '!'
	character) of length up to fifty (50) characters.  Channel names are
	case insensitive.
*/
int Server::join(Client* client, std::vector<string> arg) // standard command to create / join channels
{

	if (arg.size() < 1)
		return (1); //TODO: send appropriate error, this checl may not be necessary

	if (arg.size() == 1 && arg[0].compare("0") == 0)
	{
		//TODO: user leaves all channels it's connected to
	}
	
	if (arg.size() > 1)
	{
		return (this->joinWithKeys(client, arg));
	}

	char delimiter = ',';
	string name;
	std::vector<string> channels;
	buildStrings(arg[0], delimiter, channels);

	for (int i = 0; i < channels.size(); i++)
	{
		Channel* toJoin = isChannelValid(channels[i]);
		if (toJoin)
		{
			if (toJoin->isInviteOnly() == 0)
				return (1); //TODO: make sure the appropriate error is sent too
			toJoin->addUser(client);
		}
		else
		{
			if ((channels[i])[0] != '#') // ADD '#' to the start if not present
			{
				std::string::iterator it = channels[i].begin();
				channels[i].insert(it, '#');
			}
			this->createChannel(client, channels[i], NULL);
		}
	}

	return (0);
}

int Server::topic(Client*client, std::vector<string>arg)
{
	(void)client;
	(void)arg;

	std::cout << "topic" << std::endl;
	return 0;
}

int Server::names(Client*client, std::vector<string>arg)
{
	(void)client;
	(void)arg;

	std::cout << "names" << std::endl;
	return 0;
}

/*
	Steps of the INVITE command: 
	1. Look for nickname to see if user exists.
	2. Looks for the channel to see if it exists.
	3. Check if the inviting user is in the channel.
	4. Check if the inviting user can invite the new user to the channel.
	5. Send an invitation to the user to join the channel.
	6. Send a message to the user that invited the other user to the channel.
	7. Send a message to the channel that the user has been invited to the channel.
*/
//Errors: ERR_NOSUCHCHANNEL, ERR_NOTONCHANNEL, ERR_CHANOPRIVSNEEDED
int Server::invite(Client*client, std::vector<string>arg)
{
	if (arg.size() < 2)
		return (1);

	string channel = arg[1], user = arg[0];

	std::cout << "invite" << std::endl;
	return 0;
}

int Server::kick(Client*client, std::vector<string>arg)
{
	(void)client;
	(void)arg;

	std::cout << "kick" << std::endl;
	return 0;
}

bool Server::validOptions(const string mode) const{
    const char options[] = MODE_OPTIONS;
    //const string mode = _args[0];
    size_t i = 0;

    while(i < mode.size() && mode[i] == ' ')
        i++;
    if (mode[i] != '+' && mode[i] != '-')
        return false;
    i++;
    while(mode[i] && mode[i] != ' '){
        for(int j = 0; j < NB_OPTIONS; j++){
            if(mode[i] == options[j])
                break;
            if(j == NB_OPTIONS - 1)
                return false;
        }
        i++;
    }
    while(++i < mode.size() && mode[i] == ' ')
        ;
    if(mode[i])
        return false;
    return true;
}

int Server::mode(Client*client, std::vector<string>arg)
{
	(void)client;
	(void)arg;

	if (arg[0].empty() || arg[1].empty())
	{
		sendMessage(client, _serverName, client->getNickname(), MODE_USAGE);
		return false;
	}
	//TODO: check if channel exists (arg[0])
	if (!validOptions(arg[1])){
		sendMessage(client, _serverName, client->getNickname(), INVALID_MODE);
		return false;
	}
	std::cout << "mode" << std::endl;
	return 0;
}


void Server::init(void)
{
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("NICK", &Server::nick));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("USER", &Server::user));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("PASS", &Server::pass));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("QUIT", &Server::quit));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("JOIN", &Server::join));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("TOPIC", &Server::topic));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("NAMES", &Server::names));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("INVITE", &Server::invite));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("KICK", &Server::kick));
	_commandsMap.insert(std::make_pair<string, int (Server::*)(Client *, std::vector<string>)>("MODE", &Server::mode));
}
