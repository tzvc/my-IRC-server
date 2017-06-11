/*
** utils.c for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Thu May 25 19:02:40 2017 theo champion
** Last update Sun Jun 11 17:20:51 2017 theo champion
*/

#include "irc_server.h"

void		log_msg(int mode, const char *fmt, ...)
{
  va_list	ap;

  va_start(ap, fmt);
  if (mode == INFO)
    fprintf(stderr, "\033[34m[+] ");
  else if (mode == ERROR)
    fprintf(stderr, "\033[31m[!] ");
  else
    fprintf(stderr, "\033[33m[-] ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n\033[0m");
  va_end(ap);
}

void	init_handler(t_handle *h, t_user **users, t_chan **chans)
{
  *users = NULL;
  *chans = NULL;
  h->users = users;
  h->chans = chans;
  h->server_ip = NULL;
}

size_t		count_users(t_user **users)
{
  size_t	i;
  t_user	*tmp;

  i = 0;
  tmp = *users;
  while (tmp)
    {
      printf("listed user : %s\n", tmp->nick);
      tmp = tmp->next;
      i++;
    }
  return (i);
}

bool	is_valid_nick(char *nick)
{
  int	i;

  if (isdigit(nick[0]) > 0)
    return (false);
  i = 0;
  while (nick[i])
    {
      if (strchr(INVALID_CHAR, nick[i++]))
        return (false);
    }
  return (true);
}
