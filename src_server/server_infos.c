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

void		welcome_user(t_handle *h)
{
  h->sdr->status = REGISTERED;
  reply(h, RPL_WELCOME, "Welcome to my IRC server %s!%s@%s.",
        h->sdr->nick, h->sdr->username, h->sdr->hostname);
  reply(h, RPL_YOURHOST, "Your host is %s and is running version r2-rs501.",
        h->server_ip);
}

bool		cmd_list(t_handle *h)
{
  t_chan	*tmp;

  reply(h, RPL_LISTSTART, ":Start of /LIST");
  tmp = *h->chans;
  while (tmp)
    {
      if (h->arg[0] && strstr(tmp->name, h->arg[0]) == NULL)
        break;
      reply(h, RPL_LIST, "%s %lu :%s", tmp->name,
            count_users(&tmp->users), (tmp->topic ? tmp->topic : ""));
      tmp = tmp->next;
    }
  return (reply(h, RPL_LISTEND, ":End of /LIST"));
}

bool	reply_names(t_handle *h, t_chan *channel)
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
  reply(h, RPL_NAMREPLY, "= %s %s", channel->name, names);
  free(names);
  return (true);
}

bool		cmd_names(t_handle *h)
{
  t_chan	*channel;

  if (h->arg[0])
    {
      if ((channel = find_chan_by_name(h->chans, h->arg[0])) != NULL)
        reply_names(h, channel);
    }
  else
    {
      channel = *h->chans;
      while (channel)
        {
          reply_names(h, channel);
          channel = channel->next;
        }
    }
  return (reply(h, RPL_ENDOFNAMES, "End of /NAMES list."));
}

bool	cmd_ping(t_handle *h)
{
  if (!h->arg[0])
    return (reply(h, ERR_NOORIGIN, "PING :Not enough parameters"));
  return (idreply(0, h, "PONG %s :%s", h->server_ip, h->arg[0]));
}
