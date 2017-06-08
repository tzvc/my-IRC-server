/*
** interaction.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc/src_server
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Fri May 26 13:54:03 2017 theo champion
** Last update Thu Jun  8 10:49:25 2017 theo champion
*/

#include "irc_server.h"
#include "rfc_numlist.h"

static void	update_and_broadcast_nick(t_handle *h, char *newnick)
{
  t_chan	*channel;
  t_user	*user;

  idreply(0, h, "NICK :%s", newnick);
  channel = *h->chans;
  while (channel)
    {
      if ((user = find_user_by_fd(&channel->users, h->sdr->fd)) != NULL)
        {
          broadcast(h, channel, "NICK :%s", newnick);
          user->nick = newnick;
        }
      channel = channel->next;
    }
}

bool		cmd_nick(t_handle *h)
{
  char		*newnick;

  if (!h->arg[0])
    return (reply(h, ERR_NONICKNAMEGIVEN, ":No nickname given"));
  if (strlen(h->arg[0]) > MAXNICK)
    return (reply(h, ERR_ERRONEUSNICKNAME,
                  "%s :Nick must be 9 char max", h->arg[0]));
  if (find_user_by_nick(h->users, h->arg[0]) != NULL)
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
  h->sdr->username = strdup(h->arg[0]);
  h->sdr->realname = strdup(h->arg[3]);
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
        continue;
      log_msg(INFO, "Removing user \"%s\" from channel \"%s\"",
              h->sdr->nick, channel->name);
      remove_user(&channel->users, user);
      broadcast(h, channel, "PART %s", channel->name);
      if (channel->users == NULL)
        channel = del_chan(h->chans, channel);
      else
        channel = channel->next;
    }
  log_msg(INFO, "Removing user \"%s\"", h->sdr->nick);
  if (h->sdr->status == REGISTERED)
    idreply(0, h, "QUIT :Client Quit");
  shutdown(h->sdr->fd, SHUT_RDWR);
  h->sdr->status = DEAD;
  return (false);
}
