/*
** myirc.h for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Wed May 24 14:53:23 2017 bufferking
** Last update Wed May 24 16:51:05 2017 
*/

#ifndef MY_IRC_C
#define MY_IRC_C

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct	s_user
{
  int		fd;
  char		*nick;
  char		*host;
  struct s_user	*next;
}		t_user;

typedef struct	s_chan
{
  char		*name;
  char		*topic;
  t_user	*users;
  struct s_chan	*next;
}		t_chan;

///	socket.c	///
int	create_s_socket(struct sockaddr_in *sock, int port);
int	accept_con(int socket_fd, struct sockaddr_in *r_addr);
///	user_manager.c	///
size_t	count_users(t_user **users);
int	del_user(t_user **users, t_user *old);
int	new_user(t_user **users, int fd, char *nick, char *host);

#endif /* !MY_IRC_C */
