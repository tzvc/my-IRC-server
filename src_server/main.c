/*
** main.c for myftp in /home/rootkid/rendu/psu/PSU_2016_myftp
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Tue May  9 13:57:08 2017 theo champion
** Last update Wed May 31 16:42:12 2017 theo champion
*/

#include "irc_server.h"

static volatile bool	g_run_server;
static int		g_socket_fd;

void	signal_handler(int signal)
{
  (void)signal;
  log_msg(INFO, "Ctrl-C catched, shutting down server...");
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

static void		accept_new_user(t_handle *hdl, t_user **users)
{
  struct sockaddr_in	r_addr;
  socklen_t		addrlen;
  int			nsock;

  nsock = accept_con(g_socket_fd, &r_addr);
  log_msg(INFO, "Incoming connection from %s", inet_ntoa(r_addr.sin_addr));
  add_user(users,
           create_user(nsock, NULL, strdup(inet_ntoa(r_addr.sin_addr)), false));
  if (!hdl->server_ip)
    {
      addrlen = sizeof(r_addr);
      getsockname(nsock, (struct sockaddr *)&r_addr, &addrlen);
      hdl->server_ip = strdup(inet_ntoa(r_addr.sin_addr));
      log_msg(INFO, "Server IP is %s", hdl->server_ip);
    }
}

static int		start_service(int port)
{
  fd_set		fds;
  int			fd_max;
  t_user		*users;
  t_chan		*chans;
  t_handle		hdl;
  struct sockaddr_in	l_addr;

  if ((g_socket_fd = create_s_socket(&l_addr, port)) == -1)
    return (-1);
  listen(g_socket_fd, MAX_QUEUE);
  users = NULL;
  chans = NULL;
  hdl.users = &users;
  hdl.chans = &chans;
  hdl.server_ip = NULL;
  while (g_run_server)
    {
      update_fdset(&fds, &fd_max, users);
      if (select(fd_max + 1, &fds, NULL, NULL, NULL) < 0)
        break;
      if (FD_ISSET(g_socket_fd, &fds))
        accept_new_user(&hdl, &users);
      handle_clients(&hdl, &fds);
    }
  if (hdl.server_ip)
    free(hdl.server_ip);
  free_all_chans(&chans);
  free_all_users(&users);
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
