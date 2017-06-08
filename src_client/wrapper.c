/*
** wrapper.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Mon May 29 17:29:25 2017 bufferking
** Last update Fri Jun  9 01:56:14 2017 
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

static int	cmdlen(void)
{
  int	i;

  i = 0;
  while (g_cmd_handler[i])
    i++;
  return (i);
}

int		send_data(t_datacom *data, const char *format, ...)
{
  va_list	ap;
  size_t	len;
  char		*str;

  str = NULL;
  if (data->srv.sd == -1)
    return (logmsg(MSG, "%s\n", ERROR_NO_SRV));
  va_start(ap, format);
  len = vsnprintf(str, 0, format, ap);
  va_start(ap, format);
  if (!(str = malloc(sizeof(char) * (len + 1))))
    return (print_error("malloc"));
  vsprintf(str, format, ap);
  rb_write(data->out, str);  
  va_end(ap);
  free(str);
  return (0);
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

int		parse_reply(t_datacom *data)
{
  char		raw[BUF_SIZE];
  char		*cmd;
  size_t	len;
  ssize_t	rd;

  len = rb_get_space(data->in);
  if ((rd = recv(data->srv.sd, raw, len, 0)) > 0)
    {
      raw[rd] = 0;
      rb_write(data->in, raw);
      //printf("Received \"%s\"\n", raw);
      while ((cmd = rb_readline(data->in)) != NULL)
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

int		parse_cmd(t_datacom *data)
{
  int	i;
  
  i = -1;
  while (g_cmd_handler[++i])
    if (!(strcmp(g_cmd_list[i], data->cmd[0])))
      {
	if (!(data->srv.sd) && strcmp(data->cmd[0], "/server")
	    && strcmp(data->cmd[0], "/quit"))
	  logmsg(MSG, "%s\n", ERROR_NO_SRV);
	else
	  i = g_cmd_handler[i](data);
	break;
      }
  /* if (i == cmdlen()) */
  /*   send_data(srv, "%s\n", cmd[0]); */
  return (i == -1 ? i : i ^ i);
}

int		parse_input(t_datacom *data)
{
  FILE		*file;
  char		*line;
  size_t	len;
  int		i;
  
  len = 0;
  i = 0;
  line = NULL;
  if (!(file = fdopen(0, "r")))
    return (print_error("fdopen"));
  if (getline(&line, &len, file) == -1)
    return (print_error("getline"));
  data->cmd[0] = strtok(line, POSIX_WS);
  while (i < 4)
    (data->cmd)[++i] = strtok(NULL, POSIX_WS);
  i = parse_cmd(data);
  free(line);
  /* if (fclose(file)) */
  /*   return (print_error("fclose")); */
  //fflush(stdin);
  return (i);
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

static int	read_data(t_datacom *data, fd_set *readf)
{
  char		*str;
  size_t	len;
  
  if (data->srv.sd && FD_ISSET(data->srv.sd, readf))
    {
      str = NULL;
      len = 0;
      if (getline(&str, &len, fdopen(data->srv.sd, "r")) == -1)
	return (print_error("getline"));
      if (str)
	printf("%s", str);
      free(str);
    }
  if (FD_ISSET(0, readf))
    if (parse_input(data) == -1)
      return (-1);
  return (0);
}

static int	write_data(t_datacom *data, fd_set *writef)
{
  char		*str;
  
  if (data->srv.sd && FD_ISSET(data->srv.sd, writef))
    {
      str = rb_readline(data->out);
      logmsg(MSG, "sending %s\n", str);
      if (write(data->srv.sd, str, strlen(str)) == -1
	  || write(data->srv.sd, "\r\n", 2) == -1)
      	return (print_error("write"));      
      free(str);
    }
  return (0);
}

static int		free_all(t_datacom *data, int ret)
{
  free(data->cmd);
  free(data->in->buf);
  free(data->out->buf);
  free(data->in);
  free(data->out);
  if (data->srv.sd != -1)
    close(data->srv.sd);
  return (ret);
}

int		client_wrapper(void)
{
  fd_set	writef;
  fd_set	readf;
  int		fd_max;
  t_datacom	data;

  if (init_wrapper(&data))
    return (EXIT_FAILURE);
  while (42)
    {
      update_fds(&writef, &readf, &fd_max, &data);
      if (select(fd_max + 1, &readf, &writef, NULL, NULL) == -1)
	return (print_error("select"));
      if (read_data(&data, &readf) == -1
	  || write_data(&data, &writef) == -1)
	return (free_all(&data, EXIT_FAILURE));
    }  
  return (free_all(&data, EXIT_SUCCESS));
}
