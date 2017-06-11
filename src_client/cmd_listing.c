/*
** cmd_listing.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Sun Jun 11 15:51:12 2017 bufferking
** Last update Sun Jun 11 15:52:30 2017 bufferking
*/

#include "irc_client.h"

inline int	cmd_list(t_datacom *data)
{
  return (send_data(data, FRMT_LIST, (data->cmd)[1]));
}

inline int	cmd_users(t_datacom *data)
{
  return (send_data(data, FRMT_USERS));
}

int	cmd_names(t_datacom *data)
{
  if (!data->cmd[1] || !data->cmd[1][0])
    return (logmsg(MSG, USAGE_FRMT, USAGE_NAMES));
  return (send_data(data, FRMT_NAMES, data->cmd[1]));
}
