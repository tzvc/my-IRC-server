/*
** chan_interaction.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed Jun  7 11:06:47 2017 theo champion
** Last update Sun Jun 11 20:28:46 2017 theo champion
*/

#include "rfc_numlist.h"
#include "irc_server.h"

bool		cmd_join(t_handle *h)
{
  t_chan	*channel;

  if (!h->arg[0])
    return (reply(h, ERR_NEEDMOREPARAMS, "JOIN :Not enough parameters"));
  if (!strchr(CHANMASK, h->arg[0][0]) || strlen(h->arg[0]) > 50)
    return (reply(h, ERR_NOSUCHCHANNEL, ":No such channel"));
  if ((channel = find_chan_by_name(h->chans, h->arg[0])) == NULL)
    {
      log_msg(INFO, "%s creating chan \"%s\"", h->sdr->nick, h->arg[0]);
      if ((channel = new_chan(h->chans, h->arg[0])) == NULL)
        return (reply(h, ERR_NOSUCHCHANNEL, ":No such channel"));
    }
  else
    log_msg(INFO, "%s joining chan \"%s\"", h->sdr->nick, channel->name);
  if (find_user_by_nick(&channel->users, h->sdr->nick) != NULL)
    return (true);
  add_user(&channel->users,
           create_user(h->sdr->fd,
                       h->sdr->nick, h->sdr->hostname, true));
  broadcast(h, channel, "JOIN %s", channel->name);
  (channel->topic ?
   reply(h, RPL_TOPIC, "%s :%s", channel->name, channel->topic) :
   reply(h, RPL_TOPIC, "%s :No topic is set.", channel->name));
  reply_names(h, channel);
  return (reply(h, RPL_ENDOFNAMES, "%s :End of /NAMES list.", channel->name));
}

bool		cmd_topic(t_handle *h)
{
  t_chan	*channel;
  t_user	*user;

  if (!h->arg[0])
    return (reply(h, ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters"));
  if ((channel = find_chan_by_name(h->chans, h->arg[0])) == NULL)
    return (reply(h, ERR_NOTONCHANNEL,
                  "%s :You're not on that channel", h->arg[0]));
  if (!h->arg[1])
    return (channel->topic ?
            reply(h, RPL_TOPIC, "%s :%s", channel->name, channel->topic) :
            reply(h, RPL_TOPIC, "%s :No topic is set.", channel->name));
  if ((user = find_user_by_nick(&channel->users, h->sdr->nick)) == NULL)
    return (reply(h, ERR_NOTONCHANNEL,
                  "%s :You're not on that channel", h->arg[0]));
  if ((channel->topic = strdup(h->arg[1])) == NULL)
    return (reply(h, ERR_UNKNOWNERROR, "TOPIC :%s", strerror(errno)));
  return (broadcast(h, channel, "TOPIC %s :%s",
                    channel->name, channel->topic));
}

bool		cmd_part(t_handle *h)
{
  t_chan	*channel;
  t_user	*user;

  if (!h->arg[0])
    return (reply(h, ERR_NEEDMOREPARAMS, "PART :Not enough parameters"));
  if ((channel = find_chan_by_name(h->chans, h->arg[0])) == NULL)
    return (reply(h, ERR_NOSUCHCHANNEL, "%s :No such channel", h->arg[0]));
  if ((user = find_user_by_nick(&channel->users, h->sdr->nick)) == NULL)
    return (reply(h, ERR_NOTONCHANNEL,
                  "%s :You're not on that channel", h->arg[0]));
  (!h->arg[1] ? broadcast(h, channel, "PART %s", channel->name) :
   broadcast(h, channel, "PART %s :%s", channel->name, h->arg[1]));
  remove_user(&channel->users, user);
  log_msg(INFO, "Removing user \"%s\" from channel \"%s\"",
          h->sdr->nick, channel->name);
  if (!channel->users)
    {
      log_msg(INFO, "Destroying channel \"%s\".", channel->name);
      del_chan(h->chans, channel);
    }
  return (true);
}
