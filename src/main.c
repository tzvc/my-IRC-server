/*
** main.c for myftp in /home/rootkid/rendu/psu/PSU_2016_myftp
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Tue May  9 13:57:08 2017 theo champion
** Last update Wed May 24 15:51:45 2017 theo champion
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
  struct sockaddr_in	l_addr;
  struct sockaddr_in	r_addr;

  int max_client = 30;
  int clients[30];
  int new_socket;
  int i;
  int activity;

  //log_msg(INFO, "Waiting for incoming connections...");
  if ((g_socket_fd = create_s_socket(&l_addr, port)) == -1)
    return (-1);
  listen(g_socket_fd, 5);
  for (i = 0; i < max_client; i++)
    clients[i] = 0;
  while (g_run_server)
    {
      FD_ZERO(&fds);
      FD_SET(g_socket_fd, &fds);
      fd_max = g_socket_fd;
      for (i = 0; i < max_client; i++)
	{
	  if (clients[i] > 0)
	    FD_SET(clients[i], &fds);
	  if (clients[i] > fd_max)
	    fd_max = clients[i];
	  activity = select(fd_max + 1 , &fds , NULL , NULL , NULL);
	  if ((activity < 0) && (errno!=EINTR))
	    {
	      printf("select error");
	    }
	  if (FD_ISSET(g_socket_fd, &fds))
	    {
	      new_socket = accept_con(g_socket_fd, &r_addr);
	      printf("New connection , socket fd is %d , ip is : %s , port : %d \n",
		     new_socket,
		     inet_ntoa(r_addr.sin_addr),
		     ntohs(r_addr.sin_port));
	      for (i = 0; i < max_client; i++)
		{
		  if (clients[i] == 0) {
		    clients[i] = new_socket;
		    break;
		  }
		}
	    }
	}
    }
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
