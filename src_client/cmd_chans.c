/*
** cmd_chan.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Sat Jun 10 17:03:19 2017 bufferking
** Last update Sun Jun 11 16:01:48 2017 bufferking
*/

#include "irc_client.h"

int	cmd_join(t_datacom *data)
{
  if (data->chan && cmd_part(data) == -1)
    return (0);
  if (!data->cmd[1] || data->cmd[1][0] != '#')
    return (logmsg(MSG, USAGE_FRMT, USAGE_JOIN));
  if (!(data->chan = malloc(sizeof(char) * (strlen(data->cmd[1]) + 1))))
    return (print_error("malloc"));
  if (!(data->chan = strcpy(data->chan, data->cmd[1])))
    return (print_error("strcpy"));
  if (send_data(data, FRMT_JOIN, data->chan) == -1)
    return (0);
  return (0);
}

int	cmd_part(t_datacom *data)
{
  if (!data->chan)
    return (logmsg(MSG, "%s\n", ERROR_NO_CHAN));
  if (send_data(data, FRMT_PART, data->chan) == -1)
    return (0);
  free(data->chan);
  data->chan = NULL;
  return (0);
}

int	cmd_topic(t_datacom *data)
{
  if (!data->cmd[1])
    return (logmsg(MSG, USAGE_FRMT, USAGE_TOPIC));
  if (!data->chan)
    return (logmsg(MSG, "%s\n", ERROR_NO_CHAN));
  return (send_data(data, FRMT_TOPIC, data->chan, &(data->raw_cmd[7])));
}
