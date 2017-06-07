/*
** messages.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc/src_server
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed May 31 16:55:02 2017 theo champion
** Last update Tue Jun  6 16:46:45 2017 theo champion
*/

#include "rfc_numlist.h"
#include "irc_server.h"

static bool	message_channel(t_handle *hdl, t_chan *channel)
{
  t_user	*tmp_user;

  tmp_user = channel->users;
  while (tmp_user)
    {
      if (tmp_user->fd != hdl->sender->fd)
        idreply(tmp_user->fd, hdl, "PRIVMSG %s :%s",
                hdl->cmd_args[0], hdl->cmd_args[1]);
      tmp_user = tmp_user->next;
    }
  return (true);
}

bool		cmd_privmsg(t_handle *hdl)
{
  t_user	*user;
  t_chan	*channel;

  if (!hdl->cmd_args[0])
    return (reply(hdl, ERR_NORECIPIENT, ":No recipient given (PRIVMSG)"));
  if (!hdl->cmd_args[1])
    return (reply(hdl, ERR_NOTEXTTOSEND, ":No text to send"));
  if (hdl->cmd_args[0][0] == '#')
    {
      if ((channel = find_chan_by_name(hdl->chans, hdl->cmd_args[0])) == NULL)
        return (reply(hdl, ERR_CANNOTSENDTOCHAN,
                      "%s :No such channel", hdl->cmd_args[0]));
      if ((find_user_by_fd(&channel->users, hdl->sender->fd) == NULL))
        return (reply(hdl, ERR_CANNOTSENDTOCHAN,
                      "%s :You're not on that channel", hdl->cmd_args[0]));
      return (message_channel(hdl, channel));
    }
  else
    {
      if (((user = find_user_by_nick(hdl->users, hdl->cmd_args[0])) == NULL) ||
          user->status != REGISTERED)
        return (reply(hdl, ERR_NOSUCHNICK,
                      "%s :No such nickname", hdl->cmd_args[0]));
      return (idreply(user->fd, hdl, "PRIVMSG %s :%s",
                      hdl->cmd_args[0], hdl->cmd_args[1]));
    }
}
