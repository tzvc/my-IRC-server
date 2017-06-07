/*
** interaction.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc/src_server
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Fri May 26 13:54:03 2017 theo champion
** Last update Wed Jun  7 11:08:10 2017 theo champion
*/

#include "irc_server.h"
#include "rfc_numlist.h"

bool		cmd_nick(t_handle *hdl)
{
  if (!hdl->cmd_args[0])
    return (reply(hdl, ERR_NONICKNAMEGIVEN, ":No nickname given"));
  if (find_user_by_nick(hdl->users, hdl->cmd_args[0]) != NULL)
    return (reply(hdl, ERR_NICKCOLLISION, ":Nickname already taken"));
  hdl->sender->nick = strdup(hdl->cmd_args[0]);
  if (hdl->sender->status == NOT_REGISTERED)
    hdl->sender->status = NICK_OK;
  else if (hdl->sender->status == USER_OK)
    welcome_user(hdl);
  log_msg(INFO, "Nickname successfully changed.");
  return (true);
}

bool		cmd_user(t_handle *hdl)
{
  if (!hdl->cmd_args[0] || !hdl->cmd_args[1] ||
      !hdl->cmd_args[2] || !hdl->cmd_args[3])
    return (reply(hdl, ERR_NEEDMOREPARAMS, "USER :Not enough parameters"));
  hdl->sender->username = strdup(hdl->cmd_args[0]);
  hdl->sender->realname = strdup(hdl->cmd_args[3]);
  if (hdl->sender->status == NOT_REGISTERED)
    hdl->sender->status = USER_OK;
  else if (hdl->sender->status == NICK_OK)
    welcome_user(hdl);
  return (true);
}

bool		cmd_quit(t_handle *hdl)
{
  t_chan	*channel;
  t_user	*user;

  channel = *hdl->chans;
  while (channel)
    {
      if ((user = find_user_by_fd(&channel->users, hdl->sender->fd)) == NULL)
        continue;
      log_msg(INFO, "Removing user \"%s\" from channel \"%s\"",
              hdl->sender->nick, channel->name);
      remove_user(&channel->users, user);
      broadcast(hdl, channel, "PART %s", channel->name);
      if (channel->users == NULL)
        channel = del_chan(hdl->chans, channel);
      else
        channel = channel->next;
    }
  log_msg(INFO, "Removing user \"%s\"\n", hdl->sender->nick);
  if (hdl->sender->status == REGISTERED)
    idreply(0, hdl, "QUIT :Client Quit");
  shutdown(hdl->sender->fd, SHUT_RDWR);
  hdl->sender->status = DEAD;
  return (false);
}
