/*
** client_handler.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed May 24 17:08:25 2017 theo champion
** Last update Fri May 26 15:06:12 2017 theo champion
*/

#include "irc_server.h"

const char	*g_cmd_list[] =
  {
    "NICK", "JOIN", "LIST"
  };

cmd_funcptr	g_funcptr_list[] =
  {
    cmd_nick, cmd_join, cmd_list
  };

static int	recv_and_parse_cmd(t_handle *hdl)
{
  char		*raw;
  char		*token;
  size_t	len;
  FILE		*input_stream;
  int		i;

  i = -1;
  len = 0;
  raw = NULL;
  if ((input_stream = fdopen(dup(hdl->sender->fd), "r")) == NULL)
    return (-1);
  if (getline(&raw, &len, input_stream) == -1)
    return (-1);
  fclose(input_stream);
  if ((token = strtok(raw, POSIX_WS)) != NULL)
    {
      while (++i < (int)(sizeof(g_cmd_list) / sizeof(g_cmd_list[0])))
        if (strcmp(token, g_cmd_list[i]) == 0)
          hdl->cmd_nb = i;
      i = 0;
      if (hdl->cmd_nb >= 0)
        while ((token = strtok(NULL, POSIX_WS)) != NULL)
          hdl->cmd_args[i++] = strdup(token);
    }
  free(raw);
  return (0);
}

bool		reply(t_handle *hdl, int code, const char *fmt, ...)
{
  char		*text;
  char		*reply;
  va_list	ap;
  size_t	len;

  va_start(ap, fmt);
  len = vsnprintf(NULL, 0, fmt, ap);
  va_start(ap, fmt);
  if ((text = malloc(sizeof(char) * len + 1)) == NULL)
    return (false);
  vsprintf(text, fmt, ap);
  if ((reply = malloc(strlen(text) +
                      strlen((hdl->sender->nick ? hdl->sender->nick : "*")) +
                      12 + strlen(hdl->server_ip))) == NULL)
    return (false);
  sprintf(reply, ":%s %d %s %s \r\n", hdl->server_ip, code,
          (hdl->sender->nick ? hdl->sender->nick : "*"), text);
  write(hdl->sender->fd, reply, strlen(reply));
  free(text);
  free(reply);
  va_end(ap);
  return (code >= 400 ? false : true);
}

int			handle_clients(t_handle *hdl, fd_set *fds)
{
  t_user		*tmp;

  tmp = *hdl->users;
  while (tmp)
    {
      if (FD_ISSET(tmp->fd, fds))
        {
          hdl->cmd_nb = -1;
          hdl->sender = tmp;
	  hdl->cmd_args[0] = NULL;
	  hdl->cmd_args[1] = NULL;
          recv_and_parse_cmd(hdl);
	  if (hdl->cmd_nb >= 0)
	    {
	      log_msg(DEBUG, "Executing \"%s\" with params \"%s\" \"%s\"",
		      g_cmd_list[hdl->cmd_nb], hdl->cmd_args[0], hdl->cmd_args[1]);
	      g_funcptr_list[hdl->cmd_nb](hdl);
	    }
	  else
	    log_msg(ERROR, "Unknown command.");
	  //free cmd args
        }
      tmp = tmp->next;
    }
  return (0);
}
