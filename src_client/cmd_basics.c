/*
** cmd_basics.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Thu Jun  1 11:34:24 2017 bufferking
** Last update Sun Jun 11 22:41:54 2017 bufferking
*/

#include "irc_client.h"

extern const char	*g_cmd_list[];

int	cmd_help(t_datacom *data)
{
  int	i;

  (void)data;
  i = 0;
  logmsg(MSG, "Avaiable commands :\n");
  while (g_cmd_list[i])
    logmsg(MSG, "%s\n", g_cmd_list[i++]);
  return (0);
}

int	cmd_quit(t_datacom *data)
{
  (void)data;
  logmsg(MSG, "%s\n", BYE_MSG);
  return (-1);
}

int	cmd_server(t_datacom *data)
{
  char	*addr;
  char	*portstr;
  int	port;

  addr = strtok((data->cmd)[1], ":");
  portstr = strtok(NULL, ":");
  port = (portstr ? atoi(portstr) : DEFAULT_PORT);
  if (!(data->cmd)[1])
    return (logmsg(MSG, USAGE_FRMT, USAGE_SERVER));
  if (data->srv.sd != -1)
    close(data->srv.sd);
  if ((data->srv.sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return (print_error("socket"));
  data->srv.addr.sin_addr.s_addr = inet_addr((data->cmd)[1]);
  data->srv.addr.sin_port = htons(port);
  if (connect(data->srv.sd, (struct sockaddr*)&(data->srv.addr),
              sizeof(data->srv.addr)) != 0)
    {
      close(data->srv.sd);
      data->srv.sd = -1;
      return (print_error("connect"));
    }
  logmsg(INFO, "Successfuly connected to %s:%d\n", addr, port);
  return (0);
}

int	cmd_nick(t_datacom *data)
{
  if (!((data->cmd)[1]))
    return (logmsg(MSG, USAGE_FRMT, USAGE_NICK));
  if (send_data(data, FRMT_NICK, (data->cmd)[1]) == -1)
    return (0);
  return (send_data(data, "USER %s %s %s %\n", data->cmd[1], data->cmd[1],
                    (data->cmd)[1], (data->cmd)[1]));
}

int	cmd_msg(t_datacom *data)
{
  int	offset;
  char	*param;

  if (!strcmp(data->cmd[0], "/msg") && !data->cmd[2])
    return (logmsg(MSG, USAGE_FRMT, USAGE_MSG));
  if (strcmp(data->cmd[0], "/msg") && !data->chan)
    return (logmsg(MSG, "%s\n", ERROR_NO_CHAN));
  param = strcmp(data->cmd[0], "/msg") ? data->chan : data->cmd[1];
  offset = strcmp(data->cmd[0], "/msg") ? 0 : strlen(param) + 6;
  if (send_data(data, FRMT_MSG, param, &(data->raw_cmd[offset])) == -1)
    return (0);
  return (0);
}
