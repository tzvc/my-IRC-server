/*
** interaction.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc/src_server
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Fri May 26 13:54:03 2017 theo champion
** Last update Sun Jun 11 17:21:35 2017 theo champion
*/

#include "irc_server.h"
#include "rfc_numlist.h"

static bool	smart_broadcast(t_handle *h, t_chan *channel,
                                int *rcp, char *newnick)
{
  bool		flag;
  t_user	*user;
  int		i;

  user = channel->users;
  while (user)
    {
      flag = false;
      i = 0;
      while (rcp[i] > 0)
        if (rcp[i++] == user->fd)
          flag = true;
      if (!flag)
        {
          rcp[i] = user->fd;
          idreply(user->fd, h, "NICK :%s", newnick);
        }
      user = user->next;
    }
  return (true);
}

static bool	update_and_broadcast_nick(t_handle *h, char *newnick)
{
  t_chan	*channel;
  t_user	*user;
  int		*rcp;
  size_t	rcp_size;
  size_t	i;

  rcp_size = count_users(h->users);
  if ((rcp = malloc(sizeof(int) * (rcp_size + 1))) == NULL)
    return (false);
  i = 0;
  while (i < (rcp_size + 1))
    rcp[i++] = 0;
  channel = *h->chans;
  while (channel)
    {
      if ((user = find_user_by_fd(&channel->users, h->sdr->fd)) != NULL)
        user->nick = newnick;
      smart_broadcast(h, channel, rcp, newnick);
      channel = channel->next;
    }
  free(rcp);
  return (true);
}

bool		cmd_nick(t_handle *h)
{
  char		*newnick;
  t_user	*user;

  if (!h->arg[0])
    return (reply(h, ERR_NONICKNAMEGIVEN, ":No nickname given"));
  if (!is_valid_nick(h->arg[0]))
    return (reply(h, ERR_ERRONEUSNICKNAME, "%s :Invalid nick", h->arg[0]));
  if ((user = find_user_by_nick(h->users, h->arg[0])) == h->sdr)
    return (true);
  if (user)
    return (reply(h, ERR_NICKNAMEINUSE,
                  "%s :Nickname already taken", h->arg[0]));
  if ((newnick = strdup(h->arg[0])) == NULL)
    return (reply(h, ERR_UNKNOWNERROR, "NICK :%s", strerror(errno)));
  if (h->sdr->status == REGISTERED && newnick)
    update_and_broadcast_nick(h, newnick);
  free(h->sdr->nick);
  h->sdr->nick = newnick;
  if (h->sdr->status == NOT_REGISTERED)
    h->sdr->status = NICK_OK;
  else if (h->sdr->status == USER_OK)
    welcome_user(h);
  log_msg(INFO, "Nickname successfully changed.");
  return (true);
}

bool		cmd_user(t_handle *h)
{
  if (!h->arg[0] || !h->arg[1] ||
      !h->arg[2] || !h->arg[3])
    return (reply(h, ERR_NEEDMOREPARAMS, "USER :Not enough parameters"));
  if (h->sdr->status == REGISTERED)
    return (reply(h, ERR_ALREADYREGISTERED, ":Already registered"));
  if ((h->sdr->username = strdup(h->arg[0])) == NULL)
    return (reply(h, ERR_UNKNOWNERROR, "NICK :%s", strerror(errno)));
  if ((h->sdr->realname = strdup(h->arg[3])) == NULL)
    return (reply(h, ERR_UNKNOWNERROR, "NICK :%s", strerror(errno)));
  if (h->sdr->status == NOT_REGISTERED)
    h->sdr->status = USER_OK;
  else if (h->sdr->status == NICK_OK)
    welcome_user(h);
  return (true);
}

bool		cmd_quit(t_handle *h)
{
  t_chan	*channel;
  t_user	*user;

  channel = *h->chans;
  while (channel)
    {
      if ((user = find_user_by_fd(&channel->users, h->sdr->fd)) == NULL)
        {
          channel = channel->next;
          continue;
        }
      log_msg(INFO, "Removing user \"%s\" from channel \"%s\"",
              h->sdr->nick, channel->name);
      remove_user(&channel->users, user);
      broadcast(h, channel, "PART %s", channel->name);
      channel = (channel->users ? channel->next : del_chan(h->chans, channel));
    }
  log_msg(INFO, "Removing user \"%s\"", h->sdr->nick);
  if (h->sdr->status == REGISTERED)
    idreply(0, h, "QUIT :%s", (h->arg[0] ? h->arg[0] : "Client Quit"));
  shutdown(h->sdr->fd, SHUT_RDWR);
  h->sdr->status = DEAD;
  return (false);
}
