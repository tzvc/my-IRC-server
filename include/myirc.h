/*
** myirc.h for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Wed May 24 14:53:23 2017 bufferking
** Last update Wed May 24 15:28:05 2017 
*/

#ifndef MY_IRC_C
#define MY_IRC_C

#include <stdlib.h>

typedef struct	s_user
{
  int		fd;
  char		*nick;
  char		*host;
  struct s_user	*next;
}		t_user;

#endif /* MY_IRC_C */
