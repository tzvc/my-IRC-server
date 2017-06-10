/*
** parser.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Fri Jun  9 02:58:54 2017 bufferking
** Last update Sat Jun 10 22:51:46 2017 bufferking
*/

#include "irc_client.h"

extern const char	*g_cmd_list[];
extern t_comm_handler	g_cmd_handler[];

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
  if (!(data->cmd[0]))
    return (0);
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
  if (i == cmdlen() && data->raw_cmd[0] == '/')
    return (logmsg(MSG, "%s\n", ERROR_UNKNOWN_COMMAND));
  else if (i == cmdlen())
    cmd_msg(data);
  return (i == -1 ? i : i ^ i);
}

int		parse_input(t_datacom *data)
{
  FILE		*file;
  int		i;
  static size_t	len;

  i = 0;
  if (!(file = fdopen(0, "r")))
    return (print_error("fdopen"));
  if (getline(&(data->raw_cmd), &len, file) == -1)
    return (print_error("getline"));
  // NEED TO FREE CMD //
  data->cmd[0] = strtok(data->raw_cmd, POSIX_WS);
  while (i < 4)
    (data->cmd)[++i] = strtok(NULL, POSIX_WS);
  i = parse_cmd(data);
  return (i);
}
