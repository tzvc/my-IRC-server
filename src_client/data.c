/*
** data.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Fri Jun  9 02:58:10 2017 bufferking
** Last update Sun Jun 11 23:10:33 2017 theo champion
*/

#include "irc_client.h"

static int	get_socket(t_datacom *data, char *str)
{
  size_t	len;

  len = 0;
  if (!data->srv.stream
      && !(data->srv.stream = fdopen(data->srv.sd, "r")))
    return (print_error("fdopen"));
  errno = 0;
  if (getline(&str, &len, data->srv.stream) == -1)
    {
      if (!errno)
        {
          logmsg(ERROR, FRMT_CLOSED_CON, ANSI_BACK_CUR);
          free(str);
          free_all(data, 0);
          init_wrapper(data);
          return (pprompt(data));
        }
      else
        return (print_error("getline"));
    }
  parse_reply(data, str);
  pprompt(data);
  free(str);
  return (0);
}

int		send_data(t_datacom *data, const char *format, ...)
{
  va_list	ap;
  size_t	len;
  char		*str;

  str = NULL;
  if (data->srv.sd == -1)
    {
      logmsg(MSG, "%s\n", ERROR_NO_SRV);
      return (-1);
    }
  va_start(ap, format);
  len = vsnprintf(str, 0, format, ap);
  va_start(ap, format);
  if (!(str = malloc(sizeof(char) * (len + 1))))
    return (print_error("mem_alloc"));
  vsprintf(str, format, ap);
  rb_write(data->out, str);
  va_end(ap);
  free(str);
  return (0);
}

int	read_data(t_datacom *data, fd_set *readf)
{
  if (data->srv.sd != -1 && FD_ISSET(data->srv.sd, readf))
    if (get_socket(data, NULL) == -1)
      return (-1);
  if (FD_ISSET(0, readf))
    {
      if (parse_input(data) == -1)
        return (-1);
      pprompt(data);
    }
  return (0);
}

int		write_data(t_datacom *data, fd_set *writef)
{
  char		*str;

  if (data->srv.sd && FD_ISSET(data->srv.sd, writef))
    {
      str = rb_readline(data->out);
      if (str && (write(data->srv.sd, str, strlen(str)) == -1
                  || write(data->srv.sd, "\r\n", 2) == -1))
        return (print_error("write"));
      free(str);
    }
  return (0);
}

int	free_all(t_datacom *data, int ret)
{
  int	i;

  i = -1;
  free(data->raw_cmd);
  free(data->chan);
  free(data->in->buf);
  free(data->out->buf);
  free(data->in);
  free(data->out);
  while (++i < CMD_ARGS)
    if (data->cmd[i])
      free(data->cmd[i]);
  free(data->cmd);
  if (data->srv.stream)
    fclose(data->srv.stream);
  data->srv.sd = -1;
  return (ret);
}
