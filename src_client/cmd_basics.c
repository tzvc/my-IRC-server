/*
** cmd_basics.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Thu Jun  1 11:34:24 2017 bufferking
** Last update Thu Jun  1 11:36:52 2017 
*/

#include "irc_client.h"

int	cmd_quit(t_server *srv, char **cmd)
{
  (void)srv;
  (void)cmd;
  logmsg(MSG, "%s\n", BYE_MSG);
  return (-1);
}

int	cmd_server(t_server *srv, char **cmd)
{
  char	*addr;
  char	*portstr;
  int	port;

  addr = strtok(cmd[1], ":");
  portstr = strtok(NULL, ":");
  port = (portstr ? atoi(portstr) : DEFAULT_PORT);
  if (!cmd[1])
    return (logmsg(MSG, USAGE_FRMT, USAGE_SERVER));
  if (srv->sd)
    close(srv->sd);
  if ((srv->sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    return (print_error("socket"));
  srv->addr.sin_addr.s_addr = inet_addr(cmd[1]);
  srv->addr.sin_port = htons(port);
  if (connect(srv->sd, (struct sockaddr*)&(srv->addr), sizeof(srv->addr)) != 0)
    return (print_error("connect"));
  logmsg(INFO, "Successfuly connected to %s:%d\n", addr, port);
  return (0);
}

int	cmd_nick(t_server *srv, char **cmd)
{
  if (!(cmd[1]))
    return (logmsg(MSG, USAGE_FRMT, USAGE_NICK));
  send_data(srv, FRMT_NICK, cmd[1]);
  return (0);
}

int	cmd_list(t_server *srv, char **cmd)
{
  send_data(srv, FRMT_LIST, cmd[1]);
  return (0);
}
