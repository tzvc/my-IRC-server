/*
** irc_common.h for PSU_2016_myirc in /home/bufferking/rendu/network/PSU_2016_myirc
** 
** Made by 
** Login   <antoine.cauquil@epitech.eu>
** 
** Started on  Fri Jun  2 14:22:24 2017 
** Last update Fri Jun  2 14:33:22 2017 theo champion
*/

#ifndef IRC_COMMON_H_
#define IRC_COMMON_H_

#define BUF_SIZE 1024
#define POSIX_WS " \t\r\n\v\f"

typedef struct	s_rb
{
  char		*buf;
  char		*wend;
  char		*rend;
}		t_rb;

#endif /* !IRC_COMMON_H_ */
