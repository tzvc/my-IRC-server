/*
** wrapper.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Mon May 29 17:29:25 2017 bufferking
** Last update Fri Jun  2 18:26:55 2017 
*/

#include "irc_client.h"

const char	*g_cmd_list[] =
  {
    "/quit", "/server", "/nick", "/list", "/join", "/part",
    "/users", "/names", "/msg", "/accept_file"
  };

t_comm_handler	g_cmd_handler[] =
  {
    cmd_quit, cmd_server, cmd_nick, cmd_list
  };

int	cmdlen(void)
{
  int	i;

  i = 0;
  while (g_cmd_handler[i])
    i++;
  return (i);
}

int		send_data(t_server *srv, const char *format, ...)
{
  va_list	args;

  if (srv->sd == -1)
    return (logmsg(MSG, "%s\n", ERROR_NO_SRV));
  va_start(args, format);
  vdprintf(srv->sd, format, args);
  va_end(args);
  return (0);
}

int		recv_data(t_server *srv, char **buffer)
{
  size_t	len;

  len = 0;
  if (getline(buffer, &len, fdopen(srv->sd, "r")) == -1)
    return (print_error("getline"));
  return (len);
}

static int	init_wrapper(t_server *srv, char ***cmd, t_rb **rb)
{
  int		i;
  
  i = 0;
  srv->sd = -1;
  srv->addr.sin_family = AF_INET;
  if (!(*cmd = malloc(sizeof(char *) * cmdlen())))
    return (print_error("malloc"));  
  while (i < cmdlen())
    (*cmd)[i++] = NULL;
  *rb = rb_init();
  return (0);
}

/*

int	client_wrapper(void)
{
  char		*raw;
  char		**cmd;
  size_t	size;
  FILE		*in;
  t_server	srv;
  int		i;
  fd_set	set;
  
  raw = NULL;
  cmdlen();
  size = 0;
  i = 0;
  if (init_wrapper(&srv, &cmd, &in))
    return (EXIT_FAILURE);
  while (42)
    {
      FD_ZERO(&set);
      FD_SET(0, &set);
      if (srv.sd != -1)
	FD_SET(srv.sd, &set);
      select((srv.sd > 0 ? srv.sd : 0) + 1, &set, &set, NULL, NULL);
      if (FD_ISSET(srv.sd, &set))
	{
	  recv_data(&srv, &raw);
	  logmsg(MSG, "message incoming : %s\n", raw);
	}
      if (FD_ISSET(0, &set))
	{
	  if (i != -1 && getline(&raw, &size, in) != -1)
	    {
	      i = 0;
	      cmd[0] = strtok(raw, " \n");
	      while (i < 4)
		cmd[++i] = strtok(NULL, " \n");
	      i = -1;
	      while (g_cmd_handler[++i])
		if (!(strcmp(g_cmd_list[i], cmd[0])))
		  {
		    if (!srv.sd && strcmp(cmd[0], "/server") && strcmp(cmd[0], "/quit"))
		      logmsg(MSG, "%s\n", ERROR_NO_SRV);
		    else
		      i = g_cmd_handler[i](&srv, cmd);
		    break;
		  }
	      if (i == cmdlen())
		send_data(&srv, "%s\n", raw);
	    }
	}
    }


  
  while (i != -1 && getline(&raw, &size, in) != -1)
    {
      i = 0;
      cmd[0] = strtok(raw, " \n");
      while (i < 4)
  	cmd[++i] = strtok(NULL, " \n");
      i = -1;
      while (g_cmd_handler[++i])
  	if (!(strcmp(g_cmd_list[i], cmd[0])))
  	  {
  	    if (!srv.sd && strcmp(cmd[0], "/server") && strcmp(cmd[0], "/quit"))
  	      logmsg(MSG, "%s\n", ERROR_NO_SRV);
  	    else
  	      i = g_cmd_handler[i](&srv, cmd);
  	    break;
  	  }
      if (i == cmdlen())
  	send_data(&srv, "%s\n", raw);
    }
  free(cmd);
  free(raw);
  fclose(in);
  if (srv.sd != -1)
    close(srv.sd);
  return (EXIT_SUCCESS);
}

*/

static void	update_fds(fd_set *fds, int *fd_max, t_server *srv)
{
  FD_ZERO(fds);
  FD_SET(STDIN_FILENO, fds);
  if (srv->sd != -1)
    {
      //printf("server detected\n");
      FD_SET(srv->sd, fds);
    }
  *fd_max = (srv->sd > 0 ? srv->sd : 0);
  //printf("fd max = %d\n", *fd_max);
}

int		parse_reply(t_server *srv, t_rb *rb)
{
  char		raw[BUF_SIZE];
  char		*cmd;
  size_t	len;
  ssize_t	rd;

  len = rb_get_space(rb);
  if ((rd = recv(srv->sd, raw, len, 0)) > 0)
    {
      raw[rd] = 0;
      rb_write(rb, raw);
      //printf("Received \"%s\"\n", raw);
      while ((cmd = rb_readline(rb)) != NULL)
        {
          //printf("Command \"%s\"\n", cmd);
          //parse_cmd(hdl, cmd);
          free(cmd);
        }
    }
  else if (rd == 0)
    {
      logmsg(MSG, "Disconnected from server\n");
      //cmd_quit(hdl);
    }
  else
    return (print_error("recv"));
  return (EXIT_SUCCESS);
}

int		parse_cmd(t_server *srv, char **cmd)
{
  int	i;
  
  i = -1;
  while (g_cmd_handler[++i])
    if (!(strcmp(g_cmd_list[i], cmd[0])))
      {
	if (!srv->sd && strcmp(cmd[0], "/server") && strcmp(cmd[0], "/quit"))
	  logmsg(MSG, "%s\n", ERROR_NO_SRV);
	else
	  i = g_cmd_handler[i](srv, cmd);
	break;
      }
  if (i == cmdlen())
    send_data(srv, "%s\n", cmd[0]);
  return (i == -1 ? i : i ^ i);
}

int		parse_input(t_server *srv, t_rb *rb, char **cmd)
{
  char		raw[BUF_SIZE];
  char		*line;
  size_t	len;
  ssize_t	rd;
  int	i;

  len = rb_get_space(rb);
  if ((rd = read(0, raw, len)) != -1)
    {
      raw[rd] = 0;
      rb_write(rb, raw);
      while ((line = rb_readline(rb)) != NULL)
	{
	  i = 0;
	  cmd[0] = strtok(line, POSIX_WS);
	  while (i < 4)
	    cmd[++i] = strtok(NULL, POSIX_WS);
	  i = parse_cmd(srv, cmd);
	  free(line);
	  if (i == -1)
	    break;
	}
    }
  else
    return (print_error("read"));
  fflush(stdin);
  return (i);
}

  int		client_wrapper(void)
  {
    fd_set	fds;
    int		fd_max;
    t_server	srv;
    t_rb	*rb;
    char	**cmd;
    int		i;
    
    if (init_wrapper(&srv, &cmd, &rb))
      return (EXIT_FAILURE);
    while (42)
      {
	update_fds(&fds, &fd_max, &srv);
	if (select(fd_max + 1, &fds, &fds, NULL, NULL) == -1)
	  return (print_error("select"));
	if (FD_ISSET(srv.sd, &fds))
	  parse_reply(&srv, rb);
	i = parse_input(&srv, rb, cmd);
	if (i == -1)
	  break;
      }
    free(cmd);
    free(rb->buf);
    free(rb);
    if (srv.sd != -1)
      close(srv.sd);
    return (EXIT_SUCCESS);
  }
