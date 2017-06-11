/*
** parser.c for PSU_2016_myirc in /home/antoine.cauquil/rendu/network/PSU_2016_myirc/src_client
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Fri Jun  9 02:58:54 2017 bufferking
** Last update Sun Jun 11 22:41:16 2017 bufferking
*/

#include "irc_client.h"

extern const char	*g_cmd_list[];
extern t_comm_handler	g_cmd_handler[];

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
  int		i;
  static size_t	len;
  char		*raw;
  int		ret;

  i = 0;
  if (getline(&(data->raw_cmd), &len, stdin) == -1)
    return (print_error("getline"));
  if (!(raw = strdup(data->raw_cmd)))
    return (print_error("strdup"));
  data->cmd[0] = strtok(raw, POSIX_WS);
  while (i < CMD_ARGS)
    data->cmd[++i] = strtok(NULL, POSIX_WS);
  ret = parse_cmd(data);
  free(data->cmd[0]);
  while (--i >= 0)
    data->cmd[i] = NULL;
  return (ret);
}

int		parse_reply(t_datacom *data, const char *str)
{
  char		*sub;

  if (!(sub = strstr(str, MSG_NEEDLE)))
    return (printf("%s%s", ANSI_BACK_CUR, str));
  sub += strlen(MSG_NEEDLE) + 1;
  printf("%s%s%s%.*s%s: %s", ANSI_BACK_CUR,
         sub == strstr(sub, data->chan) ? ANSI_DEFAULT : ANSI_ERROR,
         ANSI_BOLD, (int)(strchr(str, '!') - (str + 1)), str + 1,
         ANSI_DEFAULT, strchr(sub, ':') + 1);
  return (0);
}
