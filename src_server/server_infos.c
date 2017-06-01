/*
** server_infos.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Thu May 25 16:01:37 2017 theo champion
** Last update Thu Jun  1 16:06:54 2017 theo champion
*/

#include "rfc_numlist.h"
#include "irc_server.h"

void		welcome_user(t_handle *hdl)
{
  hdl->sender->status = REGISTERED;
  reply(hdl, RPL_WELCOME, "Welcome to my IRC server %s!%s@%s",
        hdl->sender->nick, hdl->sender->username, hdl->sender->hostname);
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

static bool	reply_names(t_handle *hdl, t_chan *channel)
{
  t_user	*user;
  char		*names;

  log_msg(DEBUG, "sending reply name");
  user = channel->users;
  while (user)
    {
      names = dyn_strcat(names, user->nick);
      printf("%s\n", names);
      free(names);
    }
  return (reply(hdl, RPL_NAMREPLY, "= %s %s", channel->name, names));
}

bool		cmd_names(t_handle *hdl)
{
  t_chan	*channel;

  if (hdl->cmd_args[0])
    {
      if ((channel = find_chan_by_name(hdl->chans, hdl->cmd_args[0])) == NULL)
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
  return(idreply(0, hdl, "PONG %s :%s", hdl->server_ip, hdl->cmd_args[0]));
}
