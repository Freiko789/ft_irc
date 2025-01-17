/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/25 11:17:26 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/command.hpp"

Command::Command() {}

Command::Command(const Command&) {}

Command::~Command() {}

Command& Command::operator=(const Command& other)
{
    if (this != &other) {}
    return (*this);
}

void Command::parseIRCMessage(const std::string& command)
{
    commandName = "";
    parameters.clear();
    trailing = "";
    size_t commandEnd = command.find(' ');
    if (commandEnd != std::string::npos)
    {
        commandName = command.substr(0, commandEnd);

        size_t paramsStart = command.find(' ', commandEnd + 1);

        if (command[commandEnd + 1] == ':')
            trailing = command.substr(commandEnd + 2);
        else
        {
            std::string paramsString = command.substr(commandEnd + 1);
            std::stringstream iss(paramsString);
            std::string param;
            while (std::getline(iss, param, ' '))
            {
                if (param.find(':') != std::string::npos)
                    break;
                parameters.push_back(param);
            }
            paramsStart = command.find_last_of(':');
            if (paramsStart != std::string::npos)
            {
                trailing = command.substr(paramsStart + 1);
                if (!trailing.empty() && trailing[0] == ':')
                {
                        trailing = trailing.substr(1);
                }
            }
        }
    }
    else
        commandName = command;

    if (trailing.empty())
    {
        for (size_t i = 0; i < parameters.size(); ++i)
        {
            if (parameters[i].find("\r\n") != std::string::npos || parameters[i].find('\n') != std::string::npos)
            {
                if (parameters[i].find("\r\n") != std::string::npos)
                    parameters[i].erase(parameters[i].find("\r\n"));
                if (parameters[i].find('\n') != std::string::npos)
                    parameters[i].erase(parameters[i].find('\n'));
            }
        }
    }
    else
    {
        if (trailing.find("\r\n") != std::string::npos || trailing.find('\n') != std::string::npos)
            {
                if (trailing.find("\r\n") != std::string::npos)
                    trailing.erase(trailing.find("\r\n"));
                if (trailing.find('\n') != std::string::npos)
                    trailing.erase(trailing.find('\n'));
            }
    }
}


int Command::masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel, const std::string& serverPass, std::vector<User*> &_users)
{
    
    parseIRCMessage(command);
    if (commandName.find("CAP") != std::string::npos)
    {
        std::string response = CAP_LS_NONE();
        send(user->getSocket(), response.c_str(), response.length(), 0);
    }
	else if (commandName.find("PASS") != std::string::npos && user->isAuthentified() == false)
    {
		if (processPass(user, serverPass) == false)
            return 1;
    }
    else if (commandName.find("NICK") != std::string::npos && user->getAuthPass() == true)
        processNick(user, _users, channel);
	else if (commandName.find("USER") != std::string::npos)
    {
        if (user->isAuthentified() == true)
            processUser(user);
        else
            user->temp_USER = command;
    }
	else if (commandName.find("MODE") != std::string::npos && user->isAuthentified())
		processMode(user, channel, _users);
	else if (commandName.find("WHO") != std::string::npos || commandName.find("WHOIS") != std::string::npos)
	 	processWhoIs(user, channel, _users);
    else if (commandName.find("PING") != std::string::npos || commandName.find("PONG") != std::string::npos)
        processPing(user);
    else if (commandName.find("JOIN") != std::string::npos && user->isAuthentified())
        processJoinChannel(user, channel);
    else if (commandName.find("PART") != std::string::npos && user->isAuthentified())
        processPart(user, channel);
    else if (commandName.find("PRIVMSG") != std::string::npos && user->isAuthentified())
        processSendMess(user, channel, _users);
    else if (commandName.find("LIST") != std::string::npos && user->isAuthentified())
        processList(user, channel);
	else if (commandName.find("KICK") != std::string::npos && user->isAuthentified())
		processKick(user, channel, _users);
	else if (commandName.find("INVITE") != std::string::npos && user->isAuthentified())
		processInvite(user, channel, _users);
	else if (commandName.find("TOPIC") != std::string::npos && user->isAuthentified())
		processTopic(user, channel);
	else if (commandName.find("MODE") != std::string::npos && user->isAuthentified())
		processChannelMode(user, channel, _users);
	else if (commandName.find("QUIT") != std::string::npos)
    {
		processQuit(user);
        return 1;
    }
    else
        {
            std::string response = ERR_UNKNOWNCOMMAND(user->getNickname(), commandName);
            send(user->getSocket(), response.c_str(), response.length(), 0);
        }
    return 0;
}
