/*
** interaction.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc/src_server
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Fri May 26 13:54:03 2017 theo champion
** Last update Fri May 26 16:07:43 2017 theo champion
*/

#include "irc_server.h"
#include "rfc_numlist.h"

bool		cmd_nick(t_handle *hdl)
{
  if (!hdl->cmd_args[0])
    return (reply(hdl, ERR_NONICKNAMEGIVEN, ":No nickname given"));
  //check if nick is valid
  if (find_user_by_nick(hdl->users, hdl->cmd_args[0]) != NULL)
    return (reply(hdl, ERR_NICKCOLLISION, ":Nickname already taken"));
  hdl->sender->nick = strdup(hdl->cmd_args[0]);
  return (reply(hdl, 300, "%s :Nickname changed", hdl->cmd_args[0]));
}

bool		cmd_join(t_handle *hdl)
{
  t_chan	*channel;

  if (!hdl->cmd_args[0])
    return (reply(hdl, ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters"));
  if (hdl->cmd_args[0][0] != '#')
    return (reply(hdl, ERR_NOSUCHCHANNEL, ":No such channel"));
  if ((channel = find_chan_by_name(hdl->chans, hdl->cmd_args[0])) == NULL)
    {
      log_msg(INFO, "User %s is creating channel \"%s\"",
              hdl->sender->nick, hdl->cmd_args[0]);
      if ((channel = new_chan(hdl->chans, hdl->cmd_args[0], NULL)) == NULL)
        return (reply(hdl, ERR_NOSUCHCHANNEL, ":No such channel"));
    }
  else
    log_msg(INFO, "User %s is joining channel \"%s\"",
            hdl->sender->nick, channel->name);
  if (find_user_by_nick(&channel->users, hdl->sender->nick) != NULL)
    return (true);
  add_user(&channel->users, create_user(hdl->sender->fd,
                                        hdl->sender->nick, hdl->sender->host));
  return (reply(hdl, ERR_NOSUCHCHANNEL, ":Channel joined"));
}
