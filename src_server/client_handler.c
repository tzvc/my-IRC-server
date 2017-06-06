/*
** client_handler.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed May 24 17:08:25 2017 theo champion
** Last update Tue Jun  6 15:31:03 2017 theo champion
*/

#include "rfc_numlist.h"
#include "irc_server.h"

const char	*g_cmd_list[] =
  {
    "NICK", "USER", "QUIT", "JOIN", "PART", "PRIVMSG",
    "LIST", "NAMES", "PING"
  };

cmd_funcptr	g_funcptr_list[] =
  {
    cmd_nick, cmd_user, cmd_quit, cmd_join, cmd_part, cmd_privmsg,
    cmd_list, cmd_names, cmd_ping
  };

static void	parse_cmd(t_handle *hdl, char *raw)
{
  char		*token;
  char		*spe_arg;
  int		i;

  i = 0;
  hdl->cmd_nb = -1;
  while (i < (int)(sizeof(hdl->cmd_args) / sizeof(hdl->cmd_args[0])))
    hdl->cmd_args[i++] = NULL;
  i = 0;
  while (raw[i] && raw[i] != SPE_DELIM)
    i++;
  spe_arg = (raw[i] ? &raw[i + 1] : NULL);
  raw[i] = 0;
  i = -1;
  if ((token = strtok(raw, POSIX_WS)) == NULL)
    return;
  while (++i < (int)(sizeof(g_cmd_list) / sizeof(g_cmd_list[0])))
    if (strcmp(token, g_cmd_list[i]) == 0)
      hdl->cmd_nb = i;
  i = 0;
  if (hdl->cmd_nb >= 0)
    while (((token = strtok(NULL, POSIX_WS)) != NULL) && i < MAX_ARGS)
      hdl->cmd_args[i++] = token;
  if (i < MAX_ARGS && spe_arg)
    hdl->cmd_args[i] = spe_arg;
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
  char		raw[BUF_SIZE];
  char		*cmd;
  size_t	len;
  ssize_t	rd;

  len = rb_get_space(hdl->sender->rb);
  if ((rd = recv(hdl->sender->fd, raw, len, 0)) > 0)
    {
      raw[rd] = 0;
      rb_write(hdl->sender->rb, raw);
      while ((cmd = rb_readline(hdl->sender->rb)) != NULL)
        {
          parse_cmd(hdl, cmd);
          exec_cmd(hdl);
          free(cmd);
        }
    }
  else if (rd == 0)
    {
      log_msg(INFO, "User disconnected");
      cmd_quit(hdl);
    }
  else
    log_msg(ERROR, "recv: %s", strerror(errno));
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
