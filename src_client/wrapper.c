/*
** wrapper.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Mon May 29 17:29:25 2017 bufferking
** Last update Wed May 31 19:30:56 2017 
*/

#include "irc_client.h"

const char	*g_cmd_list[] =
  {
    "/quit", "/server", "/nick", "/list", "/join", "/part",
    "/users", "/names", "/msg", "/accept_file"
  };

t_comm_handler	g_cmd_handler[] =
  {
    cmd_quit, cmd_server, cmd_nick
  };

int	send_data(t_server *srv, const char *format, ...)
{
  va_list	args;

  va_start(args, format);
  vdprintf(srv->sd, format, args);
  va_end(args);
  return (0);
}

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
  (void)srv;
  (void)cmd;
  if (!(cmd[1]))
    return (logmsg(MSG, USAGE_FRMT, USAGE_NICK));
  send_data(srv, FRMT_NICK, cmd[1]);
  return (0);
}

static int	init_wrapper(t_server *srv, char ***cmd, FILE **in)
{
  int		i;
  
  i = 0;
  srv->sd = 0;
  srv->addr.sin_family = AF_INET;
  if (!(*in = fdopen(0, "r")))
    return (print_error("fdopen"));
  if (!(*cmd = malloc(sizeof(char *) * 5)))
    return (print_error("malloc"));
  
  while (i < 5)
    (*cmd)[i++] = NULL;
  return (0);
}

int	client_wrapper(void)
{
  char		*raw;
  char		**cmd;
  size_t	size;
  FILE		*in;
  t_server	srv;
  int		i;
  
  raw = NULL;
  size = 0;
  i = 0;
  if (init_wrapper(&srv, &cmd, &in))
    return (EXIT_FAILURE);
  while (i != -1 && getline(&raw, &size, in) != -1)
    {
      i = 0;
      cmd[0] = strtok(raw, " \n");
      while (i < 4)
	cmd[++i] = strtok(NULL, " \n");
      i = -1;
      //printf("%s | %s | %s\n", cmd[0], cmd[1], cmd[2]);
      while (g_cmd_handler[++i])
	if (!(strcmp(g_cmd_list[i], cmd[0])))
	  {
	    if (!srv.sd && strcmp(cmd[0], "/server") && strcmp(cmd[0], "/quit"))
	      logmsg(MSG, "You have to be connected to a server first.\n");
	    else
	      i = g_cmd_handler[i](&srv, cmd);
	    break;
	  }
    }
  free(cmd);
  free(raw);
  fclose(in);
  return (EXIT_SUCCESS);
}
