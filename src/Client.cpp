#include "Client.hpp"

Client::Client(int sockfd) : _sockfd(sockfd), _registration(AUTHENTICATION), _admin(false) {

}

Client::~Client() 
{
	std::cout << "deleting " << _sockfd << std::endl;
}

int Client::getSocket(void) const
{
	return (this->_sockfd);
}

Registration Client::getState(void) const{
	return _registration;
}

bool Client::isAdmin(void) const{
	return _admin;
}


string Client::getNickname(void) const
{
	return _nickname;
}

void Client::setNickname(string nickname) 
{
 	_nickname = nickname;
}

string Client::getUsername(void) const
{
	return _username;
}

void Client::setUsername(string username) 
{
 	_username = username;
}