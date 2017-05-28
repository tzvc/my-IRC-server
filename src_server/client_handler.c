/*
** client_handler.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed May 24 17:08:25 2017 theo champion
** Last update Sun May 28 20:18:23 2017 theo champion
*/

#include "rfc_numlist.h"
#include "irc_server.h"

const char	*g_cmd_list[] =
  {
    "NICK", "USER", "QUIT", "JOIN", "LIST", "PART"
  };

cmd_funcptr	g_funcptr_list[] =
  {
    cmd_nick, cmd_user, cmd_quit, cmd_join, cmd_list, cmd_part
  };

static void	parse_cmd(t_handle *hdl, char *raw)
{
  char		*token;
  int		i;

  i = 0;
  hdl->cmd_nb = -1;
  while (i < (int)(sizeof(hdl->cmd_args) / sizeof(hdl->cmd_args[0])))
    hdl->cmd_args[i++] = NULL;
  i = -1;
  if ((token = strtok(raw, POSIX_WS)) != NULL)
    {

      while (++i < (int)(sizeof(g_cmd_list) / sizeof(g_cmd_list[0]))) {
	printf("i = %d\n", i);
        if (strcmp(token, g_cmd_list[i]) == 0) {
	  printf("%s %s\n", token, g_cmd_list[i]);
          hdl->cmd_nb = i;
	}
      }
      printf("token = %s %d\n", token, hdl->cmd_nb);
      i = 0;
      if (hdl->cmd_nb >= 0)
        while ((token = strtok(NULL, POSIX_WS)) != NULL)
          hdl->cmd_args[i++] = token;
    }
}

static void	exec_cmd(t_handle *hdl)
{
  if (hdl->cmd_nb >= 0)
    {
      log_msg(DEBUG, "Executing \"%s\" with params \"%s\" \"%s\" \"%s\" \"%s\"",
              g_cmd_list[hdl->cmd_nb], hdl->cmd_args[0], hdl->cmd_args[1],
	      hdl->cmd_args[2], hdl->cmd_args[3]);
      if (hdl->cmd_nb >= REG_NEEDED && hdl->sender->status != REGISTERED)
	reply(hdl, ERR_NOTREGISTERED, "%s :You have not registered",
	      g_cmd_list[hdl->cmd_nb]);
      else
	g_funcptr_list[hdl->cmd_nb](hdl);
    }
  else
    {
      log_msg(ERROR, "Unknown command.");
      reply(hdl, ERR_UNKNOWNCOMMAND, ":Unknown command");
    }
}

static bool	recv_and_execute(t_handle *hdl)
{
  char		*raw;
  size_t	len;
  FILE		*input_stream;
  ssize_t	nread;

  len = 0;
  raw = NULL;
  if ((input_stream = fdopen(dup(hdl->sender->fd), "r")) == NULL)
    return (false);
  while ((nread = getline(&raw, &len, input_stream)) > 0)
    {
      parse_cmd(hdl, raw);
      exec_cmd(hdl);
    }
  free(raw);
  fclose(input_stream);
  return (true);
}

int			handle_clients(t_handle *hdl, fd_set *fds)
{
  t_user		*tmp;

  tmp = *hdl->users;
  while (tmp)
    {
      if (FD_ISSET(tmp->fd, fds))
        {
          hdl->sender = tmp;
          recv_and_execute(hdl);
	  tmp = tmp->next;
	  if (hdl->sender->status == DEAD)
	    del_user(hdl->users, hdl->sender);
        }
      else
        tmp = tmp->next;
    }
  return (0);
}
