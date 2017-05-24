/*
** main.c for myftp in /home/rootkid/rendu/psu/PSU_2016_myftp
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Tue May  9 13:57:08 2017 theo champion
** Last update Wed May 24 16:49:48 2017 theo champion
*/

#include "myirc.h"

static volatile bool	g_run_server;
static int		g_socket_fd;

void	signal_handler(int signal)
{
  (void)signal;
  // log_msg(INFO, "Ctrl-C catched, shutting down server...");
  shutdown(g_socket_fd, SHUT_RDWR);
  g_run_server = false;
}

static int		start_service(int port)
{
  fd_set		fds;
  int			fd_max;
  t_user		*users;
  t_user		*tmp;
  struct sockaddr_in	l_addr;
  struct sockaddr_in	r_addr;
  int			activity;
  int			new_socket;

  //log_msg(INFO, "Waiting for incoming connections...");
  if ((g_socket_fd = create_s_socket(&l_addr, port)) == -1)
    return (-1);
  listen(g_socket_fd, 5);
  users = NULL;
  while (g_run_server)
    {
      fd_max = g_socket_fd;
      FD_ZERO(&fds);
      FD_SET(g_socket_fd, &fds);
      tmp = users;
      while (tmp)
	{
	  FD_SET(tmp->fd, &fds);
	  fd_max = (tmp->fd > fd_max ? tmp->fd : fd_max);
	  tmp = tmp->next;
	}
      activity = select(fd_max + 1, &fds, NULL, NULL, NULL);
      if ((activity < 0) && (errno != EINTR))
        return (-1);
      if (FD_ISSET(g_socket_fd, &fds))
	{
	  new_socket = accept_con(g_socket_fd, &r_addr);
	  new_user(&users, new_socket, NULL, inet_ntoa(r_addr.sin_addr));
	  tmp = users;
	  printf("Users:\n");
	  while (tmp)
	    {
	      printf("fd:\t%d\tnick:\t%s\thost:\t%s\n", tmp->fd, tmp->nick, tmp->host);
	      tmp = tmp->next;
	    }
	}
    }
  //free all stuff mamene
  return (0);
}

int			main(int argc, char **argv)
{
  int			port;

  g_run_server = true;
  signal(SIGINT, signal_handler);
  if (argc < 2)
    printf("Usage : %s port\n", argv[0]);
  else if ((port = (int)strtol(argv[1], NULL, 10)) <= 0)
    fprintf(stderr, "Invalid port number\n");
  else if (start_service(port) == -1)
    fprintf(stderr, "Error launching server: %s\n", strerror(errno));
  else
    return (0);
  return (1);
}
