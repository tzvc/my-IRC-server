/*
** client_manager.c for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Wed May 24 15:01:09 2017 bufferking
** Last update Thu May 25 11:25:37 2017 theo champion
*/

#include "myirc.h"

size_t		count_chans(t_chan **chans)
{
  size_t	i;
  t_chan	*tmp;
  
  i = 0;
  tmp = *chans;
  while (tmp)
    {
      printf("listed chan : %s\n", tmp->name);
      tmp = tmp->next;
      i++;
    }
  return (i);
}

int		del_chan(t_chan **chans, t_chan *old)
{
  t_chan	*tmp;
  
  tmp = *chans;
  if (!tmp)
    return (-1);
  if (tmp == old)
    {
      *chans = tmp->next;
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

int		new_chan(t_chan **chans, char *name, char *topic)
{
  t_chan	*new;
  t_chan	*tmp;
  
  if (!(new = malloc(sizeof(t_chan))))
    return (-1);
  new->name = name;
  new->topic = topic;
  new->users = NULL;
  new->next = NULL;
  if (!(*chans))
      *chans = new;
  else
    {
      tmp = *chans;
      while (tmp->next)
	tmp = tmp->next;
      tmp->next = new;
    }
  return (0);
}

void		free_chans(t_chan *chans)
{
  t_chan	*prev;

  while (chans)
    {
      prev = chans;
      chans = chans->next;
      if (prev->name)
        free(prev->name);
      if (prev->topic)
        free(prev->topic);
      free(prev);
    }
}
