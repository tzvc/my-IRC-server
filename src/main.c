/*
** main.c for myftp in /home/rootkid/rendu/psu/PSU_2016_myftp
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Tue May  9 13:57:08 2017 theo champion
** Last update Wed May 24 19:03:07 2017 theo champion
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

static void	update_fdset(fd_set *fds, int *fd_max, t_user *users)
{
  t_user	*tmp;

  *fd_max = g_socket_fd;
  FD_ZERO(fds);
  FD_SET(g_socket_fd, fds);
  tmp = users;
  while (tmp)
    {
      FD_SET(tmp->fd, fds);
      *fd_max = (tmp->fd > *fd_max ? tmp->fd : *fd_max);
      tmp = tmp->next;
    }
}

static int		start_service(int port)
{
  fd_set		fds;
  int			fd_max;
  t_user		*users;
  t_chan		*channels;
  struct sockaddr_in	l_addr;
  struct sockaddr_in	r_addr;
  int			nsock;

  if ((g_socket_fd = create_s_socket(&l_addr, port)) == -1)
    return (-1);
  listen(g_socket_fd, MAX_QUEUE);
  users = NULL;
  channels = NULL;
  while (g_run_server)
    {
      update_fdset(&fds, &fd_max, users);
      if (select(fd_max + 1, &fds, NULL, NULL, NULL) < 0)
        break;
      if (FD_ISSET(g_socket_fd, &fds))
        {
          nsock = accept_con(g_socket_fd, &r_addr);
          new_user(&users, nsock, NULL, inet_ntoa(r_addr.sin_addr));
        }
      handle_clients(&fds, users, channels);
    }
  free_users(users);
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
