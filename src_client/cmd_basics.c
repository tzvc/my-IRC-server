/*
** cmd_basics.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Thu Jun  1 11:34:24 2017 bufferking
** Last update Fri Jun  9 02:43:58 2017 
*/

#include "irc_client.h"

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
    return (print_error("connect"));
  logmsg(INFO, "Successfuly connected to %s:%d\n", addr, port);
  return (0);
}

int	cmd_nick(t_datacom *data)
{
  if (!((data->cmd)[1]))
    return (logmsg(MSG, USAGE_FRMT, USAGE_NICK));
  return (send_data(data, FRMT_NICK, (data->cmd)[1]));
}

int	cmd_list(t_datacom *data)
{
  return (send_data(data, FRMT_LIST, (data->cmd)[1]));
}
