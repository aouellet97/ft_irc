#include "Client.hpp"

Client::Client(int sockfd) : _sockfd(sockfd), _registration(AUTHENTICATION), clientInput(""), _inLimechat(true) {

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

// bool Client::isAdmin(void) const{
// 	return _admin;
// }

bool Client::getLimeState(void) const
{
	return _inLimechat;
}

void Client::setLimeState(bool toSet){
	_inLimechat = toSet;
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

void Client::setState(Registration newState){
	_registration = newState;
}
void Client::checkIdentified(void){
	if(this->getState() == IDENTIFICATION){
		if(!getUsername().empty() && !getNickname().empty()){
			this->setState(REGISTERED);
			
			string ircMessage = ":" + _nickname +  PVM + _nickname + " :" + SUCCESS_REGISTER + "\r\n"; //<---- format
			send(getSocket(), ircMessage.c_str(), ircMessage.length(), 0); 
		}
	}
}

void Client::addChannel(Channel* channel) // adds the channel to _channels
{
	if (this->_channels.size() < this->_channels.capacity())
	{
		this->_channels.push_back(channel);
	}
}

void Client::removeChannel(Channel* channel) // removes the channel from _channels
{
	for (chIt it = _channels.begin(); it < _channels.end(); it++)
	{
		if ((*it)->getName() == channel->getName())
		{
			_channels.erase(it);
			return ;
		}
	}
}