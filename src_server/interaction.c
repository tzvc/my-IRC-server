/*
** interaction.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc/src_server
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Fri May 26 13:54:03 2017 theo champion
** Last update Thu Jun  1 16:20:01 2017 theo champion
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
      if ((channel = new_chan(hdl->chans, hdl->cmd_args[0])) == NULL)
        return (reply(hdl, ERR_NOSUCHCHANNEL, ":No such channel"));
    }
  else
    log_msg(INFO, "User %s is joining channel \"%s\"",
            hdl->sender->nick, channel->name);
  if (find_user_by_nick(&channel->users, hdl->sender->nick) != NULL)
    return (true);
  add_user(&channel->users,
           create_user(hdl->sender->fd,
                       hdl->sender->nick, hdl->sender->hostname, true));
  return (idreply(0, hdl, "JOIN %s", channel->name));
}

bool		cmd_part(t_handle *hdl)
{
  t_chan	*channel;
  t_user	*user;

  if (!hdl->cmd_args[0])
    return (reply(hdl, ERR_NEEDMOREPARAMS, "PART :Not enough parameters"));
  if ((channel = find_chan_by_name(hdl->chans, hdl->cmd_args[0])) == NULL)
    return (reply(hdl, ERR_NOSUCHCHANNEL,
                  "%s :No such channel", hdl->cmd_args[0]));
  if ((user = find_user_by_nick(&channel->users, hdl->sender->nick)) == NULL)
    return (reply(hdl, ERR_NOTONCHANNEL,
                  "%s :You're not on that channel", hdl->cmd_args[0]));
  remove_user(&channel->users, user);
  log_msg(INFO, "Removing user \"%s\" from channel \"%s\"", hdl->sender->nick, channel->name);
  idreply(0, hdl, "PART %s", channel->name);
  if (count_users(&channel->users) == 0)
    {
      log_msg(INFO, "Destroying channel \"%s\" after last user exited.", channel->name);
      del_chan(hdl->chans, channel);
    }
  return (true);
}

bool		cmd_quit(t_handle *hdl)
{
  t_chan	*channel;
  t_user	*user;

  channel = *hdl->chans;
  while (channel)
    {
      if ((user = find_user_by_nick(&channel->users, hdl->sender->nick)) == NULL)
        continue;
      remove_user(&channel->users, user);
      log_msg(INFO, "Removing user \"%s\" from channel \"%s\"",
              user->nick, channel->name);
      if (count_users(&channel->users) == 0)
        {
          log_msg(INFO, "Destroying channel \"%s\" after last user exited.",
                 channel->name);
          del_chan(hdl->chans, channel);
        }
    }
  log_msg(INFO, "Removing user \"%s\"\n", hdl->sender->nick);
  if (hdl->sender->status == REGISTERED)
    idreply(0, hdl, "QUIT :Client Quit");
  shutdown(hdl->sender->fd, SHUT_RDWR);
  hdl->sender->status = DEAD;
  return (false);
}
