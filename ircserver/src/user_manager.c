/*
** client_manager.c for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Wed May 24 15:01:09 2017 bufferking
** Last update Fri May 26 12:59:05 2017 theo champion
*/

#include "myirc.h"

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

int		del_user(t_user **users, t_user *old)
{
  t_user	*tmp;

  tmp = *users;
  if (!tmp)
    return (-1);
  if (tmp == old)
    {
      *users = tmp->next;
      free(tmp);
      return (0);
    }
  while (tmp)
    {
      if (tmp->next == old)
        {
          tmp->next = old->next;
          free(old);
          return (0);
        }
      tmp = tmp->next;
    }
  return (-1);
}

int		new_user(t_user **users, int fd, char *nick, char *host)
{
  t_user	*new;
  t_user	*tmp;

  if (!(new = malloc(sizeof(t_user))))
    return (-1);
  new->fd = fd;
  new->nick = nick;
  new->host = host;
  new->next = NULL;
  if (!(*users))
    *users = new;
  else
    {
      tmp = *users;
      while (tmp->next)
        tmp = tmp->next;
      tmp->next = new;
    }
  return (0);
}

t_user		*find_user_by_nick(t_user **users, char *nick)
{
  t_user	*tmp;

  tmp = *users;
  while (tmp)
    {
      if (tmp->nick && strcmp(tmp->nick, nick) == 0)
        return (tmp);
      tmp = tmp->next;
    }
  return (NULL);
}

void		free_users(t_user *users)
{
  t_user	*prev;

  while (users)
    {
      prev = users;
      users = users->next;
      if (prev->nick)
        free(prev->nick);
      if (prev->host)
        free(prev->host);
      free(prev);
    }
}
