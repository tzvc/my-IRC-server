/*
** chan_interaction.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed Jun  7 11:06:47 2017 theo champion
** Last update Wed Jun  7 13:23:20 2017 theo champion
*/

#include "rfc_numlist.h"
#include "irc_server.h"

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
  broadcast(hdl, channel, "JOIN %s", channel->name);
  (channel->topic ?
   reply(hdl, RPL_TOPIC, "%s :%s", channel->name, channel->topic) :
   reply(hdl, RPL_TOPIC, "%s :No topic is set.", channel->name));
  return (reply_names(hdl, channel));
}

bool		cmd_topic(t_handle *hdl)
{
  t_chan	*channel;
  t_user	*user;

  if (!hdl->cmd_args[0])
    return (reply(hdl, ERR_NEEDMOREPARAMS, "TOPIC :Not enough parameters"));
  if ((channel = find_chan_by_name(hdl->chans, hdl->cmd_args[0])) == NULL)
    return (reply(hdl, ERR_NOTONCHANNEL,
                  "%s :You're not on that channel", hdl->cmd_args[0]));
  if (!hdl->cmd_args[1])
    return (channel->topic ?
            reply(hdl, RPL_TOPIC, "%s :%s", channel->name, channel->topic) :
            reply(hdl, RPL_TOPIC, "%s :No topic is set.", channel->name));
  if ((user = find_user_by_nick(&channel->users, hdl->sender->nick)) == NULL)
    return (reply(hdl, ERR_NOTONCHANNEL,
                  "%s :You're not on that channel", hdl->cmd_args[0]));
  //check if operator
  if ((channel->topic = strdup(hdl->cmd_args[1])) == NULL)
    return (reply(hdl, ERR_UNKNOWNERROR, "TOPIC :%s", strerror(errno)));
  return (broadcast(hdl, channel, "TOPIC %s :%s",
                    channel->name, channel->topic));
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
  (!hdl->cmd_args[1] ? broadcast(hdl, channel, "PART %s", channel->name) :
   broadcast(hdl, channel, "PART %s :%s", channel->name, hdl->cmd_args[1]));
  remove_user(&channel->users, user);
  log_msg(INFO, "Removing user \"%s\" from channel \"%s\"",
          hdl->sender->nick, channel->name);
  if (!channel->users)
    {
      log_msg(INFO, "Destroying channel \"%s\".", channel->name);
      del_chan(hdl->chans, channel);
    }
  return (true);
}
