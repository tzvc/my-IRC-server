/*
** wrapper.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Mon May 29 17:29:25 2017 bufferking
** Last update Fri Jun  9 03:02:35 2017 
*/

#include "irc_client.h"

extern volatile bool	g_client_running;

const char	*g_cmd_list[] =
  {
    "/quit", "/server", "/nick", "/list", "/join", "/part",
    "/users", "/names", "/msg", "/accept_file"
  };

t_comm_handler	g_cmd_handler[] =
  {
    cmd_quit, cmd_server, cmd_nick, cmd_list
  };

static int	cmdlen(void)
{
  int	i;

  i = 0;
  while (g_cmd_handler[i])
    i++;
  return (i);
}

static int	init_wrapper(t_datacom *data)
{
  int		i;
  
  i = 0;
  data->srv.sd = -1;
  data->srv.addr.sin_family = AF_INET;
  if (!(data->cmd = malloc(sizeof(char *) * cmdlen())))
    return (print_error("malloc"));
  while (i < cmdlen())
    (data->cmd)[i++] = NULL;
  data->in = rb_init();
  data->out = rb_init();
  return (0);
}

static void	update_fds(fd_set *writef, fd_set *readf,
			   int *fd_max, t_datacom *data)
{
  if (writef)
    FD_ZERO(writef);
  if (readf)
    FD_ZERO(readf);
  if (data->srv.sd != -1)
    {
      if (data->out->rend != data->out->wend)
	writef ? FD_SET(data->srv.sd, writef) : 0;
      readf ? FD_SET(data->srv.sd, readf) : 0;
    }
  readf ? FD_SET(0, readf) : 0;
  *fd_max = (data->srv.sd > 0 ? data->srv.sd : 0);
}

int		client_wrapper(void)
{
  fd_set	writef;
  fd_set	readf;
  int		fd_max;
  t_datacom	data;

  if (init_wrapper(&data))
    return (EXIT_FAILURE);
  g_client_running = true;
  logmsg(MSG, "%s\n", WELC_MSG);
  write(1, IRC_PROMPT, strlen(IRC_PROMPT));
  while (g_client_running)
    {
      update_fds(&writef, &readf, &fd_max, &data);
      if (select(fd_max + 1, &readf, &writef, NULL, NULL) == -1 && g_client_running)
	return (print_error("select"));
      if (g_client_running && (read_data(&data, &readf) == -1
			       || write_data(&data, &writef) == -1))
	return (free_all(&data, EXIT_FAILURE));
    }
  return (free_all(&data, EXIT_SUCCESS));
}
