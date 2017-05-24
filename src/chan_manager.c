/*
** client_manager.c for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Wed May 24 15:01:09 2017 bufferking
** Last update Wed May 24 16:49:14 2017 
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

int		main(void)
{
  t_chan	*chans;

  chans = NULL;
  new_chan(&chans, "bite", "une bonne grosse bite");
  printf("%d\n", count_chans(&chans));
  del_chan(&chans, chans);
  printf("%d\n", count_chans(&chans));
  return (0);
}
