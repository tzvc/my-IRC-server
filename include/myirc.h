/*
** myirc.h for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Wed May 24 14:53:23 2017 bufferking
** Last update Wed May 24 15:52:15 2017 theo champion
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


//SOCKET.C
int	create_s_socket(struct sockaddr_in *sock, int port);
int	accept_con(int socket_fd, struct sockaddr_in *r_addr);

#endif /* MY_IRC_C */
