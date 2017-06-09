/*
** client_handler.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Wed May 24 17:08:25 2017 theo champion
<<<<<<< HEAD
** Last update Thu Jun  8 18:21:15 2017 theo champion
=======
** Last update Thu Jun  8 13:30:16 2017 
>>>>>>> bcc1e35e029d73ea4526a64182f5b3ebea033e8b
*/

#include "rfc_numlist.h"
#include "irc_server.h"

const char	*g_cmd_list[] =
  {
    "NICK", "USER", "QUIT", "JOIN", "PART",
    "NAMES", "LIST", "TOPIC", "PRIVMSG", "PING"
  };

cmd_funcptr	g_funcptr_list[] =
  {
    cmd_nick, cmd_user, cmd_quit, cmd_join, cmd_part,
    cmd_names, cmd_list, cmd_topic, cmd_privmsg, cmd_ping
  };

static void	parse_cmd(t_handle *h, char *raw)
{
  char		*token;
  char		*spe_arg;
  int		i;

  i = 0;
  h->cmd_nb = -1;
  while (i < (int)(sizeof(h->arg) / sizeof(h->arg[0])))
    h->arg[i++] = NULL;
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
      h->cmd_nb = i;
  i = 0;
  if (h->cmd_nb >= 0)
    while (((token = strtok(NULL, POSIX_WS)) != NULL) && i < MAX_ARGS)
      h->arg[i++] = token;
  if (i < MAX_ARGS && spe_arg)
    h->arg[i] = spe_arg;
}

static void	exec_cmd(t_handle *h)
{
  if (h->cmd_nb >= 0)
    {
      log_msg(DEBUG, "Executing \"%s\" with params \"%s\" \"%s\" \"%s\" \"%s\"",
              g_cmd_list[h->cmd_nb], h->arg[0], h->arg[1],
              h->arg[2], h->arg[3]);
      if (h->cmd_nb >= REG_NEEDED && h->sdr->status != REGISTERED)
        reply(h, ERR_NOTREGISTERED, "%s :You have not registered",
              g_cmd_list[h->cmd_nb]);
      else
        g_funcptr_list[h->cmd_nb](h);
    }
  else
    {
      log_msg(ERROR, "Unknown command.");
      reply(h, ERR_UNKNOWNCOMMAND, ":Unknown command");
    }
}

static bool	handle_cmd_execution(t_handle *h)
{
  char		*orig;
  char		*param;

  if (h->arg[0] && h->cmd_nb >= REG_NEEDED && h->cmd_nb <= MUL_PARAM)
    {
      if ((orig = strdup(h->arg[0])) == NULL)
        return (reply(h, ERR_UNKNOWNERROR, "%s", strerror(errno)));
      param = strtok(orig, ",");
      while (param)
        {
          h->arg[0] = param;
          exec_cmd(h);
          param = strtok(NULL, ",");
        }
      free(orig);
    }
  else
    exec_cmd(h);
  return (true);
}

static bool	recv_and_execute(t_handle *h)
{
  char		raw[BUF_SIZE];
  char		*cmd;
  size_t	len;
  ssize_t	rd;

  len = rb_get_space(h->sdr->rb);
  if ((rd = recv(h->sdr->fd, raw, len, 0)) > 0)
    {
      raw[rd] = 0;
      rb_write(h->sdr->rb, raw);
      while ((cmd = rb_readline(h->sdr->rb)) != NULL)
        {
          parse_cmd(h, cmd);
          handle_cmd_execution(h);
          free(cmd);
        }
    }
  else if (rd == 0)
    {
      log_msg(INFO, "User disconnected");
      cmd_quit(h);
    }
  else
    log_msg(ERROR, "recv: %s", strerror(errno));
  return (true);
}

int			handle_clients(t_handle *h, fd_set *fds)
{
  t_user		*tmp;

  tmp = *h->users;
  while (tmp)
    {
      if (FD_ISSET(tmp->fd, fds))
        {
          h->sdr = tmp;
          recv_and_execute(h);
          tmp = tmp->next;
          if (h->sdr->status == DEAD)
            del_user(h->users, h->sdr);
        }
      else
        tmp = tmp->next;
    }
  return (0);
}
