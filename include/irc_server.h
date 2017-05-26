/*
** irc_server.h for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Fri May 26 13:10:36 2017 theo champion
** Last update Fri May 26 13:11:46 2017 theo champion
*/

#ifndef IRC_SERVER_H_
#define IRC_SERVER_H_

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
#include <stdarg.h>

#define LAST_CMD 2
#define MAX_QUEUE 42
#define POSIX_WS " \t\r\n\v\f"

#define INFO 0
#define DEBUG 1
#define ERROR 2

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

typedef struct	s_handle
{
  int		cmd_nb;
  char		*cmd_args[2];
  t_user	*sender;
  t_user	**users;
  t_chan	**chans;
  char		*server_ip;
}		t_handle;

typedef bool	(*cmd_funcptr)(t_handle *hdl);

///	socket.c	///
int	create_s_socket(struct sockaddr_in *sock, int port);
int	accept_con(int socket_fd, struct sockaddr_in *r_addr);
///	user_manager.c	///
size_t	count_users(t_user **users);
int	del_user(t_user **users, t_user *old);
int	new_user(t_user **users, int fd, char *nick, char *host);
t_user	*find_user_by_nick(t_user **users, char *nick);
void	free_users(t_user *users);
///	chan_manager.c	///
size_t	count_chans(t_chan **chans);
int	del_chan(t_chan **chans, t_chan *old);
t_chan	*new_chan(t_chan **chans, char *name, char *topic);
t_chan	*find_chan_by_name(t_chan **chans, char *name);
void	free_chans(t_chan *chans);
///	client_handler.c	///
bool	reply(t_handle *hdl, int code, const char *fmt, ...);
int	handle_clients(t_handle *hdl, fd_set *fds);
///	connection_registration.c	///
bool	cmd_nick(t_handle *hdl);
///	utils.c		///
void	log_msg(int mode, const char *fmt, ...);

#endif /* !IRC_SERVER_H_ */
