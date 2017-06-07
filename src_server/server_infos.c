/*
** server_infos.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Thu May 25 16:01:37 2017 theo champion
** Last update Wed Jun  7 12:50:04 2017 theo champion
*/

#include "rfc_numlist.h"
#include "irc_server.h"

void		welcome_user(t_handle *hdl)
{
  hdl->sender->status = REGISTERED;
  reply(hdl, RPL_WELCOME, "Welcome to my IRC server %s!%s@%s.",
        hdl->sender->nick, hdl->sender->username, hdl->sender->hostname);
  reply(hdl, RPL_YOURHOST, "Your host is %s and is running version r2-rs501.",
        hdl->server_ip);
}

bool		cmd_list(t_handle *hdl)
{
  t_chan	*tmp;

  reply(hdl, RPL_LISTSTART, ":Start of /LIST");
  tmp = *hdl->chans;
  while (tmp)
    {
      if (hdl->cmd_args[0] && strstr(tmp->name, hdl->cmd_args[0]) == NULL)
        break;
      reply(hdl, RPL_LIST, "%s %lu :%s", tmp->name,
            count_users(&tmp->users), (tmp->topic ? tmp->topic : ""));
      tmp = tmp->next;
    }
  return (reply(hdl, RPL_LISTEND, ":End of /LIST"));
}

bool	reply_names(t_handle *hdl, t_chan *channel)
{
  t_user	*user;
  char		*names;
  size_t	size;

  user = channel->users;
  size = 0;
  while (user)
    {
      size += (strlen(user->nick) + 1);
      user = user->next;
    }
  names = NULL;
  if ((names = calloc(size + 1, 1)) == NULL)
    return (false);
  user = channel->users;
  while (user)
    {
      strcat(names, user->nick);
      if (user->next)
        strcat(names, " ");
      user = user->next;
    }
  reply(hdl, RPL_NAMREPLY, "= %s %s", channel->name, names);
  free(names);
  return (true);
}

bool		cmd_names(t_handle *hdl)
{
  t_chan	*channel;

  if (hdl->cmd_args[0])
    {
      if ((channel = find_chan_by_name(hdl->chans, hdl->cmd_args[0])) != NULL)
        reply_names(hdl, channel);
    }
  else
    {
      channel = *hdl->chans;
      while (channel)
        {
          reply_names(hdl, channel);
          channel = channel->next;
        }
    }
  return (reply(hdl, RPL_ENDOFNAMES, "End of /NAMES list."));
}

bool	cmd_ping(t_handle *hdl)
{
  if (!hdl->cmd_args[0])
    return (reply(hdl, ERR_NOORIGIN, "PING :Not enough parameters"));
  return (idreply(0, hdl, "PONG %s :%s", hdl->server_ip, hdl->cmd_args[0]));
}
