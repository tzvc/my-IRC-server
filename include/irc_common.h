/*
** irc_common.h for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by bufferking
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Fri Jun  2 14:22:24 2017 bufferking
** Last update Sun Jun 11 17:01:37 2017 bufferking
*/

#ifndef IRC_COMMON_H_
#define IRC_COMMON_H_

#include <sys/types.h>

#define BUF_SIZE 1024
#define POSIX_WS " \t\r\n\v\f"

#define ANSI_INFO "\033[92m"
#define ANSI_MSG "\033[94m"
#define ANSI_ERROR "\033[91m"
#define ANSI_BOLD "\033[1m"
#define ANSI_DEFAULT "\033[0m"
#define ANSI_BACK_CUR "\033[1000D"

typedef struct	s_rb
{
  char		*buf;
  char		*wend;
  char		*rend;
}		t_rb;

//RING_BUFFER.C
t_rb	*rb_init(void);
size_t	rb_get_space(t_rb *rb);
void	rb_write(t_rb *rb, char *data);
char	*rb_readline(t_rb *rb);

#endif /* !IRC_COMMON_H_ */
