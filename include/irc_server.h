/*
** irc_server.h for myirc in /home/rootkid/rendu/psu/PSU_2016_myirc
** 
** Made by theo champion
** Login   <theo.champion@epitech.eu>
** 
** Started on  Fri May 26 13:10:36 2017 theo champion
** Last update Thu Jun  8 17:39:49 2017 theo champion
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
#include <ctype.h>
#include "irc_common.h"

#define MAX_QUEUE 42
#define MAX_ARGS 4
#define MAXNICK 9
#define SPE_DELIM ':'
#define REG_NEEDED 3
#define MUL_PARAM 7
#define CHANMASK "&#+!"

#define INFO 0
#define DEBUG 1
#define ERROR 2

#define NOT_REGISTERED 0
#define NICK_OK 1
#define USER_OK 2
#define REGISTERED 3
#define DEAD 4

typedef struct	s_user
{
  int		fd;
  char		*nick;
  char		*username;
  char		*hostname;
  char		*realname;
  int		status;
  t_rb		*rb;
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
  char		*arg[MAX_ARGS];
  t_user	*sdr;
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
t_user	*create_user(int fd, char *nick, char *host, bool member);
bool	add_user(t_user **users, t_user *new);
t_user	*find_user_by_nick(t_user **users, char *nick);
t_user	*find_user_by_fd(t_user **users, int fd);
//FREE.C
void	free_all_users(t_user **users);
void	free_user(t_user *user);
void	free_all_chans(t_chan **chans);
void	free_chan(t_chan *chan);
///	chan_manager.c	///
size_t	count_chans(t_chan **chans);
t_chan	*del_chan(t_chan **chans, t_chan *old);
t_chan	*new_chan(t_chan **chans, char *name);
t_chan	*find_chan_by_name(t_chan **chans, char *name);
int	remove_user(t_user **users, t_user *toremove);
///COMMUNICATION.C
bool	reply(t_handle *hdl, int code, const char *fmt, ...);
bool	idreply(int fd, t_handle *hdl, const char *fmt, ...);
bool	broadcast(t_handle *hdl, t_chan *channel, const char *fmt, ...);
//RING_BUFFER.C
t_rb	*rb_init(void);
size_t	rb_get_space(t_rb *rb);
void	rb_write(t_rb *rb, char *data);
char	*rb_readline(t_rb *rb);
///	client_handler.c	///
int	handle_clients(t_handle *hdl, fd_set *fds);
///	interaction.c	///
bool	cmd_nick(t_handle *hdl);
bool	cmd_user(t_handle *hdl);
bool	cmd_quit(t_handle *hdl);
//CHAN_INTERACTION.C
bool	cmd_join(t_handle *hdl);
bool	cmd_topic(t_handle *hdl);
bool	cmd_part(t_handle *hdl);
//MESSAGES.C
bool	cmd_privmsg(t_handle *hdl);
///	server_infos.c	///
void	welcome_user(t_handle *hdl);
bool	reply_names(t_handle *hdl, t_chan *channel);
bool	cmd_list(t_handle *hdl);
bool	cmd_names(t_handle *hdl);
bool	cmd_ping(t_handle *hdl);
///	utils.c		///
void	log_msg(int mode, const char *fmt, ...);
void	init_handler(t_handle *hdl, t_user **users, t_chan **chans);

#endif /* !IRC_SERVER_H_ */
